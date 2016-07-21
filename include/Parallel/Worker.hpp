#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Parallel
	{
		class Task;
		class Thread;
		class AtomicS32;
		class Mutex;
		class ConditionVariable;

		class Worker
		{
		public:
			static uint32_t WorkerRun(void* Args);

			Worker(Thread* ThreadObj);
			~Worker();

			void PushTask(Task* TaskObj);
			void Shutdown();
			bool IsAvailable() const;

		private:
			struct WorkerArgs
			{
				AtomicS32*& Running;
				ConditionVariable*& WorkerConditionVariable;
				Mutex*& WorkerConditionVariableMutex;
				Task*& TaskObj;
				bool& IsExecutingTask;
				bool& IsAvailable;

				WorkerArgs(AtomicS32*& Running, ConditionVariable*& WorkerConditionVariable, Mutex*& WorkerConditionVariableMutex, Task*& TaskObj, bool& IsExecutingTask, bool& IsAvailable)
					: Running(Running)
					, WorkerConditionVariable(WorkerConditionVariable)
					, WorkerConditionVariableMutex(WorkerConditionVariableMutex)
					, TaskObj(TaskObj)
					, IsExecutingTask(IsExecutingTask)
					, IsAvailable(IsAvailable)
				{
				}
			};

			WorkerArgs* _WorkerArgs = nullptr;
			AtomicS32* _Running = nullptr;
			Task* _Task = nullptr;

			ConditionVariable* _WorkerConditionVariable = nullptr;
			Mutex* _WorkerConditionVariableMutex = nullptr;
			bool _IsExecutingTask = false;
			bool _IsAvailable = true;
		};
	}
}

#endif
