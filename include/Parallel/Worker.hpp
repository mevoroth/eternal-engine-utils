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
			virtual void DoTask() override;
			bool TaskIsFinished() const;
			void SetTask(Task* TaskObj);
			inline Task* GetTask()
			{
				return _CurrentTask;
			}

		private:
			Thread* _Thread = nullptr;
			Task* _CurrentTask = nullptr;
			ConditionVariable* _ConditionVariable = nullptr;
			Mutex* _ConditionVariableMutex = nullptr;

			struct WorkerArguments
			{
				Worker* WorkerObj;
			};
		};
	}
}

#endif
