#include "Log/LogFactory.hpp"

#include "Log/FileLog/FileLog.hpp"

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
			}
		}
	}
}
