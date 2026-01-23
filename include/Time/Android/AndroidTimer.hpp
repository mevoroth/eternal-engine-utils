#pragma once

#if ETERNAL_PLATFORM_ANDROID

#include "Time/Timer.hpp"

namespace Eternal
{
	namespace Time
	{
		class AndroidTimer final : public Timer
		{
		public:

			AndroidTimer();
			virtual void Begin() override final;
			virtual TimeMicroSecondsT End() override final;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const override final;

		};
	}
}

#endif
