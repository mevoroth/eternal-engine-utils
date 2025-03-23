#include "Log/FileLog/FileLog.hpp"

#include <string>
#include "File/FileFactory.hpp"
#include "File/File.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		using namespace FileSystem;

		FileLog::FileLog(_In_ const char* InFileName)
			: Log()
		{
			string Prefix(InFileName ? InFileName : "Eternal");
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
				_LogFiles[LogLevelIndex]->Open(FileOpenMode::FILE_OPEN_MODE_WRITE);
			}

			_LogAllFile = CreateFileHandle(Prefix + Suffix);
			_LogAllFile->Open(FileOpenMode::FILE_OPEN_MODE_WRITE);

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
				DestroyFileHandle(_LogFiles[LogLevelIndex]);
			}
		}

		void FileLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
		{
			char HumanReadableTime[24];
			Eternal::Time::Timer::ToHumanReadable(GetElaspedTime(), HumanReadableTime);
			string Temp = HumanReadableTime;
			Temp.append("[").append(InCategory.Category).append("]");
			Temp.append(InMessage);
			Temp.append("\n");
			_LogFiles[InLevel.Index]->Write((uint8_t*)Temp.c_str(), Temp.size());
			_LogAllFile->Write((uint8_t*)Temp.c_str(), Temp.size());
		}
	}
}
