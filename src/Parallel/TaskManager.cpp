#include "Parallel/TaskManager.hpp"

#include "Parallel/StdThread.hpp"
#include "Parallel/Worker.hpp"
#include "Parallel/CpuCoreCount.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/StdAtomicS32.hpp"
#include "Parallel/Sleep.hpp"
#include "Parallel/Task.hpp"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

using namespace std;
using namespace Eternal::Parallel;

TaskManager* TaskManager::_Inst = nullptr;

uint32_t TaskManager::TaskManagerRun(void* Args)
{
	TaskManagerArgs* Arguments = (TaskManagerArgs*)Args;

	while (Arguments->Running->Load())
	{
		Arguments->SchedulerConditionVariableMutex->Lock();
		Arguments->SchedulerConditionVariable->Wait(*Arguments->SchedulerConditionVariableMutex);
		Arguments->SchedulerConditionVariableMutex->Unlock();

		Arguments->SchedulingTask = true;

		while (Arguments->Scheduler.RemainingUnscheduledTasks())
		{
			Task* TaskObj = nullptr;
			while (!TaskObj)
			{
				Sleep(1);
				TaskObj = Arguments->Scheduler.PopTask();
			}

#ifdef ETERNAL_DEBUG
			OutputDebugString("[TaskManager::TaskManagerRun]Picking ");
			OutputDebugString(TaskObj->GetTaskName().c_str());
			OutputDebugString("\n");

			char UnscheduledTasksCount[256];
			sprintf_s(UnscheduledTasksCount, "[TaskManager::TaskManagerRun]Remaining tasks after picking : %d\n", Arguments->Scheduler.RemainingUnscheduledTasks());
			OutputDebugString(UnscheduledTasksCount);
#endif

			bool TaskToBeScheduled = true;
			while (TaskToBeScheduled)
			{
				for (uint32_t WorkerIndex = 0; TaskToBeScheduled && WorkerIndex < Arguments->WorkersCount; ++WorkerIndex)
				{
					if (Arguments->Workers[WorkerIndex]->IsAvailable())
					{
						Arguments->Workers[WorkerIndex]->PushTask(TaskObj);
						TaskToBeScheduled = false;
					}
				}
				Sleep(1);
			}
		}

		Arguments->Done = true;
	}

	return 0;
}

TaskManager::TaskManager()
{
	ETERNAL_ASSERT(!_Inst);

	_Running = new StdAtomicS32(1);

	_ThreadsWorkersCount = CpuCoreCount();
	_Threads = new Thread*[_ThreadsWorkersCount];
	_Workers = new Worker*[_ThreadsWorkersCount];
	for (int ThreadWorkerIndex = 0; ThreadWorkerIndex < _ThreadsWorkersCount; ++ThreadWorkerIndex)
	{
		_Threads[ThreadWorkerIndex] = new StdThread();
		_Workers[ThreadWorkerIndex] = new Worker(_Threads[ThreadWorkerIndex]);
	}

	_SchedulerConditionVariableMutex = new StdMutex();
	_SchedulerConditionVariable = new StdConditionVariable();

	_TaskManagerArgs = new TaskManagerArgs(_Running, _Workers, _ThreadsWorkersCount, _Scheduler, _SchedulerConditionVariable, _SchedulerConditionVariableMutex, _SchedulingTask, _Done);

	_TaskManagerThread = new StdThread();
	_TaskManagerThread->Create(TaskManager::TaskManagerRun, _TaskManagerArgs);

	_Inst = this;
}

TaskManager::~TaskManager()
{
	_Inst = nullptr;
}

TaskManager* TaskManager::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

void TaskManager::Schedule()
{
	OutputDebugString("TaskManager::Schedule\n");
	_Done = false;
	_SchedulingTask = false;

	while (!_SchedulingTask)
	{
		_SchedulerConditionVariableMutex->Lock();
		_SchedulerConditionVariable->NotifyOne();
		_SchedulerConditionVariableMutex->Unlock();
		
		Sleep(1);
	}
}

void TaskManager::Barrier()
{
	OutputDebugString("TaskManager::Barrier [BEGIN]\n");
	while (!(_Done && _Scheduler.Done()))
		Sleep(1);
	OutputDebugString("TaskManager::Barrier [END]\n");
}

void TaskManager::Shutdown()
{
	ETERNAL_ASSERT(_Running->Load());
	_Running->Store(0);
}
