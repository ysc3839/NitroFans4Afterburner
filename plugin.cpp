#include "pch.h"
#include "NitroAPI.hpp"

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

BOOL APIENTRY DllMain([[maybe_unused]] HMODULE hModule, [[maybe_unused]] DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
#ifdef _DLL // The static CRT requires DLL_THREAD_ATTACH and DLL_THREAD_DETATCH notifications to function properly.
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		DisableThreadLibraryCalls(hModule);
#endif
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
	NitroAPI::SystemHealthInformationIndex i;
	switch (index)
	{
	case 0:
		i = NitroAPI::SystemHealthInformationIndex::CPUFanSpeed;
		break;
	case 1:
		i = NitroAPI::SystemHealthInformationIndex::GPUFanSpeed;
		break;
	default:
		return FLT_MAX;
	}

	try
	{
		const auto result = NitroAPI::GetWMISystemHealthInfo(i);
		return static_cast<FLOAT>(result);
	}
	CATCH_LOG();
	return FLT_MAX;
}

PLUGIN_API void Uninit()
{
	NitroAPI::Cleanup();
}
