#include "Parallel/Scheduler.hpp"

#include "Parallel/Task.hpp"
#include "Parallel/MutexAutoLock.hpp"

using namespace Eternal::Parallel;

Scheduler::Scheduler(Mutex* MutexObj)
	: _MutexObj(MutexObj)
{
}

void Scheduler::Push(_In_ Task* TaskObj, _In_ Task* TaskDependency /* = nullptr */)
{
	MutexAutoLock TaskListMutex(this->_MutexObj);
	SchedulerNode Node;
	Node.TaskObj = TaskObj;
	Node.DependsOn = TaskDependency;
	if (Node.DependsOn)
	{
		Node.DependsOn->AddRef();
	}
	_TasksList.push_back(Node);
}

Task* Scheduler::Pop()
{
	MutexAutoLock TaskListMutex(this->_MutexObj);
	for (list<SchedulerNode>::const_iterator NodeIt = _TasksList.cbegin(); NodeIt != _TasksList.cend(); ++NodeIt)
	{
		if (!NodeIt->DependsOn || NodeIt->DependsOn->IsFinished())
		{
			Task* Node = NodeIt->TaskObj;
			if (NodeIt->DependsOn)
			{
				NodeIt->DependsOn->Release();
			}
			_TasksList.remove(*NodeIt);
			return Node;
		}
	}
	return nullptr;
}

size_t Scheduler::GetRemainingTasks() const
{
	MutexAutoLock TaskListMutex(this->_MutexObj);
	return _TasksList.size();
}
