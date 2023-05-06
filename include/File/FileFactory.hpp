#pragma once

#include "File/File.hpp"
#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		struct FileContent
		{
			uint8_t* Content	= nullptr;
			uint64_t Size		= 0ull;

			~FileContent();
		};

		class FileScope
		{
		public:

			FileScope(_In_ const std::string& InFileName, _In_ const File::OpenMode& InOpenMode);
			~FileScope();

			File* operator->() const { return _File; }

		private:

			File* _File = nullptr;
		};

		File* CreateFileHandle(_In_ const std::string& InFileName);
		void DestroyFileHandle(_Inout_ File*& InFile);
		bool FileExists(_In_ const std::string& InFileName);
		FileContent LoadFileToMemory(_In_ const std::string& InFileName);
		void UnloadFileFromMemory(_Inout_ FileContent& InOutFileContent);
	}
}
