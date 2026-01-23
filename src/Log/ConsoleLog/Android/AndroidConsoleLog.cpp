#if ETERNAL_PLATFORM_ANDROID

#include "Log/ConsoleLog/Android/AndroidConsoleLog.hpp"

#include <android/log.h>

namespace Eternal
{
	namespace LogSystem
	{
		void AndroidConsoleLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
		{
			android_LogPriority AndroidLogLevel = [&InLevel]() -> android_LogPriority
			{
				if (InLevel == LogInfo)
					return ANDROID_LOG_INFO;

				if (InLevel == LogWarning)
					return ANDROID_LOG_WARN;

				if (InLevel == LogError)
					return ANDROID_LOG_ERROR;

				if (InLevel == LogCritical)
					return ANDROID_LOG_FATAL;

				return ANDROID_LOG_FATAL;
			}();

			string FormattedLogMessage = FormatLogMessage(InLevel, InCategory, InMessage);
			__android_log_print(AndroidLogLevel, InCategory.Category, "%s", FormattedLogMessage.c_str());
		}
	}
}

#endif
