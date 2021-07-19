#pragma once

#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		using namespace std;
		
		enum class FileType
		{
			FILE_TYPE_SHADERS	= 0,
			FILE_TYPE_LEVELS,
			FILE_TYPE_TEXTURES,
			FILE_TYPE_MESHES,
			FILE_TYPE_COUNT
		};

		class FilePath
		{
		public:
			static void Register(_In_ const string& FolderPath, _In_ const FileType& Type);
			static string Find(_In_ const string& FileName, _In_ const FileType& Type);

		private:
			static vector<string> _FolderPaths[static_cast<int32_t>(FileType::FILE_TYPE_COUNT)];
		};
	}
}
