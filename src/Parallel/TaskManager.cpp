#include "Parallel/TaskManager.hpp"

#include <cassert>

#include "Parallel/StdMutex.hpp"
#include "Parallel/StdThread.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Worker.hpp"

using namespace Eternal::Parallel;

TaskManager* TaskManager::_Inst = nullptr;

uint32_t TaskManager::TaskRun(void* Args)
{
	Worker** Workers = (Worker**)((void**)Args)[0];
	list<Task*>* TaskList = (list<Task*>*)((void**)Args)[1];
	Mutex* TasksListMutex = (Mutex*)((void**)Args)[2];
	ConditionVariable* TaskRunConditionVariable = (ConditionVariable*)((void**)Args)[3];
	Mutex* TaskRunConditionVariableMutex = (Mutex*)((void**)Args)[4];

	for (;;)
	{
		while (TaskList->size() > 0)
		{
			for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
			{
				if (!Workers[WorkerIndex]->TaskIsFinished())
				{
					continue;
				}

				TasksListMutex->Lock();
				
				if (TaskList->size() == 0)
				{
					TasksListMutex->Unlock();
					break;
				}

				Task* NewTask = TaskList->front();
				TaskList->pop_front();

				TasksListMutex->Unlock();

				Workers[WorkerIndex]->SetTask(NewTask);
			}
		}
		TaskRunConditionVariableMutex->Lock();
		TaskRunConditionVariable->Wait(*TaskRunConditionVariableMutex);
		TaskRunConditionVariableMutex->Unlock();
	}
	return 0;
}

uint32_t TaskManager::WorkerRun(void* Args)
{
	((Worker*)Args)->DoTask();
	return 0;
}

TaskManager::TaskManager()
{
	assert(!_Inst);

	_TasksListMutex = new StdMutex();
	
	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	{
		_Workers[WorkerIndex] = new Worker();
		_WorkersThreads[WorkerIndex] = new StdThread();
		_WorkersThreads[WorkerIndex]->Create(TaskManager::WorkerRun, _Workers[WorkerIndex]);
	}

	_TaskRunConditionVariable = new StdConditionVariable();
	_TaskRunConditionVariableMutex = new StdMutex();

	_Scheduler = new StdThread();
	void* TaskRunArgs[] = {
		_Workers,
		&_TasksList,
		_TasksListMutex,
		_TaskRunConditionVariable,
		_TaskRunConditionVariableMutex
	};
	_Scheduler->Create(TaskManager::TaskRun, TaskRunArgs);

	_Inst = this;
}

TaskManager::~TaskManager()
{
	_Inst = nullptr;

	delete _Scheduler;
	_Scheduler = nullptr;

	delete _TaskRunConditionVariable;
	_TaskRunConditionVariable = nullptr;

	delete _TaskRunConditionVariableMutex;
	_TaskRunConditionVariableMutex = nullptr;

	for (uint32_t WorkerIndex = 0; WorkerIndex < TASK_MANAGER_WORKERS_COUNT; ++WorkerIndex)
	{
		delete _WorkersThreads[WorkerIndex];
		_WorkersThreads[WorkerIndex] = nullptr;

		delete _Workers[WorkerIndex];
		_Workers[WorkerIndex] = nullptr;
	}

	delete _TasksListMutex;
	_TasksListMutex = nullptr;
}

TaskManager* TaskManager::Get()
{
	assert(_Inst);
	return _Inst;
}

void TaskManager::Push(Task* TaskObj)
{
	_TasksListMutex->Lock();
	_TasksList.push_back(TaskObj);
	_TasksListMutex->Unlock();

	_TaskRunConditionVariableMutex->Lock();
	_TaskRunConditionVariable->NotifyAll();
	_TaskRunConditionVariableMutex->Unlock();
}
