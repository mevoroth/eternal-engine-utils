#if ETERNAL_PLATFORM_ANDROID

#include "Time/Android/AndroidTimer.hpp"

#include <time.h>

namespace Eternal
{
	namespace Time
	{
		AndroidTimer::AndroidTimer()
		{
			_Frequency					= 1000000000ull;
			_PreviousTimeMicroSeconds	= GetTimeMicroSeconds();
		}

		void AndroidTimer::Begin()
		{
			ETERNAL_ASSERT(!_TimerStack.Full());
			timespec NewTimeSpec = {};
			clock_gettime(CLOCK_MONOTONIC, &NewTimeSpec);
			_TimerStack.Push(NewTimeSpec);
		}

		TimeMicroSecondsT AndroidTimer::End()
		{
			ETERNAL_ASSERT(!_TimerStack.Empty());
			timespec CurrentTimeSpec = {};
			clock_gettime(CLOCK_MONOTONIC, &CurrentTimeSpec);

			uint64_t ElapsedTimeNanoSeconds = CurrentTimeSpec.tv_sec * _Frequency + CurrentTimeSpec.tv_nsec
				- _TimerStack.Head().tv_sec * _Frequency + _TimerStack.Head().tv_nsec;

			return static_cast<TimeMicroSecondsT>(static_cast<double>(ElapsedTimeNanoSeconds) * NanoSecondsToMicroSeconds);
		}

		TimeMicroSecondsT AndroidTimer::GetTimeMicroSeconds() const
		{
			timespec CurrentTimeSpec = {};
			clock_gettime(CLOCK_MONOTONIC, &CurrentTimeSpec);
			uint64_t CurrentTimeNanoSeconds = CurrentTimeSpec.tv_sec * _Frequency + CurrentTimeSpec.tv_nsec;
			return static_cast<TimeMicroSecondsT>(static_cast<double>(CurrentTimeNanoSeconds) * NanoSecondsToMicroSeconds);
		}
	}
}

#endif
