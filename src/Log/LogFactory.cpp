#include "Log/LogFactory.hpp"

#include "Log/FileLog/FileLog.hpp"
#include "Log/ConsoleLog/ConsoleLog.hpp"
#include "Log/MultiChannelLog/MultiChannelLog.hpp"

namespace Eternal
{
	namespace Log
	{
		Log* CreateLog(_In_ const LogType& LogTypeObj, _In_ const char* Session)
		{
			switch (LogTypeObj)
			{
			case FILE:
				return new FileLog(Session);
				break;

			case CONSOLE:
				return new ConsoleLog();
				break;

			case MULTI_CHANNEL:
				ETERNAL_BREAK(); // Multi Channel Log must be created with CreateMultiChannelLog()
				break;
			}
			return nullptr;
		}

		Log* CreateMultiChannelLog(_In_ Log** Logs, _In_ int LogsCount)
		{
			MultiChannelLog* MultiChannelLogObj = new MultiChannelLog();

			for (int LogIndex = 0; LogIndex < LogsCount; ++LogIndex)
			{
				MultiChannelLogObj->Add(Logs[LogIndex]);
			}

			return MultiChannelLogObj;
		}

		void DeleteLog(Log*& LogObj)
		{
			delete LogObj;
			LogObj = nullptr;
		}
	}
}
