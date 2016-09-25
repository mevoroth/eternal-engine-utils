#ifndef _WIN_TIME_HPP_
#define _WIN_TIME_HPP_

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include "Time/Time.hpp"
#include "Container/Stack.hpp"

using namespace Eternal::Container;

namespace Eternal
{
	namespace Time
	{
		class WinTime : public Time
		{
		public:
			static const int STACKSIZE = 1024;
		private:
			LARGE_INTEGER _Freq;
			Stack<LARGE_INTEGER, STACKSIZE> _Stack;
			TimeMicroSecondsT _PreviousTimeMicroSeconds = 0ull;
			TimeMicroSecondsT _DeltaTimeMicroSeconds = 0ull;
			TimeSecondsT _DeltaTimeSeconds = 0.0;
		public:
			WinTime();
			virtual void Begin() override;
			virtual TimeMicroSecondsT End() override;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const override;
			virtual TimeMicroSecondsT GetDeltaTimeMicroSeconds() const override;
			virtual TimeSecondsT GetDeltaTimeSeconds() const override;
			virtual void Update() override;
		};
	}
}

#endif
