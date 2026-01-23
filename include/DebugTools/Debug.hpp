#pragma once

namespace Eternal
{
	namespace DebugTools
	{
		void WaitForDebugger(bool BreakIfNotPresent = false);
		bool IsDebuggerPresent();
	}
}
