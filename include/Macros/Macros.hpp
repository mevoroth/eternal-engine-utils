#pragma once

#ifdef ETERNAL_DEBUG

#if ETERNAL_PLATFORM_ANDROID
#define ETERNAL_BREAK()	\
	do	\
	{	\
		__builtin_trap();	\
	} while (false)
#else
#define ETERNAL_BREAK()	\
	do	\
	{	\
		__debugbreak();	\
	} while (false)
#endif

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

#define ETERNAL_FALLTHROUGH()					[[fallthrough]]
#define ETERNAL_ARRAYSIZE(Array)				(sizeof(Array)/sizeof(Array[0]))
#define ETERNAL_OFFSETOF(Structure, Member)		((unsigned long long)&(((Structure*)nullptr)->Member))
#define __ETERNAL_STRINGIFY_INTERNAL(Macro)		#Macro
#define ETERNAL_STRINGIFY(Macro)				__ETERNAL_STRINGIFY_INTERNAL(Macro)

//////////////////////////////////////////////////////////////////////////
// Profiling
#define ETERNAL_PROFILER(LEVEL)					ETERNAL_PROFILER_ ## LEVEL

#ifndef ETERNAL_PROFILER_USE_BASIC
#define ETERNAL_PROFILER_USE_BASIC				(ETERNAL_DEBUG && 1)
#endif

#ifndef ETERNAL_PROFILER_USE_DETAIL
#define ETERNAL_PROFILER_USE_DETAIL				(ETERNAL_DEBUG && 1)
#endif

#ifndef ETERNAL_PROFILER_USE_INFO
#define ETERNAL_PROFILER_USE_INFO				(ETERNAL_DEBUG && 1)
#endif

#ifndef ETERNAL_PROFILER_USE_VERBOSE
#define ETERNAL_PROFILER_USE_VERBOSE			(ETERNAL_DEBUG && 0)
#endif

#if ETERNAL_PROFILER_USE_BASIC
	#define ETERNAL_PROFILER_BASIC(...)			OPTICK_EVENT(__VA_ARGS__);
#else
	#define ETERNAL_PROFILER_BASIC(...)
#endif

#if ETERNAL_PROFILER_USE_DETAIL
	#define ETERNAL_PROFILER_DETAIL(...)		OPTICK_EVENT(__VA_ARGS__);
#else
	#define ETERNAL_PROFILER_DETAIL(...)
#endif

#if ETERNAL_PROFILER_USE_INFO
	#define ETERNAL_PROFILER_INFO(...)			OPTICK_EVENT(__VA_ARGS__);
#else
	#define ETERNAL_PROFILER_INFO(...)
#endif

#if ETERNAL_PROFILER_USE_VERBOSE
	#define ETERNAL_PROFILER_VERBOSE(...)		OPTICK_EVENT(__VA_ARGS__);
#else
	#define ETERNAL_PROFILER_VERBOSE(...)
#endif
