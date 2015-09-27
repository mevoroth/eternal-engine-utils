#include "Parallel/TaskManager.hpp"

#include "Macros/Macros.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/StdThread.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Worker.hpp"
#include "Parallel/MutexAutoLock.hpp"
#include "Parallel/Scheduler.hpp"

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
			if (!TaskManagerArgs.Workers[WorkerIndex]->TaskIsFinished())
			{
				continue;
			}

			{
				MutexAutoLock(TaskManagerArgs.TasksToCleanMutex);
				Task* CurrentTaskInWorker = TaskManagerArgs.Workers[WorkerIndex]->GetTask();
				if (CurrentTaskInWorker)
					TaskManagerArgs.TasksToClean->push_back(CurrentTaskInWorker);
			}

			Task* NewTask = nullptr;
			while (!NewTask)
			{
				NewTask = TaskManagerArgs.TasksList->Pop();
				if (!NewTask)
					TaskManagerArgs.TaskManagerThread->Sleep(1);
			}

			TaskManagerArgs.Workers[WorkerIndex]->SetTask(NewTask);
		}
		TaskManagerArgs.SchedulerConditionVariableMutex->Lock();
		TaskManagerArgs.SchedulerConditionVariable->Wait(*TaskManagerArgs.SchedulerConditionVariableMutex);
		TaskManagerArgs.SchedulerConditionVariableMutex->Unlock();
	}

	return 0;
}

uint32_t TaskManager::TaskClean(_In_ void* Args)
{
	ETERNAL_ASSERT(Args);
	CleanTasksArguments& CleanTasks = *(CleanTasksArguments*)Args;

	for (;;)
	{
		for (list<Task*>::iterator TaskIt = CleanTasks.TasksList->begin(); TaskIt != CleanTasks.TasksList->end(); )
		{
			MutexAutoLock(CleanTasks.TasksListMutex);
			Task* Current = *TaskIt;
			if (Current->IsFinished() && Current->IsNotReferenced())
			{
				TaskIt = CleanTasks.TasksList->erase(TaskIt);
				delete Current;
			}
			else
			{
				++TaskIt;
			}
		}
		CleanTasks.CleanTaskThread->Sleep(1);
	}

	return 0;
}

TaskManager::TaskManager()
{
	ETERNAL_ASSERT(!_Inst);

	_TasksListMutex = new StdMutex();
	_TasksToCleanMutex = new StdMutex();

	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	{
		_WorkersThreads[WorkerIndex] = new StdThread();
		_Workers[WorkerIndex] = new Worker(_WorkersThreads[WorkerIndex]);
	}

	_SchedulerConditionVariable = new StdConditionVariable();
	_SchedulerConditionVariableMutex = new StdMutex();

	_TaskManager = new StdThread();

	_TasksList = new Scheduler(_TasksListMutex);

	TaskManagerArguments* TaskManagerArgs = new TaskManagerArguments;
	TaskManagerArgs->Workers = _Workers;
	TaskManagerArgs->TasksList = _TasksList;
	TaskManagerArgs->SchedulerConditionVariable = _SchedulerConditionVariable;
	TaskManagerArgs->SchedulerConditionVariableMutex = _SchedulerConditionVariableMutex;
	TaskManagerArgs->TaskManagerThread = _TaskManager;
	TaskManagerArgs->TasksToClean = &_TasksToClean;
	TaskManagerArgs->TasksToCleanMutex = _TasksToCleanMutex;

	_TaskManager->Create(TaskManager::TaskRun, TaskManagerArgs);

	_CleanTasks = new StdThread();

	CleanTasksArguments* CleanTasksArgs = new CleanTasksArguments;
	CleanTasksArgs->TasksList = &_TasksToClean;
	CleanTasksArgs->TasksListMutex = _TasksToCleanMutex;
	CleanTasksArgs->CleanTaskThread = _CleanTasks;

	_CleanTasks->Create(TaskManager::TaskClean, CleanTasksArgs);

	_Inst = this;
}

TaskManager::~TaskManager()
{
	_Inst = nullptr;

	delete _TaskManager;
	_TaskManager = nullptr;

	delete _SchedulerConditionVariable;
	_SchedulerConditionVariable = nullptr;

	delete _SchedulerConditionVariableMutex;
	_SchedulerConditionVariableMutex = nullptr;

	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	{
		delete _WorkersThreads[WorkerIndex];
		_WorkersThreads[WorkerIndex] = nullptr;

		delete _Workers[WorkerIndex];
		_Workers[WorkerIndex] = nullptr;
	}

	delete _TasksToCleanMutex;
	_TasksToCleanMutex = nullptr;

	delete _TasksListMutex;
	_TasksListMutex = nullptr;
}

TaskManager* TaskManager::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

void TaskManager::Push(_In_ Task* TaskObj, _In_ Task* DependsOn /* = nullptr*/)
{
	_TasksList->Push(TaskObj, DependsOn);

	_SchedulerConditionVariableMutex->Lock();
	_SchedulerConditionVariable->NotifyAll();
	_SchedulerConditionVariableMutex->Unlock();
}
