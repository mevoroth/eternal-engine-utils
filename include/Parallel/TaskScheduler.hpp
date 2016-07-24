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
		class AtomicS32;

		class TaskScheduler
		{
		public:
			TaskScheduler();
			TaskScheduler(const TaskScheduler&) = delete;
			TaskScheduler& operator=(const TaskScheduler&) = delete;
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
			AtomicS32* _UnscheduledTasks = nullptr;

			bool _IsSchedulable(const vector<Task*>& SubTasks) const;
		};
	}
}

#endif
