#include "Parallel/Task.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Parallel;

Task::Task()
{
}

Task::~Task()
{
}

bool Task::TaskIsExecutable()
{
	bool AllExecuted = true;
	for (uint32_t TaskIndex = 0; AllExecuted && TaskIndex < _Dependencies.size(); ++TaskIndex)
	{
		AllExecuted &= _Dependencies[TaskIndex]->TaskIsExecuted();
	}
	return AllExecuted;
}

void Task::DependsOn(Task* TaskObj)
{
	ETERNAL_ASSERT(TaskObj != this);
#ifdef ETERNAL_DEBUG
	for (uint32_t TaskIndex = 0; TaskIndex < _Dependencies.size(); ++TaskIndex)
	{
		ETERNAL_ASSERT(_Dependencies[TaskIndex] != this);
	}
#endif

	_Dependencies.push_back(TaskObj);
}
