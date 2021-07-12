#pragma once

#include <string>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class Task;
		class Thread;
		class AtomicS32;
		class Mutex;
		class ConditionVariable;

		using ThreadFunction = uint32_t(*)(void*);

		class Worker
		{
		public:
			static uint32_t WorkerRunGeneric(_In_ void* InArguments);
			static uint32_t WorkerRunDedicated(_In_ void* InArguments);

			Worker(_In_ const string& InName, _In_ ThreadFunction InFunction = nullptr);
			~Worker();

			void Shutdown();
			bool IsAvailable() const;
			bool EnqueueTask(_In_ Task* InTask);

		private:
			struct WorkerArguments
			{
				WorkerArguments();
				~WorkerArguments();

				Task* WorkerTask								= nullptr;
				AtomicS32* WorkerRunning						= nullptr;
				AtomicS32* WorkerExecuting						= nullptr;
				Mutex* WorkerConditionVariableMutex				= nullptr;
				ConditionVariable* WorkerConditionVariable		= nullptr;
				string Name;
			};

			WorkerArguments	_WorkerArguments;
			Thread*			_Thread = nullptr;
		};
	}
}
