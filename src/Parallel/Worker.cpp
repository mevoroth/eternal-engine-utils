#include "Parallel/Worker.hpp"

#include "Macros/Macros.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/StdMutex.hpp"

using namespace Eternal::Parallel;

Worker::Worker()
{
	_ConditionVariable = new StdConditionVariable();
	_ConditionVariableMutex = new StdMutex();
}

void Worker::DoTask()
{
	for (;;)
	{
		if (_CurrentTask && !_CurrentTask->IsFinished())
		{
			_CurrentTask->DoTask();
		}
		_ConditionVariableMutex->Lock();
		_ConditionVariable->Wait(*_ConditionVariableMutex);
		_ConditionVariableMutex->Unlock();
	}
}

bool Worker::TaskIsFinished() const
{
	if (!_CurrentTask)
	{
		return true;
	}

	return _CurrentTask->IsFinished();
}

void Worker::RemoveTask()
{
	ETERNAL_ASSERT(TaskIsFinished());
	if (_CurrentTask)
	{
		delete _CurrentTask;
		_CurrentTask = nullptr;
	}
}

void Worker::SetTask(Task* TaskObj)
{
	ETERNAL_ASSERT(!_CurrentTask);
	_CurrentTask = TaskObj;
	_ConditionVariableMutex->Lock();
	_ConditionVariable->NotifyAll();
	_ConditionVariableMutex->Unlock();
}
