#if ETERNAL_PLATFORM_WINDOWS

#include "DebugTools/Debug.hpp"

#include <Windows.h>

namespace Eternal
{
	namespace DebugTools
	{
		void WaitForDebugger(bool BreakIfNotPresent)
		{
			//return;
			while (!IsDebuggerPresent())
			{
				if (BreakIfNotPresent)
				{
					ETERNAL_BREAK();
				}
			}
		}
	}
}

#endif
