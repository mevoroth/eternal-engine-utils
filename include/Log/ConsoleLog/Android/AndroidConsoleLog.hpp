#pragma once

#include "Log/Log.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		class AndroidConsoleLog : public Log
		{
		public:

			using Log::Log;

			virtual void Write(_In_ const LogLevel& InLevel, _In_ const LogCategory& InCategory, _In_ const string& InMessage) override final;

		};
	}
}
