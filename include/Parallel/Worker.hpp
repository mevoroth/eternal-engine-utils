#ifndef _WORKER_HPP_
#define _WORKER_HPP_

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

			bool PushTask(Task* TaskObj);
			void Shutdown();
			bool IsAvailable() const;

		private:
			struct WorkerArgs
			{
				AtomicS32*& Running;
				ConditionVariable*& WorkerConditionVariable;
				Mutex*& WorkerConditionVariableMutex;
				Task*& TaskObj;
				AtomicS32*& IsExecutingTask;
				AtomicS32*& IsAvailable;
				AtomicS32*& IsPushing;

				WorkerArgs(AtomicS32*& Running, ConditionVariable*& WorkerConditionVariable, Mutex*& WorkerConditionVariableMutex, Task*& TaskObj, AtomicS32*& IsExecutingTask, AtomicS32*& IsAvailable, AtomicS32*& IsPushing)
					: Running(Running)
					, WorkerConditionVariable(WorkerConditionVariable)
					, WorkerConditionVariableMutex(WorkerConditionVariableMutex)
					, TaskObj(TaskObj)
					, IsExecutingTask(IsExecutingTask)
					, IsAvailable(IsAvailable)
					, IsPushing(IsPushing)
				{
				}
			};

			WorkerArgs* _WorkerArgs = nullptr;
			AtomicS32* _Running = nullptr;
			AtomicS32* _Pushing = nullptr;
			Task* _Task = nullptr;

			ConditionVariable* _WorkerConditionVariable = nullptr;
			Mutex* _WorkerConditionVariableMutex = nullptr;
			AtomicS32* _IsExecutingTask = nullptr;
			AtomicS32* _IsAvailable = nullptr;
		};
	}
}

#endif
