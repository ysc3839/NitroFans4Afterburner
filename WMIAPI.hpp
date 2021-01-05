#pragma once

#include "AcerWMI.hpp"

namespace WMIAPI
{
	namespace
	{
		bool s_init = false;
		wil::com_ptr<IWbemServices> s_namespace;
		wil::unique_bstr s_instPath;
		wil::unique_bstr s_methodName;
		wil::com_ptr<IWbemClassObject> s_inParams;
		wil::unique_variant s_paramCache[AcerWMI::SystemHealthInformationIndexCount] = {};

		void Init()
		{
			if (s_init)
				return;

			THROW_IF_FAILED(CoInitializeEx(0, COINIT_MULTITHREADED));

			THROW_IF_FAILED(CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_SECURE_REFS, nullptr));

			auto locator = wil::CoCreateInstance<IWbemLocator>(CLSID_WbemLocator);

			THROW_IF_FAILED(locator->ConnectServer(wil::make_bstr(LR"(ROOT\WMI)").get(), nullptr, nullptr, nullptr, 0, nullptr, nullptr, s_namespace.put()));

			auto className = wil::make_bstr(L"AcerGamingFunction");

			wil::com_ptr<IEnumWbemClassObject> instEnum;
#pragma warning(suppress: 5054) // operator '|': deprecated between enumerations of different types
			THROW_IF_FAILED(s_namespace->CreateInstanceEnum(className.get(), WBEM_FLAG_SHALLOW | WBEM_FLAG_FORWARD_ONLY, nullptr, instEnum.put()));

			wil::com_ptr<IWbemClassObject> inst;
			ULONG retCount = 0;
			THROW_IF_FAILED(instEnum->Next(WBEM_INFINITE, 1, inst.put(), &retCount));
			THROW_HR_IF(static_cast<HRESULT>(WBEM_E_NOT_FOUND), retCount != 1);

			VARIANT instPathVariant = {};
			CIMTYPE type;
			THROW_IF_FAILED(inst->Get(L"__PATH", 0, &instPathVariant, &type, nullptr));
			THROW_HR_IF(static_cast<HRESULT>(WBEM_E_TYPE_MISMATCH), type != CIM_STRING);

			s_instPath.reset(V_BSTR(&instPathVariant)); // Take ownership

			wil::com_ptr<IWbemClassObject> wmiClass;
			THROW_IF_FAILED(s_namespace->GetObject(className.get(), 0, nullptr, wmiClass.put(), nullptr));

			s_methodName = wil::make_bstr(L"GetGamingSysInfo");

			wil::com_ptr<IWbemClassObject> inParamsClass;
			THROW_IF_FAILED(wmiClass->GetMethod(s_methodName.get(), 0, inParamsClass.put(), nullptr));

			THROW_IF_FAILED(inParamsClass->SpawnInstance(0, s_inParams.put()));

			s_init = true;
		}

	}

	uint16_t GetWMISystemHealthInfo(AcerWMI::SystemHealthInformationIndex index)
	{
		Init();

		auto& v = s_paramCache[static_cast<size_t>(index)];
		if (V_VT(&v) == VT_EMPTY)
		{
			V_VT(&v) = VT_UI4;
			V_UI4(&v) = AcerWMI::GetGamingSysInfoInput(index);
			THROW_IF_FAILED(VariantChangeType(&v, &v, 0, VT_BSTR));
		}

		THROW_IF_FAILED(s_inParams->Put(L"gmInput", 0, &v, CIM_UINT32));

		wil::com_ptr<IWbemClassObject> outParams;
		THROW_IF_FAILED(s_namespace->ExecMethod(s_instPath.get(), s_methodName.get(), 0, nullptr, s_inParams.get(), outParams.put(), nullptr));

		wil::unique_variant output;
		CIMTYPE type;
		THROW_IF_FAILED(outParams->Get(L"gmOutput", 0, &output, &type, nullptr));
		THROW_HR_IF(static_cast<HRESULT>(WBEM_E_TYPE_MISMATCH), type != CIM_UINT64);
		THROW_IF_FAILED(VariantChangeType(&output, &output, 0, VT_UI8));

		const uint64_t result = V_UI8(&output);
		THROW_HR_IF(E_FAIL, (result & 0xff) != 0);
		return static_cast<uint16_t>(result >> 8);
	}

	void Cleanup()
	{
		s_instPath.reset();
		s_methodName.reset();
		s_inParams.reset();
		s_namespace.reset();

		for (auto& v : s_paramCache)
			v.reset();

		s_init = false;
	}
}
