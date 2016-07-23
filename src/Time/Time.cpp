#include "Time/Time.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Time;

Time* Time::_Inst = nullptr;

Time::Time()
{
	ETERNAL_ASSERT(!_Inst);
	_Inst = this;
}
