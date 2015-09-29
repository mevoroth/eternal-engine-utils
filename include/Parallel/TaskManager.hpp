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
		class Scheduler;

		class TaskManager
		{
		public:
			static TaskManager* Get();
			
			static uint32_t TaskRun(_In_ void* Args);
			static uint32_t TaskClean(_In_ void* Args);

			TaskManager();
			~TaskManager();
			void Push(_In_ Task* TaskObj, _In_ Task* DependsOn = nullptr);
			void Barrier();

		private:
			static TaskManager* _Inst;

			Thread* _TaskManager = nullptr;
			Worker* _Workers[TASK_MANAGER_WORKERS_COUNT];
			Thread* _WorkersThreads[TASK_MANAGER_WORKERS_COUNT];
			Mutex* _TasksListMutex = nullptr;
			Scheduler* _TasksList = nullptr;
			ConditionVariable* _SchedulerConditionVariable = nullptr;
			Mutex* _SchedulerConditionVariableMutex = nullptr;

			Thread* _CleanTasks = nullptr;
			list<Task*> _TasksToClean;
			Mutex* _TasksToCleanMutex = nullptr;

			struct TaskManagerArguments
			{
				Worker** Workers;
				Scheduler* TasksList;
				ConditionVariable* SchedulerConditionVariable;
				Mutex* SchedulerConditionVariableMutex;
				list<Task*>* TasksToClean;
				Mutex* TasksToCleanMutex;
			};

			struct CleanTasksArguments
			{
				list<Task*>* TasksList;
				Mutex* TasksListMutex;
			};
		};
	}
}

#endif
