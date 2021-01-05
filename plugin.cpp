#include "pch.h"
#include "NitroAPI.hpp"
#include "WMIAPI.hpp"

#define PLUGIN_API extern "C" __declspec(dllexport)

struct DataSource
{
	std::string_view name;
	std::string_view units;
	std::string_view group;
	DWORD id;
};

constexpr DataSource DataSources[] = { {
	"CPU fan speed",
	"RPM",
	"CPU",
	MONITORING_SOURCE_ID_PLUGIN_CPU
}, {
	"GPU fan speed",
	"RPM",
	"GPU",
	MONITORING_SOURCE_ID_PLUGIN_GPU
} };

enum class API
{
	WMI,
	NitroSense
};

HMODULE g_hModule = nullptr;
API g_API = API::WMI;

auto GetCfgPath()
{
	return GetModuleFsPath(g_hModule).replace_extension(L".cfg");
}

void ReadConfig()
{
	const auto api = GetPrivateProfileIntW(L"Settings", L"API", 0, GetCfgPath().c_str());
	g_API = static_cast<API>(api);
}

BOOL APIENTRY DllMain(HMODULE hModule, [[maybe_unused]] DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_hModule = hModule;

		ReadConfig();

#ifdef _DLL // The static CRT requires DLL_THREAD_ATTACH and DLL_THREAD_DETATCH notifications to function properly.
		DisableThreadLibraryCalls(hModule);
#endif
	}
	return TRUE;
}

PLUGIN_API BOOL SetupSource([[maybe_unused]] DWORD index, HWND hWnd)
{
	if (hWnd)
	{
		ReadConfig();

		TASKDIALOG_BUTTON buttons[] = { {
			.nButtonID = 0,
			.pszButtonText = L"WMI (recommended)\nRequires no dependency."
		}, {
			.nButtonID = 1,
			.pszButtonText = L"NitroSense Service\nRequires NitroSense installed."
		} };
		TASKDIALOGCONFIG config = {
			.cbSize = sizeof(config),
			.hwndParent = hWnd,
			.dwFlags = TDF_POSITION_RELATIVE_TO_WINDOW,
			.dwCommonButtons = TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON,
			.pszWindowTitle = L"NitroFans4Afterburner",
			.pszMainInstruction = L"Select which API to use",
			.nDefaultButton = IDOK,
			.cRadioButtons = static_cast<UINT>(std::size(buttons)),
			.pRadioButtons = buttons,
			.nDefaultRadioButton = static_cast<int>(g_API)
		};
		int button = 0, radioBtn = 0;
		if (SUCCEEDED(TaskDialogIndirect(&config, &button, &radioBtn, nullptr)) && button == IDOK)
		{
			g_API = static_cast<API>(radioBtn);

			const auto s = std::to_wstring(static_cast<int>(radioBtn));
			WritePrivateProfileStringW(L"Settings", L"API", s.c_str(), GetCfgPath().c_str());

			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

PLUGIN_API DWORD GetSourcesNum()
{
	return static_cast<DWORD>(std::size(DataSources));
}

PLUGIN_API BOOL GetSourceDesc(DWORD index, LPMONITORING_SOURCE_DESC desc)
{
	const size_t i = static_cast<size_t>(index);
	if (i < std::size(DataSources))
	{
		const auto& source = DataSources[index];

		source.name.copy(desc->szName, sizeof(desc->szName));
		source.units.copy(desc->szUnits, sizeof(desc->szUnits));
		source.group.copy(desc->szGroup, sizeof(desc->szGroup));
		desc->dwID = source.id;
		desc->dwInstance = index;

		return TRUE;
	}
	return FALSE;
}

PLUGIN_API FLOAT GetSourceData(DWORD index)
{
	AcerWMI::SystemHealthInformationIndex i;
	switch (index)
	{
	case 0:
		i = AcerWMI::SystemHealthInformationIndex::CPUFanSpeed;
		break;
	case 1:
		i = AcerWMI::SystemHealthInformationIndex::GPUFanSpeed;
		break;
	default:
		return FLT_MAX;
	}

	try
	{
		const auto result = (g_API == API::WMI) ? WMIAPI::GetWMISystemHealthInfo(i) : NitroAPI::GetWMISystemHealthInfo(i);
		return static_cast<FLOAT>(result);
	}
	CATCH_LOG();
	return FLT_MAX;
}

PLUGIN_API void Uninit()
{
	WMIAPI::Cleanup();
	NitroAPI::Cleanup();
}
