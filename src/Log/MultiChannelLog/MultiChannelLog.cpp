#include "Log/MultiChannelLog/MultiChannelLog.hpp"
#include "Log/LogFactory.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		MultiChannelLog::MultiChannelLog(_In_ const vector<LogType>& InLogTypes)
			: Log()
		{
			_Channels.resize(InLogTypes.size());
			for (uint32_t LogTypeIndex = 0; LogTypeIndex < InLogTypes.size(); ++LogTypeIndex)
				_Channels[LogTypeIndex] = CreateLog(InLogTypes[LogTypeIndex]);

			Initialize(this);
		}

		MultiChannelLog::~MultiChannelLog()
		{
			for (uint32_t ChannelIndex = 0; ChannelIndex < _Channels.size(); ++ChannelIndex)
				DestroyLog(_Channels[ChannelIndex]);
		}

		void MultiChannelLog::Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage)
		{
			for (uint32_t ChannelIndex = 0; ChannelIndex < _Channels.size(); ++ChannelIndex)
				_Channels[ChannelIndex]->Write(InLevel, InCategory, InMessage);
		}
	}
}
