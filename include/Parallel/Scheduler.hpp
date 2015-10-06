#ifndef _SCHEDULER_HPP_
#define _SCHEDULER_HPP_

#include <list>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class Task;
		class Mutex;

		class Scheduler
		{
		public:
			Scheduler(_In_ Mutex* MutexObj);
			void Push(_In_ Task* TaskObj, _In_ Task* TaskDependency = nullptr);
			Task* Pop();
			size_t GetRemainingTasks() const;

		private:
			struct SchedulerNode
			{
				Task* TaskObj;
				Task* DependsOn = nullptr;
				bool operator==(const SchedulerNode& Node)
				{
					return TaskObj == Node.TaskObj && DependsOn == Node.DependsOn;
				}
			};
			list<SchedulerNode> _TasksList;
			Mutex* _MutexObj = nullptr;
		};
	}
}

#endif
