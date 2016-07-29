#ifndef _CONSOLE_LOG_HPP_
#define _CONSOLE_LOG_HPP_

#include "Log/Log.hpp"
#include "Time/Time.hpp"

namespace Eternal
{
	namespace Log
	{
		using namespace Eternal::Time;

		class ConsoleLog : public Log
		{
		public:
			ConsoleLog() : Log() {}
			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) override;
		};
	}
}

#endif
