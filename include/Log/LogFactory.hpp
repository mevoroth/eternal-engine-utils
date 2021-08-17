#pragma once

#include <vector>

namespace Eternal
{
	namespace LogSystem
	{
		using namespace std;

		class Log;

		enum class LogType
		{
			LOG_TYPE_FILE,
			LOG_TYPE_CONSOLE,
			LOG_TYPE_MULTI_CHANNEL
		};

		Log* CreateLog(_In_ const LogType& InLogType, _In_ const char* InSession = nullptr);
		Log* CreateMultiChannelLog(_In_ const vector<LogType>& InLogTypes);

		void DestroyLog(_Inout_ Log*& InLog);
	}
}
