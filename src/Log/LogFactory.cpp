#include "Log/LogFactory.hpp"

#include "Log/FileLog/FileLog.hpp"
#include "Log/ConsoleLog/ConsoleLog.hpp"
#include "Log/MultiChannelLog/MultiChannelLog.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		Log* CreateLog(_In_ const LogType& InLogType, _In_ const char* InSession)
		{
			switch (InLogType)
			{
			case LogType::LOG_TYPE_FILE:
				return new FileLog(InSession);

			case LogType::LOG_TYPE_CONSOLE:
				return new ConsoleLog();

			case LogType::LOG_TYPE_MULTI_CHANNEL:
				ETERNAL_BREAK(); // Multi Channel Log must be created with CreateMultiChannelLog()
				break;
			}
			return nullptr;
		}

		Log* CreateMultiChannelLog(_In_ const vector<LogType>& InLogTypes)
		{
			return new MultiChannelLog(InLogTypes);
		}

		void DeleteLog(Log*& LogObj)
		{
			delete LogObj;
			LogObj = nullptr;
		}
	}
}
