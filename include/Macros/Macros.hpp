#pragma once

#if ETERNAL_PLATFORM_ANDROID
#define __debugbreak()	__builtin_debugtrap()
#endif

#ifdef ETERNAL_DEBUG
#define ETERNAL_BREAK()	\
	do	\
	{	\
		__debugbreak();	\
	} while (false)
#else
#define ETERNAL_BREAK()
#endif

#ifdef ETERNAL_DEBUG
#define ETERNAL_ASSERT(Condition)	if (!(Condition)) { ETERNAL_BREAK(); }
#else
#define ETERNAL_ASSERT(Condition)	(void)(Condition)
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
#define ETERNAL_STRINGIFY_MACRO(Macro)			#Macro

//////////////////////////////////////////////////////////////////////////
// Platform
#if ETERNAL_PLATFORM_WINDOWS
#define ETERNAL_PLATFORM_NAME					ETERNAL_STRINGIFY_MACRO(ETERNAL_PLATFORM_WINDOWS)
#endif

#if ETERNAL_PLATFORM_PROSPERO
#define ETERNAL_PLATFORM_NAME					ETERNAL_STRINGIFY_MACRO(ETERNAL_PLATFORM_PROSPERO)
#endif

#if ETERNAL_PLATFORM_SCARLETT
#define											ETERNAL_STRINGIFY_MACRO(ETERNAL_PLATFORM_SCARLETT)
#endif

#if ETERNAL_PLATFORM_ANDROID
#define ETERNAL_PLATFORM_NAME					ETERNAL_STRINGIFY_MACRO(ETERNAL_PLATFORM_ANDROID)
#endif

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
