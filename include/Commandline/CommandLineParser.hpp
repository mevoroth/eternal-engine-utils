#pragma once

#include <string>

namespace Eternal
{
	namespace CommandLine
	{
		using namespace std;

		class CommandLineParser
		{
		public:

			CommandLineParser(_In_ int InArgumentsCount, _In_ const char** InArgumentValues);

			string ParseCommandLine(_In_ const string& InCommand) const;
			bool ContainsCommandLine(_In_ const string& InCommand) const;

		private:

			int _FindCommandLine(_In_ const string& InCommand) const;

			int _ArgumentsCount = 0;
			const char** _ArgumentValues;

		};
	}
}
