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
			FileLog(_In_ const char* FileName);
			~FileLog();
			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) override;

		private:
			File* _LogFiles[LogLevelCount];
			File* _LogAllFile = nullptr;
		};
	}
}
