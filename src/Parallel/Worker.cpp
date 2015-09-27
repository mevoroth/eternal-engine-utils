#include "Parallel/Worker.hpp"

#include "Macros/Macros.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/Thread.hpp"

using namespace Eternal::Parallel;

uint32_t Worker::WorkerRun(void* Args)
{
	WorkerArguments* WorkerArgs = (WorkerArguments*)Args;
	WorkerArgs->WorkerObj->DoTask();
	return 0;
}

Worker::Worker(Thread* ThreadObj)
	: _Thread(ThreadObj)
{
	_ConditionVariable = new StdConditionVariable();
	_ConditionVariableMutex = new StdMutex();

	WorkerArguments* Arguments = new WorkerArguments;
	Arguments->WorkerObj = this;

	_Thread->Create(Worker::WorkerRun, Arguments);
}

Worker::~Worker()
{
	delete _ConditionVariable;
	_ConditionVariable = nullptr;
	delete _ConditionVariableMutex;
	_ConditionVariableMutex = nullptr;
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

void Worker::SetTask(Task* TaskObj)
{
	_CurrentTask = TaskObj;
	_ConditionVariableMutex->Lock();
	_ConditionVariable->NotifyAll();
	_ConditionVariableMutex->Unlock();
}
