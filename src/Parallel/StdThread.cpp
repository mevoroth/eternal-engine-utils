#include "Parallel/StdThread.hpp"

using namespace Eternal::Parallel;

void StdThread::Create(ThreadFunction InFunction, void* InFunctionParameters)
{
	_Thread = thread(InFunction, InFunctionParameters);
}
