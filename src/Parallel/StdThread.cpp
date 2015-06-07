#include "Parallel/StdThread.hpp"

#include <chrono>

using namespace Eternal::Parallel;

void StdThread::Create(ThreadFunction Function, void* FunctionParams)
{
	_Thread = thread(Function, FunctionParams);
}

void StdThread::Sleep(TimeT Milliseconds)
{
	this_thread::sleep_for(chrono::milliseconds(Milliseconds));
}
