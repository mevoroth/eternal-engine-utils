#ifndef _MACROS_HPP_
#define _MACROS_HPP_

#ifdef ETERNAL_DEBUG
#define ETERNAL_BREAK()	__debugbreak()
#else
#define ETERNAL_BREAK()
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

//////////////////////////////////////////////////////////////////////////
// Profiling
#define ETERNAL_PROFILER(LEVEL)					ETERNAL_PROFILER_ ## LEVEL

#if ETERNAL_DEBUG || 1
#define ETERNAL_PROFILER_INFO(...)				OPTICK_EVENT(__VA_ARGS__)
#define ETERNAL_PROFILER_DETAIL(...)			OPTICK_EVENT(__VA_ARGS__)
#define ETERNAL_PROFILER_BASIC(...)				OPTICK_EVENT(__VA_ARGS__)
#else
#define ETERNAL_PROFILER_INFO(...)
#define ETERNAL_PROFILER_DETAIL(...)
#define ETERNAL_PROFILER_BASIC(...)
#endif

#endif
