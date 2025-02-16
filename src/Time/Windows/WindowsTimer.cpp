#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Time/Windows/WindowsTimer.hpp"

#include <Windows.h>

namespace Eternal
{
	namespace Time
	{
		WindowsTimer::WindowsTimer()
			: Timer()
		{
			if (!QueryPerformanceFrequency(&_Frequency))
			{
				memset(&_Frequency, 0x0, sizeof(LARGE_INTEGER));
			}
			ETERNAL_ASSERT(_Frequency.QuadPart != 0);

			_PreviousTimeMicroSeconds = GetTimeMicroSeconds();
		}

		void WindowsTimer::Begin()
		{
			ETERNAL_ASSERT(!_TimerStack.Full());

			LARGE_INTEGER Value;
			if (QueryPerformanceCounter(&Value))
			{
				_TimerStack.Push(Value);
			}
		}

		TimeMicroSecondsT WindowsTimer::End()
		{
			ETERNAL_ASSERT(!_TimerStack.Empty());

			LARGE_INTEGER Value;
			if (QueryPerformanceCounter(&Value))
			{
				Value.QuadPart = Value.QuadPart - _TimerStack.Head().QuadPart;
				_TimerStack.Pop();
				return Value.QuadPart * 1000000ull / _Frequency.QuadPart;
			}

			return ~0ull;
		}

		TimeMicroSecondsT WindowsTimer::GetTimeMicroSeconds() const
		{
			LARGE_INTEGER Value;
			if (QueryPerformanceCounter(&Value))
			{
				return Value.QuadPart * 1000000ull / _Frequency.QuadPart;
			}
			return ~0ull;
		}

		void WindowsTimer::Update()
		{
			TimeMicroSecondsT PreviousTimeMicroSeconds = _PreviousTimeMicroSeconds;
#if ETERNAL_DEBUG
			static constexpr double TimerThreshold = 333000;
			TimeMicroSecondsT DeltaTimeMicroSeconds = GetTimeMicroSeconds() - PreviousTimeMicroSeconds;
			if (DeltaTimeMicroSeconds > TimerThreshold && IsDebuggerPresent())
				PreviousTimeMicroSeconds = GetTimeMicroSeconds();
#endif
			_PreviousTimeMicroSeconds = GetTimeMicroSeconds();
			_DeltaTimeMicroSeconds = _PreviousTimeMicroSeconds - PreviousTimeMicroSeconds;
			_DeltaTimeSeconds = (double)_DeltaTimeMicroSeconds / 1000000.0;
		}
	}
}

#endif
