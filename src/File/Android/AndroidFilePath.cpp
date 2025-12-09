#if ETERNAL_PLATFORM_ANDROID

#include "File/FilePath.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		void FilePath::SplitPath(_In_ const string& InFilePath, _Out_ string& OutDirectory, _Out_ string& OutFileName, _Out_ string& OutExtension)
		{
			if (InFilePath.length() == 0)
				return;

			char Directory[255] = {};
			char FileName[255] = {};
			char Extension[8] = {};

			size_t ExtensionOffset = InFilePath.rfind('.');
			size_t FileNameOffset = InFilePath.rfind('/', ExtensionOffset);

			OutDirectory = InFilePath.substr(0ull, FileNameOffset + 1);
			OutFileName = InFilePath.substr(FileNameOffset + 1, ExtensionOffset - FileNameOffset - 1);
			OutExtension = &InFilePath[ExtensionOffset];
		}

		string FilePath::CurrentDirectory()
		{
			return string();
		}

		string FilePath::MakePath(_In_ const string& InFilePath)
		{
			return /*string("/app0/") +*/ InFilePath;
		}
	}
}

#endif
