#include "Parallel/Task.hpp"

#include "Parallel/StdAtomicInt.hpp"

using namespace Eternal::Parallel;

Task::Task()
{
	_RefCount = new StdAtomicInt(0);
}

Task::~Task()
{
	delete _RefCount;
	_RefCount = nullptr;
}

void Task::SetFinished()
{
	_Finished = true;
}

bool Task::IsFinished() const
{
	return _Finished;
}
