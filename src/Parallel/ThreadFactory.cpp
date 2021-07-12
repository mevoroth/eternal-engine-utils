#include "Parallel/ThreadFactory.hpp"
#include "Parallel/StdThread.hpp"

namespace Eternal
{
	namespace Parallel
	{
		Thread* CreateThread(_In_ const string& InName, _In_ ThreadFunction InFunction, _In_ void* InFunctionParameters)
		{
			Thread* NewThread = new StdThread(InName);
			NewThread->Create(InFunction, InFunctionParameters);
			return NewThread;
		}

		void DestroyThread(_Inout_ Thread*& InOutThread)
		{
			delete InOutThread;
			InOutThread = nullptr;
		}
	}
}
