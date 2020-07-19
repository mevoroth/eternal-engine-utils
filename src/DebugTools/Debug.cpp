#include "DebugTools/Debug.hpp"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

namespace Eternal
{
	namespace DebugTools
	{
		void WaitForDebugger(bool BreakIfNotPresent)
		{
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
