#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include "Task.hpp"

namespace Eternal
{
	namespace Parallel
	{
		class ConditionVariable;
		class Mutex;

		class Worker : public Task
		{
		public:
			Worker();
			virtual void DoTask() override;
			bool TaskIsFinished() const;
			void SetTask(Task* TaskObj);

		private:
			Task* _CurrentTask = nullptr;
			ConditionVariable* _ConditionVariable = nullptr;
			Mutex* _ConditionVariableMutex = nullptr;
		};
	}
}

#endif
