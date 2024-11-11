#pragma once

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include <Windows.h>

#include "Time/Timer.hpp"
#include "Container/Stack.hpp"

namespace Eternal
{
	namespace Time
	{
		using namespace Eternal::Container;

		class WindowsTimer final : public Timer
		{
		public:

			WindowsTimer();
			virtual void Begin() override final;
			virtual TimeMicroSecondsT End() override final;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const override final;
			virtual void Update() override;

		private:
			LARGE_INTEGER							_Frequency;
			Stack<LARGE_INTEGER, TIMER_STACK_SIZE>	_TimerStack;
		};
	}
}

#endif
