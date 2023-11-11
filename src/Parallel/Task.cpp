#include "Parallel/Task.hpp"
#include "Parallel/AtomicS32.hpp"
#include "Parallel/AtomicS32Factory.hpp"

namespace Eternal
{
	namespace Parallel
	{
		static constexpr int IsTaskExecuting	= 1;
		static constexpr int IsTaskDone			= 0;

		Task::Task(_In_ const TaskCreateInformation& InTaskCreateInformation)
			: _TaskCreateInformation(InTaskCreateInformation)
			,  _IsExecuting(CreateAtomicS32())
		{
		}

		void Task::Execute()
		{
			ETERNAL_PROFILER(BASIC)(GetTaskName().c_str());
			_IsExecuting->Store(IsTaskExecuting);
			DoExecute();
			_IsExecuting->Store(IsTaskDone);
		}

		bool Task::IsDone() const
		{
			return _IsExecuting->Load() == IsTaskDone;
		}
	}
}
