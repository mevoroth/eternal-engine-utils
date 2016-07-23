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
			static const int STACKSIZE = 64;
		private:
			LARGE_INTEGER _Freq;
			Stack<LARGE_INTEGER, STACKSIZE> _Stack;
			TimeT _PreviousTime = 0ull;
			TimeT _DeltaTime = 0ull;
		public:
			WinTime();
			virtual void Begin() override;
			virtual TimeT End() override;
			virtual TimeT GetTime() const override;
			virtual TimeT GetDeltaTime() const override;
			virtual void Update() override;
		};
	}
}

#endif
