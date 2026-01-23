#if ETERNAL_PLATFORM_ANDROID

#include "DebugTools/Debug.hpp"

namespace Eternal
{
	namespace DebugTools
	{
		bool IsDebuggerPresent()
		{
			return false;
		}
	}
}

#endif
