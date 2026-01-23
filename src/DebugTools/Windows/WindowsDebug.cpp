#if ETERNAL_PLATFORM_WINDOWS

#include "DebugTools/Debug.hpp"

#include <Windows.h>

namespace Eternal
{
	namespace DebugTools
	{
		bool IsDebuggerPresent()
		{
			return ::IsDebuggerPresent();
		}
	}
}

#endif
