#ifndef _CONDITION_VARIABLE_HPP_
#define _CONDITION_VARIABLE_HPP_

#include "Time/Time.hpp"

namespace Eternal
{
	namespace Parallel
	{
		using namespace Eternal::Time;
		class Mutex;

		class ConditionVariable
		{
		public:
			virtual void NotifyAll() = 0;
			virtual void NotifyOne() = 0;
			virtual void WaitFor(Mutex& MutexObj, TimeT Milliseconds) = 0;
			virtual void Wait(Mutex& MutexObj) = 0;
		};
	}
}

#endif
