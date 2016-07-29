#ifndef _FILE_LOG_HPP_
#define _FILE_LOG_HPP_

#include "Log/Log.hpp"

namespace Eternal
{
	namespace File
	{
		class File;
	}

	namespace Log
	{
		using namespace Eternal::File;

		class FileLog : public Log
		{
		public:
			FileLog(_In_ const char* FileName);
			~FileLog();
			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) override;

		private:
			Eternal::File::File* _LogFiles[LogLevelCount];
			Eternal::File::File* _LogAllFile = nullptr;
		};
	}
}

#endif
