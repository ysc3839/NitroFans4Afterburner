#pragma once

namespace AcerWMI
{
	enum struct SystemHealthInformationIndex : uint32_t
	{
		CPUTemperature = 1,
		CPUFanSpeed,
		SystemTemperature,
		SystemFanSpeed,
		FrostCore,
		GPUFanSpeed,
		System2Temperature,
		System2FanSpeed,
		GPU2FanSpeed,
		GPU1Temperature,
		GPU2Temperature
	};
	constexpr size_t SystemHealthInformationIndexCount = static_cast<size_t>(SystemHealthInformationIndex::GPU2Temperature);

	struct GetGamingSysInfoInput
	{
		uint32_t unk : 8 = 1;
		SystemHealthInformationIndex index : 24;

		constexpr GetGamingSysInfoInput(SystemHealthInformationIndex i) noexcept { index = i; }
		constexpr operator uint32_t() const { return std::bit_cast<uint32_t>(*this); }
	};
	static_assert(sizeof(GetGamingSysInfoInput) == sizeof(uint32_t));
}
