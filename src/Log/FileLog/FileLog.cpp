#include "Log/FileLog/FileLog.hpp"

#include <string>
#include "File/FileFactory.hpp"
#include "File/File.hpp"

using namespace std;
using namespace Eternal::Log;

FileLog::FileLog(_In_ const char* FileName)
	: Log()
{
	string Prefix(FileName);
	string Suffix = ".log";

	const Log::LogLevel* LogLevels[] = {
		&Info,
		&Warning,
		&Error,
		&Critical
	};

	for (int LogLevelIndex = 0; LogLevelIndex < Log::LogLevelCount; ++LogLevelIndex)
	{
		_LogFiles[LogLevelIndex] = CreateFileHandle(Prefix + string(".") + string(LogLevels[LogLevelIndex]->Tag) + Suffix);
		_LogFiles[LogLevelIndex]->Open(Eternal::File::File::WRITE);
	}

	_LogAllFile = CreateFileHandle(Prefix + Suffix);
	_LogAllFile->Open(Eternal::File::File::WRITE);

	Write(Info, Engine, "Start logging");
}

FileLog::~FileLog()
{
	_LogAllFile->Close();
	delete _LogAllFile;
	_LogAllFile = nullptr;

	for (int LogLevelIndex = 0; LogLevelIndex < Log::LogLevelCount; ++LogLevelIndex)
	{
		_LogFiles[LogLevelIndex]->Close();
		delete _LogFiles[LogLevelIndex];
		_LogFiles[LogLevelIndex] = nullptr;
	}
}

void FileLog::Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message)
{
	char HumanReadableTime[24];
	Eternal::Time::Time::ToHumanReadable(GetElaspedTime(), HumanReadableTime);
	string Temp = HumanReadableTime;
	Temp.append("[").append(Category.Category).append("]");
	Temp.append(Message);
	Temp.append("\n");
	_LogFiles[Level.Index]->Write((uint8_t*)Temp.c_str(), Temp.size());
	_LogAllFile->Write((uint8_t*)Temp.c_str(), Temp.size());
}
