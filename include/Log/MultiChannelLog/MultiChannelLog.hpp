#ifndef _MULTI_CHANNEL_LOG_HPP_
#define _MULTI_CHANNEL_LOG_HPP_

#include "Log/Log.hpp"

namespace Eternal
{
	namespace Log
	{
		using namespace std;

		class MultiChannelLog : public Log
		{
		public:
			MultiChannelLog();
			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) override;
			
			void Add(_In_ Log* LogObj);

		private:
			vector<Log*> _Channels;
		};
	}
}

#endif
