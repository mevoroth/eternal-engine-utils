#ifndef _SLEEP_HPP_
#define _SLEEP_HPP_

#include "Time/Time.hpp"

namespace Eternal
{
	namespace Parallel
	{
		using namespace Eternal::Time;

		extern void Sleep(const TimeT& NanoSeconds);
	}
}

#endif
