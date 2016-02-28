#include "Parallel/TaskManager.hpp"

#include <algorithm>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#include "Macros/Macros.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/StdThread.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Worker.hpp"
#include "Parallel/MutexAutoLock.hpp"
//#include "Parallel/Scheduler.hpp"
#include "Parallel/Sleep.hpp"

using namespace std;
using namespace Eternal::Parallel;

TaskManager* TaskManager::_Inst = nullptr;

uint32_t TaskManager::TaskRun(_In_ void* Args)
{
	ETERNAL_ASSERT(Args);
	TaskManagerArguments& TaskManagerArgs = *(TaskManagerArguments*)Args;

	for (;;)
	{
		for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
		{
			Worker* CurrentWorker = TaskManagerArgs.Workers[WorkerIndex];
			if (!CurrentWorker->TaskIsExecuted())
			{
				OutputDebugString("TASK IS NOT EXECUTED\n");
				continue;
			}

			{
				TaskManagerArgs.ExecutingTasksMutex->Lock();
				TaskManagerArgs.ExecutingTasks->remove(CurrentWorker->GetTask());
				TaskManagerArgs.ExecutingTasksMutex->Unlock();
				
				Task* NewTask = nullptr;

				TaskManagerArgs.NewTasksMutex->Lock();
				for (list<Task*>::iterator It = TaskManagerArgs.NewTasks->begin(), End = TaskManagerArgs.NewTasks->end(); It != End; ++It)
				{
					if ((*It)->TaskIsExecutable())
					{
						{
							MutexAutoLock ExecutingMutex(TaskManagerArgs.ExecutingTasksMutex);
							TaskManagerArgs.ExecutingTasks->push_back((*It));
						}
						NewTask = *It;
						TaskManagerArgs.NewTasks->erase(It);
						break;
					}
				}
				TaskManagerArgs.NewTasksMutex->Unlock();

				if (NewTask)
					CurrentWorker->SetTask(NewTask);
			}
		}

		Sleep(1);
	}

	//for (;;)
	//{
	//	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	//	{
	//		if (!TaskManagerArgs.Workers[WorkerIndex]->TaskIsFinished())
	//		{
	//			continue;
	//		}

	//		{
	//			MutexAutoLock CleanMutex(TaskManagerArgs.TasksToCleanMutex);
	//			Task* CurrentTaskInWorker = TaskManagerArgs.Workers[WorkerIndex]->GetTask();
	//			if (CurrentTaskInWorker)
	//				TaskManagerArgs.TasksToClean->push_back(CurrentTaskInWorker);
	//		}

	//		Task* NewTask = nullptr;
	//		while (!NewTask)
	//		{
	//			NewTask = TaskManagerArgs.TasksList->Pop();
	//			if (!NewTask)
	//				Sleep(1);
	//		}

	//		TaskManagerArgs.Workers[WorkerIndex]->SetTask(NewTask);
	//	}
	//	TaskManagerArgs.SchedulerConditionVariableMutex->Lock();
	//	TaskManagerArgs.SchedulerConditionVariable->Wait(*TaskManagerArgs.SchedulerConditionVariableMutex);
	//	TaskManagerArgs.SchedulerConditionVariableMutex->Unlock();
	//}

	return 0;
}

uint32_t TaskManager::TaskClean(_In_ void* Args)
{
	//ETERNAL_ASSERT(Args);
	//CleanTasksArguments& CleanTasks = *(CleanTasksArguments*)Args;

	//for (;;)
	//{
	//	{
	//		MutexAutoLock CleanMutex(CleanTasks.TasksToCleanMutex);
	//		for (list<Task*>::iterator TaskIt = CleanTasks.TasksToClean->begin(); TaskIt != CleanTasks.TasksToClean->end(); )
	//		{
	//			Task* Current = *TaskIt;
	//			if (Current->IsFinished() && Current->IsNotReferenced())
	//			{
	//				TaskIt = CleanTasks.TasksToClean->erase(TaskIt);
	//				delete Current;
	//			}
	//			else
	//			{
	//				++TaskIt;
	//			}
	//		}
	//	}
	//	
	//	Sleep(1);
	//}

	return 0;
}

