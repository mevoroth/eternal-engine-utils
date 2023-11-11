#include "Time/Timer.hpp"

#include <cstdio>

namespace Eternal
{
	namespace Time
	{
		Timer* Timer::_Instance = nullptr;

		Timer::Timer()
		{
			ETERNAL_ASSERT(!_Instance);
			_Instance = this;
		}

		Timer* Timer::Get()
		{
			ETERNAL_ASSERT(_Instance);
			return _Instance;
		}

		void Timer::ToHumanReadable(_In_ const TimeMicroSecondsT& Value, _Out_ char HumanReadable[24])
		{
			uint32_t MicroSeconds = 0;
			uint32_t MilliSeconds = 0;
			uint32_t Seconds = 0;
			uint32_t Minutes = 0;
			uint32_t Hours = 0;
			uint32_t Days = 0;

			TimeMicroSecondsT TempValue = Value;
			MicroSeconds = TempValue % 1000;
			TempValue /= 1000;
			MilliSeconds = TempValue % 1000;
			TempValue /= 1000;
			Seconds = TempValue % 60;
			TempValue /= 60;
			Minutes = TempValue % 60;
			TempValue /= 60;
			Hours = TempValue % 24;
			TempValue /= 24;
			Days = (uint32_t)TempValue;

			sprintf_s(HumanReadable, 24, "[%02dd%02dh%02dm%02ds%04d.%04d]", Days, Hours, Minutes, Seconds, MilliSeconds, MicroSeconds);
		}
	}
}
