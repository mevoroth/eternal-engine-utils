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

#ifndef ETERNAL_PROFILER_USE_BASIC
#define ETERNAL_PROFILER_USE_BASIC (1)
#endif

#ifndef ETERNAL_PROFILER_USE_DETAIL
#define ETERNAL_PROFILER_USE_DETAIL (1)
#endif

#ifndef ETERNAL_PROFILER_USE_INFO
#define ETERNAL_PROFILER_USE_INFO (0)
#endif

#ifndef ETERNAL_PROFILER_USE_VERBOSE
#define ETERNAL_PROFILER_USE_VERBOSE (0)
#endif

#if ETERNAL_DEBUG || 1
#define ETERNAL_PROFILER_BASIC(...)				if (ETERNAL_PROFILER_USE_BASIC)		{ OPTICK_EVENT(__VA_ARGS__); }
#define ETERNAL_PROFILER_DETAIL(...)			if (ETERNAL_PROFILER_USE_DETAIL)	{ OPTICK_EVENT(__VA_ARGS__); }
#define ETERNAL_PROFILER_INFO(...)				if (ETERNAL_PROFILER_USE_INFO)		{ OPTICK_EVENT(__VA_ARGS__); }
#define ETERNAL_PROFILER_VERBOSE(...)			if (ETERNAL_PROFILER_USE_VERBOSE)	{ OPTICK_EVENT(__VA_ARGS__); }
#else
#define ETERNAL_PROFILER_BASIC(...)
#define ETERNAL_PROFILER_DETAIL(...)
#define ETERNAL_PROFILER_INFO(...)
#define ETERNAL_PROFILER_VERBOSE(...)
#endif

#endif
