#pragma once

#include "Time/Timer.hpp"
#include <string>

using namespace std;

namespace Eternal
{
	namespace Time
	{
		using TimeMicroSecondsT	= uint64_t;
	}
	namespace LogSystem
	{
		using namespace Eternal::Time;

		class Log
		{
		public:
			class LogLevel
			{
			public:
				LogLevel(_In_ int Index, _In_ const char* Tag)
					: Index(Index)
					, Tag(Tag)
				{}
				int Index;
				const char* Tag = nullptr;
			};
			static constexpr int LogLevelCount = 4;
			static const LogLevel Info;
			static const LogLevel Warning;
			static const LogLevel Error;
			static const LogLevel Critical;

			class LogCategory
			{
			public:
				LogCategory(_In_ const char* Category)
					: Category(Category)
				{}
				const char* Category = nullptr;
			};

			static const LogCategory Graphics;
			static const LogCategory Engine;
			static const LogCategory Save;
			static const LogCategory Import;

			static void Initialize(Log* LogObj);
			static Log* Get();

			Log();
			virtual ~Log() {}

			virtual void Write(_In_ const LogLevel& Level, _In_ const LogCategory& Category, _In_ const string& Message) = 0;
			TimeMicroSecondsT GetElaspedTime() const;

		private:
			static Log* _Instance;

			TimeMicroSecondsT _InitialTime = 0ull;
		};
	}

	// Alias
	extern const LogSystem::Log::LogLevel& LogInfo;
	extern const LogSystem::Log::LogLevel& LogWarning;
	extern const LogSystem::Log::LogLevel& LogError;
	extern const LogSystem::Log::LogLevel& LogCritical;

	extern const LogSystem::Log::LogCategory& LogGraphics;
	extern const LogSystem::Log::LogCategory& LogEngine;
	extern const LogSystem::Log::LogCategory& LogSave;
	extern const LogSystem::Log::LogCategory& LogImport;

	void LogWrite(_In_ const LogSystem::Log::LogLevel& Level, _In_ const LogSystem::Log::LogCategory& Category, _In_ const string& Message);
}
