#ifndef _MACROS_HPP_
#define _MACROS_HPP_

#ifdef ETERNAL_DEBUG
#define ETERNAL_BREAK()	__debugbreak()
#endif

#ifdef ETERNAL_DEBUG
#define ETERNAL_ASSERT(Condition)	if (!(Condition)) { ETERNAL_BREAK(); }
#else
#define ETERNAL_ASSERT(Condition)
#endif

#ifdef ETERNAL_DEBUG
#define ETERNAL_STATIC_ASSERT(Condition, ErrorString)	static_assert((Condition), ErrorString)
#else
#define ETERNAL_STATIC_ASSERT(Condition, ErrorString)
#endif

#define ETERNAL_ARRAYSIZE(Array)				(sizeof(Array)/sizeof(Array[0]))
#define ETERNAL_OFFSETOF(Structure, Member)		((unsigned long long)&(((Structure*)nullptr)->Member))

#endif
