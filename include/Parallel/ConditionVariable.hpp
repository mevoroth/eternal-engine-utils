#pragma once

namespace Eternal
{
	namespace Time
	{
		using TimeNanoSecondsT	= uint64_t;
	}
	namespace Parallel
	{
		using namespace Eternal::Time;
		class Mutex;

		class ConditionVariable
		{
		public:
			virtual ~ConditionVariable() {}
			virtual void NotifyAll() = 0;
			virtual void NotifyOne() = 0;
			virtual void WaitFor(_In_ Mutex& InMutex, _In_ TimeNanoSecondsT InNanoSeconds) = 0;
			virtual void Wait(_In_ Mutex& InMutex) = 0;
		};
	}
}
