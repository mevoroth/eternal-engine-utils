#include "Log/MultiChannelLog/MultiChannelLog.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		MultiChannelLog::MultiChannelLog(_In_ const vector<LogType>& InLogTypes)
			: Log()
		{
			Initialize(this);
		}

		void MultiChannelLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
		{
			for (int ChannelIndex = 0; ChannelIndex < _Channels.size(); ++ChannelIndex)
			{
				_Channels[ChannelIndex]->Write(InLevel, InCategory, InMessage);
			}
		}
	}
}
