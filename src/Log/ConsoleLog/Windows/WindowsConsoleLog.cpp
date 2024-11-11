#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Log/ConsoleLog/Windows/WindowsConsoleLog.hpp"

#include <Windows.h>

namespace Eternal
{
	namespace LogSystem
	{
		WindowsConsoleLog::WindowsConsoleLog()
			: Log()
		{
		}

		void WindowsConsoleLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
		{
			char HumanReadableTime[24];
			Eternal::Time::Timer::ToHumanReadable(GetElaspedTime(), HumanReadableTime);
			string Temp = HumanReadableTime;
			Temp.append("[").append(InCategory.Category).append("]");
			Temp.append("[").append(InLevel.Tag).append("]");
			Temp.append(InMessage);
			Temp.append("\n");
			OutputDebugString(Temp.c_str());
		}
	}
}

#endif
