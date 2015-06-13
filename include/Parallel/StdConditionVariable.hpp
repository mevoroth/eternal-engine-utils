#ifndef _STD_CONDITION_VARIABLE_HPP_
#define _STD_CONDITION_VARIABLE_HPP_

#include "ConditionVariable.hpp"

#include <condition_variable>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class StdConditionVariable : public ConditionVariable
		{
		public:
			virtual void NotifyAll() override;
			virtual void NotifyOne() override;
			virtual void WaitFor(Mutex& MutexObj, TimeT Milliseconds) override;
			virtual void Wait(Mutex& MutexObj) override;

		private:
			condition_variable_any _ConditionVariable;
		};
	}
}

#endif
