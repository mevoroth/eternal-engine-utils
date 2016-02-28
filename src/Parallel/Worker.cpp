#include "Parallel/Worker.hpp"

#include "Macros/Macros.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/Thread.hpp"
#include "Parallel/MutexAutoLock.hpp"

using namespace Eternal::Parallel;

uint32_t Worker::WorkerRun(void* Args)
{
	WorkerArguments* WorkerArgs = (WorkerArguments*)Args;
	WorkerArgs->WorkerObj->Setup();
	WorkerArgs->WorkerObj->Execute();
	return 0;
}

Worker::Worker(Thread* ThreadObj)
	: _Thread(ThreadObj)
{
	_ConditionVariable = new StdConditionVariable();
	_ConditionVariableMutex = new StdMutex();
	_TaskMutex = new StdMutex();

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
	delete _TaskMutex;
	_TaskMutex = nullptr;
}

bool Worker::TaskIsExecuted()
{
	Task* CurrentTask = _Task;
	return !CurrentTask || CurrentTask->TaskIsExecuted();
}

void Worker::Setup()
{
}

void Worker::Execute()
{
	for (;;)
	{
		Task* CurrentTask = _Task;
		if (CurrentTask && !CurrentTask->TaskIsExecuted())
		{
			CurrentTask->Execute();
			{
				MutexAutoLock TaskMutex(_TaskMutex);
				_Task = nullptr;
			}
		}

		_ConditionVariableMutex->Lock();
		_ConditionVariable->Wait(*_ConditionVariableMutex);
		_ConditionVariableMutex->Unlock();
	}
}

void Worker::SetTask(Task* TaskObj)
{
	ETERNAL_ASSERT(TaskObj->TaskIsExecutable());
	ETERNAL_ASSERT(!TaskObj->TaskIsExecuted());
	ETERNAL_ASSERT(TaskIsExecuted());

	{
		MutexAutoLock TaskMutex(_TaskMutex);
		_Task = TaskObj;
	}

	_ConditionVariableMutex->Lock();
	_ConditionVariable->NotifyAll();
	_ConditionVariableMutex->Unlock();
}
