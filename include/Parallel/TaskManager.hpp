#ifndef _TASK_MANAGER_HPP_
#define _TASK_MANAGER_HPP_

#include <cstdint>

#include "Macros/Macros.hpp"
#include "TaskScheduler.hpp"

#include <cstdint>

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
		class AtomicS32;

		class TaskManager
		{
		public:
			static uint32_t TaskManagerRun(void* Args);
			static TaskManager* Get();
			TaskManager();
			virtual ~TaskManager();

			inline TaskScheduler& GetTaskScheduler()
			{
				ETERNAL_ASSERT(_Done);
				return _Scheduler;
			}

			void Schedule();
			void Barrier();
			void Shutdown();

		private:
			struct TaskManagerArgs
			{
				AtomicS32*& Running;
				Worker**& Workers;
				const uint32_t& WorkersCount;
				TaskScheduler& Scheduler;
				ConditionVariable*& SchedulerConditionVariable;
				Mutex*& SchedulerConditionVariableMutex;
				bool& SchedulingTask;
				bool& Done;
				
				TaskManagerArgs(AtomicS32*& Running, Worker**& Workers, const uint32_t& WorkersCount, TaskScheduler& Scheduler, ConditionVariable*& SchedulerConditionVariable, Mutex*& SchedulerConditionVariableMutex, bool& SchedulingTask, bool& Done)
					: Running(Running)
					, Workers(Workers)
					, WorkersCount(WorkersCount)
					, Scheduler(Scheduler)
					, SchedulerConditionVariable(SchedulerConditionVariable)
					, SchedulerConditionVariableMutex(SchedulerConditionVariableMutex)
					, SchedulingTask(SchedulingTask)
					, Done(Done)
				{
				}
			};
			static TaskManager* _Inst;

			Thread* _TaskManagerThread = nullptr;

			AtomicS32* _Running = nullptr;
			TaskManagerArgs* _TaskManagerArgs = nullptr;
			Thread** _Threads = nullptr;
			Worker** _Workers = nullptr;
			uint32_t _ThreadsWorkersCount = 0;
			TaskScheduler _Scheduler;
			ConditionVariable* _SchedulerConditionVariable = nullptr;
			Mutex* _SchedulerConditionVariableMutex = nullptr;
			bool _SchedulingTask = false;
			bool _Done = true;
		};
	}
}

#endif
