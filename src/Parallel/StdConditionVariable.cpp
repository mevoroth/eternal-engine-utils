#include "Parallel/StdConditionVariable.hpp"

#include <chrono>

#include "Parallel/StdMutex.hpp"

using namespace std;
using namespace Eternal::Parallel;

void StdConditionVariable::NotifyAll()
{
	_ConditionVariable.notify_all();
}

void StdConditionVariable::NotifyOne()
{
	_ConditionVariable.notify_one();
}

void StdConditionVariable::WaitFor(Mutex& MutexObj, TimeNanoSecondsT NanoSeconds)
{
	_ConditionVariable.wait_for(static_cast<StdMutex&>(MutexObj)._Mutex, chrono::nanoseconds(NanoSeconds));
}

void StdConditionVariable::Wait(Mutex& MutexObj)
{
	_ConditionVariable.wait(static_cast<StdMutex&>(MutexObj)._Mutex);
}
