#include "Parallel/StdThread.hpp"

using namespace Eternal::Parallel;

void StdThread::Create(ThreadFunction Function, void* FunctionParams)
{
	_Thread = thread(Function, FunctionParams);
}
