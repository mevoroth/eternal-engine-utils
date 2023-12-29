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
			FILE_TYPE_SHADERS_PDB,
			FILE_TYPE_LEVELS,
			FILE_TYPE_TEXTURES,
			FILE_TYPE_MESHES,
			FILE_TYPE_CACHED_MESHES,
			FILE_TYPE_CACHED_PIPELINES,
			FILE_TYPE_MATERIALS,
			FILE_TYPE_COUNT
		};

		class FilePath
		{
		public:
			static const vector<string>& GetFolderPaths(_In_ const FileType& InFileType);

			static void Register(_In_ const string& InFolderPath, _In_ const FileType& InFileType);
			static string Find(_In_ const string& InFileName, _In_ const FileType& InFileType);
			static string FindOrCreate(_In_ const string& InFileName, _In_ const FileType& InFileType);
			static void NormalizePath(_Inout_ string& InOutPath);
			static string AbsolutePath(_In_ const string& InFileName);
			static void SplitPath(_In_ const string& InFilePath, _Out_ string& OutDirectory, _Out_ string& OutFileName, _Out_ string& OutExtension);

		private:
			static vector<string> _FolderPaths[static_cast<int32_t>(FileType::FILE_TYPE_COUNT)];
		};
	}
}
