#include "Log/ConsoleLog/ConsoleLog.hpp"

#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace Eternal::Log;

void ConsoleLog::Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message)
{
	char HumanReadableTime[24];
	Eternal::Time::Time::ToHumanReadable(GetElaspedTime(), HumanReadableTime);
	string Temp = HumanReadableTime;
	Temp.append("[").append(Category.Category).append("]");
	Temp.append("[").append(Level.Tag).append("]");
	Temp.append(Message);
	OutputDebugString(Temp.c_str());
}
