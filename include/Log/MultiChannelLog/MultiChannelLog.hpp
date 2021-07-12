#pragma once

#include "Log/Log.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		using namespace std;

		enum class LogType;

		class MultiChannelLog final : public Log
		{
		public:
			MultiChannelLog(_In_ const vector<LogType>& InLogTypes);
			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) override;
			
		private:
			vector<Log*> _Channels;
		};
	}
}
