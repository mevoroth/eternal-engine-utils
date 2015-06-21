#ifndef _MACROS_HPP_
#define _MACROS_HPP_

#define ETERNAL_ASSERT(Condition)	if (!Condition) { __debugbreak(); }

#define ETERNAL_ARRAYSIZE(Array)	_countof(Array)

#endif
