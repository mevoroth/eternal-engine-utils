#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include <cstdint>
#include "Task.hpp"

namespace Eternal
{
	namespace Parallel
	{
		class ConditionVariable;
		class Mutex;
		class Thread;

		class Worker : public Task
		{
		public:
			static uint32_t WorkerRun(void* Args);

			Worker(Thread* ThreadObj);
			~Worker();

			virtual bool TaskIsExecuted() override;
			virtual void Setup() override;
			virtual void Execute() override;

			void SetTask(Task* TaskObj);
			Task* GetTask()
			{
				return _Task;
			}

		private:
			Thread* _Thread = nullptr;
			
			ConditionVariable* _ConditionVariable = nullptr;
			Mutex* _ConditionVariableMutex = nullptr;

			Task* _Task = nullptr;
			Mutex* _TaskMutex = nullptr;

			struct WorkerArguments
			{
				Worker* WorkerObj;
			};
		};
	}
}

#endif
