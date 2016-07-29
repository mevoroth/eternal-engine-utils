#ifndef _LOG_FACTORY_HPP_
#define _LOG_FACTORY_HPP_

namespace Eternal
{
	namespace Log
	{
		class Log;

		enum LogType
		{
			FILE
		};

		Log* CreateLog(_In_ const LogType& LogTypeObj, _In_ const char* Session);
	}
}

#endif
