#include "Parallel/StdThread.hpp"

namespace Eternal
{
	namespace Parallel
	{
		void StdThread::Create(_In_ ThreadFunction InFunction, _In_ void* InFunctionParameters)
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
