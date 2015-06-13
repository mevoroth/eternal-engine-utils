#include "Parallel/Task.hpp"

using namespace Eternal::Parallel;

void Task::SetFinished()
{
	_Finished = true;
}

bool Task::IsFinished() const
{
	return _Finished;
}
