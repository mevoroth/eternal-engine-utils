#include "Parallel/Worker.hpp"

#include "Macros/Macros.hpp"
#include "Parallel/Thread.hpp"
#include "Parallel/StdAtomicInt.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Sleep.hpp"

using namespace Eternal::Parallel;

uint32_t Worker::WorkerRun(void* Args)
{
	WorkerArgs* Arguments = (WorkerArgs*)Args;

	while (Arguments->Running->Load())
	{
		Arguments->WorkerConditionVariableMutex->Lock();
		Arguments->WorkerConditionVariable->Wait(*Arguments->WorkerConditionVariableMutex);
		Arguments->WorkerConditionVariableMutex->Unlock();

		Arguments->IsExecutingTask = true;
		//ETERNAL_ASSERT(Arguments->TaskObj->GetState() == Task::SETUP);
		Arguments->TaskObj->Setup();
		Arguments->TaskObj->Execute();

		Arguments->IsAvailable = true;
	}

	return 0;
}

Worker::Worker(Thread* ThreadObj)
{
	_Running = new StdAtomicInt(1);
	_WorkerConditionVariableMutex = new StdMutex();
	_WorkerConditionVariable = new StdConditionVariable();

	_WorkerArgs = new WorkerArgs(
		_Running,
		_WorkerConditionVariable,
		_WorkerConditionVariableMutex,
		_Task,
		_IsExecutingTask,
		_IsAvailable
	);

	ThreadObj->Create(Worker::WorkerRun, _WorkerArgs);
}

Worker::~Worker()
{
	delete _WorkerArgs;
	_WorkerArgs = nullptr;
}

void Worker::Shutdown()
{
	ETERNAL_ASSERT(_Running->Load());
	_Running->Store(0);
}

bool Worker::IsAvailable() const
{
	return _IsAvailable;
}

void Worker::PushTask(Task* TaskObj)
{
	ETERNAL_ASSERT(_IsAvailable);
	_IsAvailable = false;
	_IsExecutingTask = false;
	_Task = TaskObj;

	while (!_IsExecutingTask)
	{
		_WorkerConditionVariableMutex->Lock();
		_WorkerConditionVariable->NotifyOne();
		_WorkerConditionVariableMutex->Unlock();

		Sleep(1);
	}
}
