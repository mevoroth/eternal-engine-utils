#ifndef _LOG_FACTORY_HPP_
#define _LOG_FACTORY_HPP_

namespace Eternal
{
	namespace Log
	{
		class Log;

		enum LogType
		{
			FILE,
			CONSOLE,
			MULTI_CHANNEL
		};

		Log* CreateLog(_In_ const LogType& LogTypeObj, _In_ const char* Session);
		Log* CreateMultiChannelLog(_In_ Log** Logs, _In_ int LogsCount);
	}
}

#endif
