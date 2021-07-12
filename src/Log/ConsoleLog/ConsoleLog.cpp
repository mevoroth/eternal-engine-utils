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

		void ConsoleLog::Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message)
		{
			char HumanReadableTime[24];
			Eternal::Time::Timer::ToHumanReadable(GetElaspedTime(), HumanReadableTime);
			string Temp = HumanReadableTime;
			Temp.append("[").append(Category.Category).append("]");
			Temp.append("[").append(Level.Tag).append("]");
			Temp.append(Message);
			Temp.append("\n");
			OutputDebugString(Temp.c_str());
		}
	}
}
