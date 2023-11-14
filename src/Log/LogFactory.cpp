#include "Log/LogFactory.hpp"

#include "Log/MultiChannelLog/MultiChannelLog.hpp"
#include "Log/FileLog/FileLog.hpp"
#if ETERNAL_PLATFORM_WINDOWS
#include "Log/ConsoleLog/WindowsConsoleLog.hpp"
#endif
#if ETERNAL_USE_PRIVATE
#include "Log/ConsoleLog/ConsoleLogFactoryPrivate.hpp"
#endif

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
#if ETERNAL_PLATFORM_WINDOWS
				return new WindowsConsoleLog();
#endif
#if ETERNAL_USE_PRIVATE
				return CreateConsoleLogPrivate();
#endif

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

		void DestroyLog(_Inout_ Log*& InLog)
		{
			delete InLog;
			InLog = nullptr;
		}
	}
}
