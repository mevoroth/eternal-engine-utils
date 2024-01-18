#if ETERNAL_PLATFORM_WINDOWS

#include "File/FilePath.hpp"
#include <cstdlib>

#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

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

			_splitpath_s(
				InFilePath.c_str(),
				nullptr, 0ull,
				Directory, ETERNAL_ARRAYSIZE(Directory),
				FileName, ETERNAL_ARRAYSIZE(FileName),
				Extension, ETERNAL_ARRAYSIZE(Extension)
			);

			OutDirectory = Directory;
			OutFileName = FileName;
			OutExtension = Extension;
		}

		string FilePath::CurrentDirectory()
		{
			char CurrentDirectoryString[1024];
			DWORD Result = GetCurrentDirectory(ETERNAL_ARRAYSIZE(CurrentDirectoryString), CurrentDirectoryString);
			ETERNAL_ASSERT(Result > 0);
			return CurrentDirectoryString;
		}
	}
}

#endif
