#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Time/Windows/WindowsTimer.hpp"

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
	}
}

#endif
