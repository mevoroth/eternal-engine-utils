#include "Parallel/Worker.hpp"

#include "Parallel/Thread.hpp"
#include "Parallel/ThreadFactory.hpp"
#include "Parallel/AtomicS32.hpp"
#include "Parallel/AtomicS32Factory.hpp"
#include "Parallel/ConditionVariable.hpp"
#include "Parallel/ConditionVariableFactory.hpp"
#include "Parallel/Mutex.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Sleep.hpp"

namespace Eternal
{
	namespace Parallel
	{
		Worker::WorkerArguments::WorkerArguments()
			: WorkerRunning(CreateAtomicS32(1))
			, WorkerExecuting(CreateAtomicS32())
			, WorkerDone(CreateAtomicS32())
			, WorkerConditionVariableMutex(CreateMutex())
			, WorkerConditionVariable(CreateConditionVariable())
		{
		}

		Worker::WorkerArguments::~WorkerArguments()
		{
			DestroyConditionVariable(WorkerConditionVariable);
			DestroyMutex(WorkerConditionVariableMutex);
			DestroyAtomicS32(WorkerDone);
			DestroyAtomicS32(WorkerExecuting);
			DestroyAtomicS32(WorkerRunning);
		}

		uint32_t Worker::WorkerRunGeneric(_In_ void* InArguments)
		{
			ETERNAL_ASSERT(InArguments);
			WorkerArguments& Arguments = *(WorkerArguments*)InArguments;

			OPTICK_THREAD(Arguments.Name.c_str());

			while (Arguments.WorkerRunning->Load())
			{
				OPTICK_FRAME(Arguments.Name.c_str());
				Arguments.WorkerConditionVariableMutex->Lock();
				Arguments.WorkerConditionVariable->Wait(*Arguments.WorkerConditionVariableMutex);

				if (Arguments.WorkerTask)
				{
					Arguments.WorkerExecuting->Store(1);
					Arguments.WorkerTask->Execute();
					Arguments.WorkerTask = nullptr;
					Arguments.WorkerExecuting->Store(0);
				}
			}

			Arguments.WorkerDone->Store(1);

			return 0;
		}

		uint32_t Worker::WorkerRunDedicated(_In_ void* InArguments)
		{
			ETERNAL_ASSERT(InArguments);
			WorkerArguments& Arguments = *(WorkerArguments*)InArguments;

			OPTICK_THREAD(Arguments.Name.c_str());

			while (Arguments.WorkerRunning->Load())
			{
				OPTICK_FRAME(Arguments.Name.c_str());
				if (!Arguments.WorkerTask)
				{
					Arguments.WorkerConditionVariableMutex->Lock();
					Arguments.WorkerConditionVariable->Wait(*Arguments.WorkerConditionVariableMutex);
				}

				Arguments.WorkerExecuting->Store(1);
				Arguments.WorkerTask->Execute();
				Arguments.WorkerExecuting->Store(0);
			}

			Arguments.WorkerDone->Store(1);

			return 0;
		}

		Worker::Worker(_In_ const string& InName, _In_ ThreadFunction InFunction)
		{
			_WorkerArguments.Name = InName;
			_Thread = CreateThread(InName, InFunction == nullptr ? &WorkerRunGeneric : InFunction, &_WorkerArguments);
		}

		Worker::~Worker()
		{
			DestroyThread(_Thread);
		}

		bool Worker::IsAvailable() const
		{
			return _WorkerArguments.WorkerExecuting->Load() == 0;
		}

		bool Worker::IsDone() const
		{
			return _WorkerArguments.WorkerDone->Load() == 1;
		}

		void Worker::Shutdown()
		{
			_WorkerArguments.WorkerRunning->Store(0);
		}

		void Worker::WakeWorker()
		{
			_WorkerArguments.WorkerConditionVariableMutex->Unlock();
			_WorkerArguments.WorkerConditionVariable->NotifyOne();
		}

		bool Worker::EnqueueTask(_In_ Task* InTask)
		{
			if (!IsAvailable())
				return false;

			ETERNAL_ASSERT(!_WorkerArguments.WorkerTask);

			_WorkerArguments.WorkerTask = InTask;

			WakeWorker();

			return true;
		}
	}
}
