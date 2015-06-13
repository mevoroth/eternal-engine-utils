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

void StdConditionVariable::WaitFor(Mutex& MutexObj, TimeT Milliseconds)
{
	_ConditionVariable.wait_for(static_cast<StdMutex&>(MutexObj)._Mutex, chrono::milliseconds(Milliseconds));
}

void StdConditionVariable::Wait(Mutex& MutexObj)
{
	_ConditionVariable.wait(static_cast<StdMutex&>(MutexObj)._Mutex);
}
