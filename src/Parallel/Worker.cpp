#include "Parallel/Worker.hpp"

#include <cassert>

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
		if (!_CurrentTask->IsFinished())
		{
			_CurrentTask->DoTask();
		}
		_ConditionVariable->Wait(*_ConditionVariableMutex);
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

void Worker::SetTask(Task* TaskObj)
{
	assert(TaskIsFinished());
	_CurrentTask = TaskObj;
	_ConditionVariable->NotifyAll();
}
