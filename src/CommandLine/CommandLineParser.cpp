#include "CommandLine/CommandLineParser.hpp"

namespace Eternal
{
	namespace CommandLine
	{
		CommandLineParser::CommandLineParser(_In_ int InArgumentsCount, _In_ const char** InArgumentValues)
			: _ArgumentsCount(InArgumentsCount)
			, _ArgumentValues(InArgumentValues)
		{
		}

		string CommandLineParser::ParseCommandLine(_In_ const string& InCommand) const
		{
			string FormattedCommand = "-" + InCommand + "=";
			
			int CommandIndex = _FindCommandLine(FormattedCommand);
			if (CommandIndex >= _ArgumentsCount)
				return string();

			string CommandValue = _ArgumentValues[CommandIndex];
			return CommandValue.substr(FormattedCommand.length());
		}

		bool CommandLineParser::ContainsCommandLine(_In_ const string& InCommand) const
		{
			string FormattedCommand = "-" + InCommand;
			return _FindCommandLine(FormattedCommand) < _ArgumentsCount;
		}

		int CommandLineParser::_FindCommandLine(_In_ const string& InCommand) const
		{
			for (int CommandIndex = 1; CommandIndex < _ArgumentsCount; ++CommandIndex)
			{
				if (strncmp(InCommand.c_str(), _ArgumentValues[CommandIndex], InCommand.length()) == 0)
					return CommandIndex;
			}
			return _ArgumentsCount;
		}
	}
}
