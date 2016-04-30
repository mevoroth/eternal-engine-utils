#ifndef _TASK_SCHEDULER_HPP_
#define _TASK_SCHEDULER_HPP_

#include <vector>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class Task;
		class Mutex;
		class AtomicInt;

		class TaskScheduler
		{
		public:
			TaskScheduler();
			virtual ~TaskScheduler();
			
			void PushTask(Task* ChildTask, Task* ParentTask = nullptr);
			Task* PopTask();
			int RemainingUnscheduledTasks() const;
			bool Done() const;
			void Reset();

		private:
			struct Dependency
			{
				Task* TaskObj = nullptr;
				vector<Task*> Dependencies;
			};

			Mutex* _TasksMutex = nullptr;
			vector<Dependency> _TasksList;
			AtomicInt* _UnscheduledTasks = nullptr;

			bool _IsSchedulable(const vector<Task*>& SubTasks) const;
		};
	}
}

#endif
