#pragma once

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
			virtual void WaitFor(_In_ Mutex& InMutex, _In_ TimeNanoSecondsT InNanoSeconds) override;
			virtual void Wait(_In_ Mutex& InMutex) override;

		private:
			condition_variable_any _ConditionVariable;
		};
	}
}
