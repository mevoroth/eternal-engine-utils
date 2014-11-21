#ifndef _WIN_TIME_HPP_
#define _WIN_TIME_HPP_

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
			static const int STACKSIZE = 50;
		private:
			LARGE_INTEGER _freq;
			Stack<LARGE_INTEGER, STACKSIZE> _stack;
		public:
			WinTime();
			virtual void Begin() override;
			virtual TimeT End() override;
			virtual TimeT GetTime() const override;
		};
	}
}

#endif
