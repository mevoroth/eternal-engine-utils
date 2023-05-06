#include "Log/ConsoleLog/ConsoleLog.hpp"

#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eternal
{
	namespace LogSystem
	{
		ConsoleLog::ConsoleLog()
			: Log()
		{
		}

		void ConsoleLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
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
