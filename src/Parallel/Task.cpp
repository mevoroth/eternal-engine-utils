#include "Parallel/Task.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Parallel;

void Task::Schedule()
{
	ETERNAL_ASSERT(_TaskState == IDLE);
	SetState(SCHEDULED);
}

void Task::SetTaskName(const string& TaskName)
{
#ifdef ETERNAL_DEBUG
	_TaskName = TaskName;
#endif
}
