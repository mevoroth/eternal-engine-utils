#pragma once

#include "Log/Log.hpp"
#include "Time/Timer.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		using namespace Eternal::Time;

		class ConsoleLog final : public Log
		{
		public:
			ConsoleLog();
			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) override final;
		};
	}
}
