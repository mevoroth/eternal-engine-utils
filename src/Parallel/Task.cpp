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
	_InstanceID = 0;
}

int Task::GetInstanceCount() const
{
	return _InstanceCount;
}

void Task::Setup()
{
	ETERNAL_ASSERT(GetState() == SCHEDULED);
	ETERNAL_ASSERT(_InstanceID < _InstanceCount);
	SetState(SETTINGUP);
	DoSetup();
	SetState(SETUP);
}

void Task::Reset()
{
	ETERNAL_ASSERT(GetState() == DONE);
	DoReset();
	_InstanceID = 0;
	SetState(IDLE);
}

void Task::Execute()
{
	ETERNAL_ASSERT(GetState() == SETUP);
	SetState(EXECUTING);
	DoExecute();

	++_InstanceID;
	if (_InstanceID < _InstanceCount)
		SetState(IDLE);
	else
		SetState(DONE);
}

int Task::GetInstanceID() const
{
	ETERNAL_ASSERT(GetState() != IDLE);
	return _InstanceID;
}
