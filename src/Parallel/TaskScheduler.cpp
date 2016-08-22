#include "Parallel/TaskScheduler.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include "Macros/Macros.hpp"

#include "Parallel/StdMutex.hpp"
#include "Parallel/MutexAutoLock.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/StdAtomicS32.hpp"

using namespace Eternal::Parallel;

TaskScheduler::TaskScheduler()
{
	_TasksMutex = new StdMutex();
	_UnscheduledTasks = new StdAtomicS32(0);
}

TaskScheduler::~TaskScheduler()
{
	delete _TasksMutex;
	_TasksMutex = nullptr;
}

void TaskScheduler::PushTask(Task* ChildTask, Task* ParentTask /* = nullptr */)
{
	ETERNAL_ASSERT(ChildTask != ParentTask);
	ETERNAL_ASSERT(ChildTask->GetState() == Task::IDLE);

	MutexAutoLock MutexAutoLockObj(_TasksMutex);

	Dependency* TaskDependency = nullptr;
	for (int TaskIndex = 0; !TaskDependency && TaskIndex < _TasksList.size(); ++TaskIndex)
	{
		Dependency& CurrentDependency = _TasksList[TaskIndex];
		if (_TasksList[TaskIndex].TaskObj == ChildTask)
			TaskDependency = &CurrentDependency;
	}
	if (!TaskDependency)
	{
		_TasksList.push_back(Dependency());
		TaskDependency = &_TasksList.back();
		TaskDependency->TaskObj = ChildTask;
		_UnscheduledTasks->Add();
	}
	if (ParentTask)
		TaskDependency->Dependencies.push_back(ParentTask);
}

Task* TaskScheduler::PopTask()
{
	MutexAutoLock MutexAutoLockObj(_TasksMutex);

	for (int TaskIndex = 0; TaskIndex < _TasksList.size(); ++TaskIndex)
	{
		Dependency& TaskDependency = _TasksList[TaskIndex];
		if (TaskDependency.TaskObj->GetState() == Task::IDLE && _IsSchedulable(TaskDependency.Dependencies))
		{
			TaskDependency.TaskObj->Schedule();
			_UnscheduledTasks->Sub();
			return TaskDependency.TaskObj;
		}
	}

	return nullptr;
}

bool TaskScheduler::_IsSchedulable(const vector<Task*>& SubTasks) const
{
	bool IsSchedulable = true;
	for (int TaskIndex = 0; IsSchedulable && TaskIndex < SubTasks.size(); ++TaskIndex)
	{
		IsSchedulable &= SubTasks[TaskIndex]->GetState() == Task::DONE;
	}
	return IsSchedulable;
}

int TaskScheduler::RemainingUnscheduledTasks() const
{
	return _UnscheduledTasks->Load();
}

bool TaskScheduler::Done() const
{
	bool DoneTasks = true;
	for (int TaskIndex = 0; DoneTasks && TaskIndex < _TasksList.size(); ++TaskIndex)
	{
		DoneTasks &= _TasksList[TaskIndex].TaskObj->GetState() == Task::DONE;
	}
	return DoneTasks;
}

void TaskScheduler::Reset()
{
	ETERNAL_ASSERT(!RemainingUnscheduledTasks());
	ETERNAL_ASSERT(Done());

//#ifdef ETERNAL_DEBUG
//	char UnscheduledTasksCount[256];
//	sprintf_s(UnscheduledTasksCount, "[TaskScheduler::Reset]Task Count : %d\n", RemainingUnscheduledTasks());
//	OutputDebugString(UnscheduledTasksCount);
//#endif

	for (int TaskIndex = 0; TaskIndex < _TasksList.size(); ++TaskIndex)
	{
		_TasksList[TaskIndex].TaskObj->Reset();
	}
	_UnscheduledTasks->Store(_TasksList.size());
}
