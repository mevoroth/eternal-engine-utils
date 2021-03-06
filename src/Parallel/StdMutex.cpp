#include "Parallel/StdMutex.hpp"
#include "Log/Log.hpp"

using namespace Eternal::Parallel;

bool StdMutex::TryLock()
{
	return _Mutex.try_lock();
}

void StdMutex::Lock()
{
	_Mutex.lock();
}

void StdMutex::Unlock()
{
	_Mutex.unlock();
}
