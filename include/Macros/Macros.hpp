#ifndef _MACROS_HPP_
#define _MACROS_HPP_

#ifdef ETERNAL_DEBUG
#define ETERNAL_ASSERT(Condition)	if (!(Condition)) { __debugbreak(); }
#else
#define ETERNAL_ASSERT(Condition)
#endif

#define ETERNAL_ARRAYSIZE(Array)	(sizeof(Array)/sizeof(Array[0]))

#endif
