#pragma once

#include "Log/Log.hpp"
#include "Time/Timer.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		using namespace Eternal::Time;

		class WindowsConsoleLog final : public Log
		{
		public:

			using Log::Log;
			
			virtual void Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage) override final;
	
		};
	}
}
