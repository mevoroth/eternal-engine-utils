#pragma once

#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		class File;

		File* CreateFileHandle(_In_ const std::string& InFileName);
		void DestroyFileHandle(_In_ File*& InFile);
		bool FileExists(_In_ const std::string& InFileName);
	}
}
