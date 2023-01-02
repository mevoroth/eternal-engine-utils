#pragma once

#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		class File;

		struct FileContent
		{
			uint8_t* Content	= nullptr;
			uint64_t Size		= 0ull;

			~FileContent();
		};

		File* CreateFileHandle(_In_ const std::string& InFileName);
		void DestroyFileHandle(_Inout_ File*& InFile);
		bool FileExists(_In_ const std::string& InFileName);
		FileContent LoadFileToMemory(_In_ const std::string& InFileName);
		void UnloadFileFromMemory(_Inout_ FileContent& InOutFileContent);
	}
}
