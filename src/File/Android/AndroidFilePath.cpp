#if ETERNAL_PLATFORM_ANDROID

#include "File/FilePath.hpp"
#include <algorithm>

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
			static const string Prefixes[] =
			{
				"assets\\",
				"eternal-engine-shaders\\",
			};

			string NormalizedPath = InFilePath;
			for (uint32_t PrefixIndex = 0; PrefixIndex < ETERNAL_ARRAYSIZE(Prefixes); ++PrefixIndex)
			{
				if (!NormalizedPath.compare(0, Prefixes[PrefixIndex].length(), Prefixes[PrefixIndex]))
				{
					NormalizedPath = NormalizedPath.substr(Prefixes[PrefixIndex].length());
					break;
				}
			}

			std::replace(NormalizedPath.begin(), NormalizedPath.end(), '\\', '/');

			return NormalizedPath;
		}
	}
}

#endif
