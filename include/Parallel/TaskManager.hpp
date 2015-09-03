#ifndef _TASK_MANAGER_HPP_
#define _TASK_MANAGER_HPP_

#include <cstdint>
#include <list>

#define TASK_MANAGER_WORKERS_COUNT 8

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class Worker;
		class Task;
		class Mutex;
		class Thread;
		class ConditionVariable;

		class TaskManager
		{
		public:
			static TaskManager* Get();
			
			static uint32_t WorkerRun(void* Args);
			static uint32_t TaskRun(void* Args);

			TaskManager();
			~TaskManager();
			void Push(Task* TaskObj);

		private:
			static TaskManager* _Inst;

			Thread* _Scheduler;
			Worker* _Workers[TASK_MANAGER_WORKERS_COUNT];
			Thread* _WorkersThreads[TASK_MANAGER_WORKERS_COUNT];
			Mutex* _TasksListMutex;
			list<Task*> _TasksList;
			ConditionVariable* _TaskRunConditionVariable;
			Mutex* _TaskRunConditionVariableMutex;
		};
	}
}

#endif
