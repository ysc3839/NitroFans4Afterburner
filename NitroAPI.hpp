#pragma once

#include "AcerWMI.hpp"
#include "Util.hpp"

namespace NitroAPI
{
	namespace
	{
		constexpr auto PipeName = LR"(\\.\pipe\PredatorSense_service_namedpipe)";

		wil::unique_hfile s_hPipe;

		void ConnectPipe()
		{
			if (!s_hPipe)
			{
				THROW_IF_WIN32_BOOL_FALSE(WaitNamedPipeW(PipeName, NMPWAIT_WAIT_FOREVER));
				s_hPipe.reset(CreateFileW(PipeName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr));
				THROW_LAST_ERROR_IF(!s_hPipe);
			}
		}

#pragma pack(push, 1)
		template <size_t count>
		struct Command
		{
			uint16_t cmdCode;
			uint8_t argsCount = count;
			struct Arg {
				uint32_t size;
				uint32_t data;
			} args[count];
		};
		static_assert(sizeof(Command<1>) == 11);
#pragma pack(pop)

		uint64_t GetAcerGamingSystemInformation(uint32_t data)
		{
			ConnectPipe();

			Command<1> cmd = {
				.cmdCode = 13,
				.args = { {
					.size = sizeof(uint32_t),
					.data = data
				} }
			};
			WriteFileCheckSize(s_hPipe.get(), &cmd, sizeof(cmd));

			THROW_IF_WIN32_BOOL_FALSE(FlushFileBuffers(s_hPipe.get()));

			uint8_t buf[13];
			ReadFileCheckSize(s_hPipe.get(), buf, sizeof(buf));

			return *reinterpret_cast<uint64_t*>(buf + 5);
		}
	}

	uint16_t GetWMISystemHealthInfo(AcerWMI::SystemHealthInformationIndex index)
	{
		const auto result = GetAcerGamingSystemInformation(AcerWMI::GetGamingSysInfoInput(index));
		THROW_HR_IF(E_FAIL, (result & 0xff) != 0);
		return static_cast<uint16_t>(result >> 8);
	}

	void Cleanup()
	{
		s_hPipe.reset();
	}
}
