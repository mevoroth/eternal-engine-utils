#pragma once

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
#include <Windows.h>
#endif
#if ETERNAL_PLATFORM_ANDROID
#include <time.h>
#endif
#if ETERNAL_USE_PRIVATE
#include "Time/TimeHeaderPrivate.hpp"
#endif

namespace Eternal
{
	namespace Time
	{
#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
		using TimeInternalType		= LARGE_INTEGER;
		using FrequencyInternalType	= LARGE_INTEGER;
#endif
#if ETERNAL_PLATFORM_ANDROID
		using TimeInternalType		= timespec;
		using FrequencyInternalType	= uint64_t;
#endif
	}
}
