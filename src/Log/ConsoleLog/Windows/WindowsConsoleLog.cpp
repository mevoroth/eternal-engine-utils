#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Log/ConsoleLog/Windows/WindowsConsoleLog.hpp"

#include <Windows.h>

namespace Eternal
{
	namespace LogSystem
	{
		void WindowsConsoleLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
		{
			string FormattedLogMessage = FormatLogMessage(InLevel, InCategory, InMessage);
			OutputDebugString(FormattedLogMessage.c_str());
		}
	}
}

#endif
