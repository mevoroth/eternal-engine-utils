#include "Parallel/Worker.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include "Parallel/Thread.hpp"
#include "Parallel/StdAtomicS32.hpp"
#include "Parallel/StdConditionVariable.hpp"
#include "Parallel/StdMutex.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Sleep.hpp"

#include "Parallel/Task.hpp"

using namespace Eternal::Parallel;

uint32_t Worker::WorkerRun(void* Args)
{
	WorkerArgs* Arguments = (WorkerArgs*)Args;

	while (Arguments->Running->Load())
	{
		Arguments->WorkerConditionVariableMutex->Lock();
		Arguments->WorkerConditionVariable->Wait(*Arguments->WorkerConditionVariableMutex);
		Arguments->WorkerConditionVariableMutex->Unlock();

		Arguments->IsExecutingTask->Store(1);

		while (Arguments->IsPushing->Load())
			Sleep(1);

		//ETERNAL_ASSERT(Arguments->TaskObj->GetState() == Task::SETUP);

//#ifdef ETERNAL_DEBUG
//		OutputDebugString("Executing ");
//		OutputDebugString(Arguments->TaskObj->GetTaskName().c_str());
//		OutputDebugString("\n");
//#endif

		Arguments->TaskObj->Setup();
		Arguments->TaskObj->Execute();

//#ifdef ETERNAL_DEBUG
//		OutputDebugString("Finishing ");
//		OutputDebugString(Arguments->TaskObj->GetTaskName().c_str());
//		OutputDebugString("\n");
//#endif

		Arguments->IsAvailable->Store(1);
	}

	return 0;
}

Worker::Worker(Thread* ThreadObj)
{
	_Running = new StdAtomicS32(1);
	_WorkerConditionVariableMutex = new StdMutex();
	_WorkerConditionVariable = new StdConditionVariable();
	_IsExecutingTask = new StdAtomicS32(0);
	_IsAvailable = new StdAtomicS32(1);
	_Pushing = new StdAtomicS32(0);

	_WorkerArgs = new WorkerArgs(
		_Running,
		_WorkerConditionVariable,
		_WorkerConditionVariableMutex,
		_Task,
		_IsExecutingTask,
		_IsAvailable,
		_Pushing
	);

	ThreadObj->Create(Worker::WorkerRun, _WorkerArgs);
}

Worker::~Worker()
{
	delete _WorkerArgs;
	_WorkerArgs = nullptr;

	delete _Pushing;
	_Pushing = nullptr;

	delete _IsAvailable;
	_IsAvailable = nullptr;

	delete _IsExecutingTask;
	_IsExecutingTask = nullptr;

	delete _WorkerConditionVariable;
	_WorkerConditionVariable = nullptr;
	
	delete _WorkerConditionVariableMutex;
	_WorkerConditionVariableMutex = nullptr;

	delete _Running;
	_Running = nullptr;
}

void Worker::Shutdown()
{
	ETERNAL_ASSERT(_Running->Load());
	_Running->Store(0);
}

bool Worker::IsAvailable() const
{
	return _IsAvailable->Load() != 0;
}

bool Worker::PushTask(Task* TaskObj)
{
	ETERNAL_ASSERT(IsAvailable());

	if (_Pushing->CompareAndSwap(0, 1))
		return false;

	_IsAvailable->Store(0);
	_IsExecutingTask->Store(0);
	_Task = TaskObj;

	while (!_IsExecutingTask->Load())
	{
		_WorkerConditionVariableMutex->Lock();
		_WorkerConditionVariable->NotifyOne();
		_WorkerConditionVariableMutex->Unlock();

		Sleep(1);
	}

	_Pushing->Store(0);
	return true;
}
