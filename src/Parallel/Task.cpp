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

void Task::SetFrameConstraint(_In_ bool FrameConstraint)
{
	_FrameConstraint = FrameConstraint;
}

bool Task::GetFrameConstraint() const
{
	return _FrameConstraint;
}

void Task::SetInstanceCount(_In_ int InstanceCount)
{
	ETERNAL_ASSERT(GetState() == IDLE);
	_InstanceCount = InstanceCount;
	_RemainingInstanceCount = InstanceCount;
}

int Task::GetInstanceCount() const
{
	return _InstanceCount;
}

void Task::Setup()
{
	ETERNAL_ASSERT(GetState() == SCHEDULED);
	SetState(SETTINGUP);
	--_RemainingInstanceCount;
	DoSetup();
	SetState(SETUP);
}

void Task::Reset()
{
	ETERNAL_ASSERT(GetState() == DONE);
	DoReset();
	_RemainingInstanceCount = _InstanceCount;
	SetState(IDLE);
}

void Task::Execute()
{
	ETERNAL_ASSERT(GetState() == SETUP);
	SetState(EXECUTING);
	DoExecute();

	if (_RemainingInstanceCount)
		SetState(SCHEDULED);
	else
		SetState(DONE);
}
