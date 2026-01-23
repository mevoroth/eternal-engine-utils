#pragma once

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Time/Timer.hpp"

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
		};
	}
}

#endif
