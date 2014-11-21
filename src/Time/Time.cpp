#include "Time/Time.hpp"

#include <cassert>

using namespace Eternal::Time;

Time* Time::_inst = 0;

Time::Time()
{
	assert(!_inst);
	_inst = this;
}
