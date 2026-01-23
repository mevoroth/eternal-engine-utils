#include "DebugTools/Debug.hpp"

namespace Eternal
{
	namespace DebugTools
	{
		void WaitForDebugger(bool BreakIfNotPresent /* = false */)
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
