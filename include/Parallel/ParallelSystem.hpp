#pragma once

#include <string>
#include <vector>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class Worker;
		class Task;
		class Mutex;

		struct ParallelSystemCreateInformation
		{
			vector<Task*> DedicatedTasks;
			uint32_t FrameCount = 0;
		};

		using DependenciesType = vector<Task*>;

		class ParallelSystem
		{
		public:
			ParallelSystem(_In_ const ParallelSystemCreateInformation& InParallelSystemCreateInformation);
			~ParallelSystem();

			void StartFrame();
			void ExecuteTask(_In_ Task* InTask, _In_ const DependenciesType& Dependencies = DependenciesType());
			void EndFrame();

		private:
			class Scheduler
			{
			public:
				Scheduler(_In_ uint32_t FrameCount);

				void StartFrame();
				void RegisterTask(_In_ Task* InTask, _In_ const DependenciesType& Dependencies = DependenciesType());
				void EndFrame();

				bool OldestFrameHasPendingTasks() const;
				void AdvanceNextTask();
				Task* GetNextTask() const;

			private:
				struct TaskDependencies
				{
					Task*				TaskToExecute = nullptr;
					DependenciesType	Dependencies;
				};

				struct Frame
				{
					vector<TaskDependencies> Tasks;
					uint32_t FirstPendingTask	= 0;
					uint32_t ExecutedTasksCount	= 0;

					void Reset()
					{
						Tasks.clear();
						FirstPendingTask	= 0;
						ExecutedTasksCount	= 0;
					}
				};

				uint32_t		_CurrentFrame	= 0;
				uint32_t		_ExecutingFrame	= 0;
				vector<Frame>	_Frames;
			};

			void _FlushPendingTasks();
			bool _HasAllDedicatedCore() const;

			ParallelSystemCreateInformation	_ParallelSystemCreateInformation;
			vector<Worker*>					_Workers;
			Scheduler						_Scheduler;
			int32_t							_CPUCount = 0;
			int32_t							_UsedCPUCount = 0;
			int32_t							_GenericWorkersCount = 0;
			int32_t							_DedicatedWorkersCount = 0;
			uint32_t						_DedicatedTaskIndex = 0;
		};
	}
}
