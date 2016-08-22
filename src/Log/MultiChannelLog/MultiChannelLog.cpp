#include "Log/MultiChannelLog/MultiChannelLog.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Log;

MultiChannelLog::MultiChannelLog()
	: Log()
{
	Initialize(this);
}

void MultiChannelLog::Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message)
{
	for (int ChannelIndex = 0; ChannelIndex < _Channels.size(); ++ChannelIndex)
	{
		_Channels[ChannelIndex]->Write(Level, Category, Message);
	}
}

void MultiChannelLog::Add(_In_ Log* LogObj)
{
	ETERNAL_ASSERT(LogObj);
	_Channels.push_back(LogObj);
}
