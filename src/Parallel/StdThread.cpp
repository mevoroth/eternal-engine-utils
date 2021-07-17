#include "Parallel/StdThread.hpp"

namespace Eternal
{
	namespace Parallel
	{
		void StdThread::Create(ThreadFunction InFunction, void* InFunctionParameters)
		{
			_Thread = thread(InFunction, InFunctionParameters);
		}

		StdThread::~StdThread()
		{
			if (_Thread.joinable())
				_Thread.join();
		}
	}
}
