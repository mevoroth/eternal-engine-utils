#include "Parallel/StdConditionVariable.hpp"

#include <chrono>
#include "Parallel/StdMutex.hpp"

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		void StdConditionVariable::NotifyAll()
		{
			_ConditionVariable.notify_all();
		}

		void StdConditionVariable::NotifyOne()
		{
			_ConditionVariable.notify_one();
		}

		void StdConditionVariable::WaitFor(_In_ Mutex& InMutex, _In_ TimeNanoSecondsT InNanoSeconds)
		{
			_ConditionVariable.wait_for(static_cast<StdMutex&>(InMutex)._Mutex, chrono::nanoseconds(InNanoSeconds));
		}

		void StdConditionVariable::Wait(_In_ Mutex& InMutex)
		{
			_ConditionVariable.wait(static_cast<StdMutex&>(InMutex)._Mutex);
		}
	}
}
