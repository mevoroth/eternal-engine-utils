#include "Parallel/ParallelSystem.hpp"
#include "Parallel/Worker.hpp"
#include "Parallel/CpuCoreCount.hpp"
#include "Parallel/ThreadFactory.hpp"
#include "Parallel/Task.hpp"
#include "Parallel/Mutex.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/MutexAutoLock.hpp"
#include "Parallel/Sleep.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace Parallel
	{
		using namespace Eternal::Math;

		ParallelSystem::ParallelSystem(_In_ const ParallelSystemCreateInformation& InParallelSystemCreateInformation)
			: _ParallelSystemCreateInformation(InParallelSystemCreateInformation)
			, _CPUCount(static_cast<int32_t>(CpuCoreCount()))
			, _Scheduler(InParallelSystemCreateInformation.FrameCount)
			
		{
			_UsedCPUCount = Max(_CPUCount - 1, 0);
			if (_UsedCPUCount > 0)
			{
				_Workers.resize(_UsedCPUCount);

				if (_UsedCPUCount > 1)
				{
					int32_t DedicatedCPUCoreCount = static_cast<int32_t>(InParallelSystemCreateInformation.DedicatedTasks.size());
					_GenericWorkersCount = Max(static_cast<int32_t>(_UsedCPUCount) - DedicatedCPUCoreCount, 1);
					_DedicatedWorkersCount = _UsedCPUCount - _GenericWorkersCount;

					int32_t WorkerIndex = 0;
					for (; WorkerIndex < _GenericWorkersCount; ++WorkerIndex)
					{
						std::string WorkerName = std::string("Worker ") + std::to_string(WorkerIndex);
						_Workers[WorkerIndex] = new Worker(WorkerName);
					}
					for (int32_t DedicatedWorkerIndex = 0; DedicatedWorkerIndex < _DedicatedWorkersCount; ++DedicatedWorkerIndex)
					{
						std::string WorkerName = std::string("Worker ") + InParallelSystemCreateInformation.DedicatedTasks[DedicatedWorkerIndex]->GetTaskName();
						_Workers[WorkerIndex++] = new Worker(WorkerName, _HasAllDedicatedCore() ? Worker::WorkerRunDedicated : nullptr);
					}

					if (_HasAllDedicatedCore())
					{
						for (int32_t DedicatedWorkerIndex = 0; DedicatedWorkerIndex < _DedicatedWorkersCount; ++DedicatedWorkerIndex)
							_Workers[_GenericWorkersCount + DedicatedWorkerIndex]->EnqueueTask(InParallelSystemCreateInformation.DedicatedTasks[DedicatedWorkerIndex]);
					}
				}
				else
				{
					_Workers[0] = new Worker("Worker");
				}

			}
		}

		ParallelSystem::~ParallelSystem()
		{
			for (uint32_t WorkerIndex = 0; WorkerIndex < _Workers.size(); ++WorkerIndex)
				_Workers[WorkerIndex]->Shutdown();

			for (uint32_t WorkerIndex = 0; WorkerIndex < _Workers.size(); ++WorkerIndex)
			{
				while (!_Workers[WorkerIndex]->IsDone())
				{
					_Workers[WorkerIndex]->WakeWorker();
					Sleep(1);
				}

				delete _Workers[WorkerIndex];
			}
		}

		void ParallelSystem::StartFrame()
		{
			_Scheduler.StartFrame();
			_FlushPendingTasks();
		}

		void ParallelSystem::ExecuteTask(_In_ Task* InTask, _In_ const DependenciesType& Dependencies)
		{
			ETERNAL_PROFILER(DETAIL)();

			_Scheduler.RegisterTask(InTask, Dependencies);
			_FlushPendingTasks();
		}

		void ParallelSystem::EndFrame()
		{
			_Scheduler.EndFrame();
			{
				ETERNAL_PROFILER(BASIC)("WaitForFreeFrame");
				while (_Scheduler.OldestFrameHasPendingTasks())
				{
					_FlushPendingTasks();
					Sleep(1);
				}
			}
		}

		void ParallelSystem::_FlushPendingTasks()
		{
			ETERNAL_PROFILER(DETAIL)();

			for (int32_t WorkerIndex = 0; WorkerIndex < _GenericWorkersCount; ++WorkerIndex)
			{
				if (_Workers[WorkerIndex]->IsAvailable())
				{
					if (Task* EnqueuedTask = _Scheduler.GetNextTask())
					{
						_Workers[WorkerIndex]->EnqueueTask(EnqueuedTask);
						_Scheduler.AdvanceNextTask();
					}
				}
			}

			if (!_HasAllDedicatedCore())
			{
				if (_UsedCPUCount == 1)
				{
					_Scheduler.RegisterTask(_ParallelSystemCreateInformation.DedicatedTasks[_DedicatedTaskIndex]);
					_DedicatedTaskIndex = (_DedicatedTaskIndex + 1) % _ParallelSystemCreateInformation.DedicatedTasks.size();
				}
				else
				{
					vector<Task*>& DedicatedTasks = _ParallelSystemCreateInformation.DedicatedTasks;
					for (int32_t DedicatedWorkerIndex = 0; DedicatedWorkerIndex < _DedicatedWorkersCount; ++DedicatedWorkerIndex)
					{
						if (_Workers[_GenericWorkersCount + DedicatedWorkerIndex]->IsAvailable())
						{
							_Workers[_GenericWorkersCount + DedicatedWorkerIndex]->EnqueueTask(DedicatedTasks[_DedicatedTaskIndex]);
							_DedicatedTaskIndex = (_DedicatedTaskIndex + 1) % DedicatedTasks.size();
						}
					}
				}
			}
		}

		bool ParallelSystem::_HasAllDedicatedCore() const
		{
			return _DedicatedWorkersCount == _ParallelSystemCreateInformation.DedicatedTasks.size();
		}

		//////////////////////////////////////////////////////////////////////////
		// Scheduler

		ParallelSystem::Scheduler::Scheduler(_In_ uint32_t FrameCount)
		{
			_Frames.resize(FrameCount);
		}

		void ParallelSystem::Scheduler::StartFrame()
		{
			_CurrentFrame = (_CurrentFrame + 1) % _Frames.size();
		}

		void ParallelSystem::Scheduler::EndFrame()
		{

		}

		void ParallelSystem::Scheduler::RegisterTask(_In_ Task* InTask, _In_ const DependenciesType& Dependencies /* = DependenciesType() */)
		{
			_Frames[_CurrentFrame].Tasks.push_back({
				InTask, Dependencies
			});
		}

		bool ParallelSystem::Scheduler::OldestFrameHasPendingTasks() const
		{
			const Frame& OldestFrame = _Frames[(_CurrentFrame + 1) % _Frames.size()];
			return OldestFrame.Tasks.size() > 0;
		}

		Task* ParallelSystem::Scheduler::GetNextTask() const
		{
			const Frame& ExecutingFrame = _Frames[_ExecutingFrame];
			for (uint32_t TaskIndex = ExecutingFrame.FirstPendingTask; TaskIndex < ExecutingFrame.Tasks.size(); ++TaskIndex)
			{
				const TaskDependencies& CurrentDependencies = ExecutingFrame.Tasks[TaskIndex];

				bool CanExecuteTask = true;
				for (uint32_t DependencyIndex = 0; DependencyIndex < CurrentDependencies.Dependencies.size(); ++DependencyIndex)
				{
					Task* DependencyTask = CurrentDependencies.Dependencies[DependencyIndex];
					CanExecuteTask &= DependencyTask->IsDone();
				}
				if (CanExecuteTask)
					return CurrentDependencies.TaskToExecute;
			}
			return nullptr;
		}

		void ParallelSystem::Scheduler::AdvanceNextTask()
		{
			Frame& ExecutingFrame = _Frames[_ExecutingFrame];
			
			if (ExecutingFrame.Tasks.size() > 0)
			{
				++ExecutingFrame.ExecutedTasksCount;
				if (ExecutingFrame.ExecutedTasksCount == ExecutingFrame.Tasks.size())
				{
					ExecutingFrame.Reset();
					_ExecutingFrame = (_ExecutingFrame + 1) % _Frames.size();
					return;
				}

				for (uint32_t TaskIndex = ExecutingFrame.FirstPendingTask; TaskIndex < ExecutingFrame.Tasks.size(); ++TaskIndex)
				{
					TaskDependencies& CurrentDependencies = ExecutingFrame.Tasks[TaskIndex];

					bool CanExecuteTask = true;
					for (uint32_t DependencyIndex = 0; CanExecuteTask && DependencyIndex < CurrentDependencies.Dependencies.size(); ++DependencyIndex)
					{
						Task* DependencyTask = CurrentDependencies.Dependencies[DependencyIndex];
						CanExecuteTask &= DependencyTask->IsDone();
					}
					if (!CanExecuteTask)
					{
						ExecutingFrame.FirstPendingTask = TaskIndex;
						break;
					}
				}
			}
		}
	}
}
