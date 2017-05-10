#ifndef _LOG_HPP_
#define _LOG_HPP_

#include "Time/Time.hpp"
#include <string>

using namespace std;

namespace Eternal
{
	namespace Log
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
			static Log* _Inst;

			TimeMicroSecondsT _InitialTime = 0ull;
		};
	}
}

#endif