TaskManager::TaskManager()
{
	ETERNAL_ASSERT(!_Inst);

	//_TasksListMutex = new StdMutex();
	//_TasksToCleanMutex = new StdMutex();

	_NewTasksMutex = new StdMutex();
	_ExecutingTasksMutex = new StdMutex();
	_ExecutedTasksMutex = new StdMutex();

	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	{
		_WorkersThreads[WorkerIndex] = new StdThread();
		_Workers[WorkerIndex] = new Worker(_WorkersThreads[WorkerIndex]);
	}

	//_SchedulerConditionVariable = new StdConditionVariable();
	//_SchedulerConditionVariableMutex = new StdMutex();

	_TaskManager = new StdThread();

	TaskManagerArguments* TaskManagerArgs = new TaskManagerArguments();
	TaskManagerArgs->Workers = _Workers;
	TaskManagerArgs->NewTasks = &_NewTasks;
	TaskManagerArgs->NewTasksMutex = _NewTasksMutex;
	TaskManagerArgs->ExecutingTasks = &_ExecutingTasks;
	TaskManagerArgs->ExecutingTasksMutex = _ExecutingTasksMutex;
	TaskManagerArgs->ExecutedTasks = &_ExecutedTasks;
	TaskManagerArgs->ExecutedTasksMutex = _ExecutedTasksMutex;

	_TaskManager->Create(TaskManager::TaskRun, TaskManagerArgs);

	//_TasksList = new Scheduler(_TasksListMutex);

	//TaskManagerArguments* TaskManagerArgs = new TaskManagerArguments;
	//TaskManagerArgs->Workers = _Workers;
	//TaskManagerArgs->TasksList = _TasksList;
	//TaskManagerArgs->SchedulerConditionVariable = _SchedulerConditionVariable;
	//TaskManagerArgs->SchedulerConditionVariableMutex = _SchedulerConditionVariableMutex;
	//TaskManagerArgs->TasksToClean = &_TasksToClean;
	//TaskManagerArgs->TasksToCleanMutex = _TasksToCleanMutex;

	//_TaskManager->Create(TaskManager::TaskRun, TaskManagerArgs);

	//_CleanTasks = new StdThread();

	//CleanTasksArguments* CleanTasksArgs = new CleanTasksArguments;
	//CleanTasksArgs->TasksToClean = &_TasksToClean;
	//CleanTasksArgs->TasksToCleanMutex = _TasksToCleanMutex;

	//_CleanTasks->Create(TaskManager::TaskClean, CleanTasksArgs);

	_Inst = this;
}

TaskManager::~TaskManager()
{
	_Inst = nullptr;

	delete _TaskManager;
	_TaskManager = nullptr;

	//delete _SchedulerConditionVariable;
	//_SchedulerConditionVariable = nullptr;

	//delete _SchedulerConditionVariableMutex;
	//_SchedulerConditionVariableMutex = nullptr;

	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	{
		delete _WorkersThreads[WorkerIndex];
		_WorkersThreads[WorkerIndex] = nullptr;

		delete _Workers[WorkerIndex];
		_Workers[WorkerIndex] = nullptr;
	}

	delete _ExecutedTasksMutex;
	_ExecutedTasksMutex = nullptr;

	delete _ExecutingTasksMutex;
	_ExecutingTasksMutex = nullptr;

	delete _NewTasksMutex;
	_NewTasksMutex = nullptr;

	//delete _TasksToCleanMutex;
	//_TasksToCleanMutex = nullptr;

	//delete _TasksListMutex;
	//_TasksListMutex = nullptr;
}

TaskManager* TaskManager::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

void TaskManager::Push(_In_ Task* TaskObj, _In_ Task* DependsOn /* = nullptr*/)
{
	_NewTasksMutex->Lock();
	_NewTasks.push_back(TaskObj);
	_NewTasksMutex->Unlock();

	//_TasksList->Push(TaskObj, DependsOn);

	//_SchedulerConditionVariableMutex->Lock();
	//_SchedulerConditionVariable->NotifyAll();
	//_SchedulerConditionVariableMutex->Unlock();
}

void TaskManager::Barrier()
{
	for (;;)
	{
		bool NewTasksEmpty = false;
		bool ExecutingTasksEmpty = false;

		if (_NewTasksMutex->TryLock())
		{
			size_t NewTasksCount = _NewTasks.size();
			_NewTasksMutex->Unlock();

			if (NewTasksCount == 0)
				OutputDebugString("NO NEW TASK\n");
			else
				OutputDebugString("NEW TASK OCCURS\n");
			NewTasksEmpty = !NewTasksCount;
		}

		if (_ExecutingTasksMutex->TryLock())
		{
			size_t ExecutingTasksCount = _ExecutingTasks.size();
			_ExecutingTasksMutex->Unlock();

			if (ExecutingTasksCount == 0)
				OutputDebugString("NO EXECUTING TASK\n");
			else
				OutputDebugString("EXECUTING TASK OCCURS\n");
			ExecutingTasksEmpty = !ExecutingTasksCount;
		}

		if (ExecutingTasksEmpty && NewTasksEmpty)
		{
			OutputDebugString("END OF BARRIER\n");
			break;
		}

		OutputDebugString("SLEEP\n");

		Sleep(1);
	}
}
