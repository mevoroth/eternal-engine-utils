#pragma once

#include "Log/Log.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		class File;
	}

	namespace LogSystem
	{
		using namespace Eternal::FileSystem;

		class FileLog final : public Log
		{
		public:
			FileLog(_In_ const char* InFileName);
			~FileLog();
			virtual void Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage) override;

		private:
			File* _LogFiles[LogLevelCount];
			File* _LogAllFile = nullptr;
		};
	}
}
