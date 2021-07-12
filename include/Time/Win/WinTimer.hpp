#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include "Time/Timer.hpp"
#include "Container/Stack.hpp"


namespace Eternal
{
	namespace Time
	{
		using namespace Eternal::Container;

		class WinTimer final : public Timer
		{
		public:
			static const int STACKSIZE = 1024;
		public:
			WinTimer();
			virtual void Begin() override final;
			virtual TimeMicroSecondsT End() override final;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const override final;
			virtual TimeMicroSecondsT GetDeltaTimeMicroSeconds() const override final;
			virtual TimeSecondsT GetDeltaTimeSeconds() const override final;
			virtual void Update() override;

		private:
			LARGE_INTEGER					_Freq;
			Stack<LARGE_INTEGER, STACKSIZE>	_Stack;
			TimeMicroSecondsT				_PreviousTimeMicroSeconds = 0ull;
			TimeMicroSecondsT				_DeltaTimeMicroSeconds = 0ull;
			TimeSecondsT					_DeltaTimeSeconds = 0.0;
		};
	}
}
