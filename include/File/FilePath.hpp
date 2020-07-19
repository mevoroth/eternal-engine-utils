#ifndef _FILE_PATH_HPP_
#define _FILE_PATH_HPP_

#include <string>

namespace Eternal
{
	namespace File
	{
		using namespace std;
		
		enum class FileType
		{
			SHADERS		= 0,
			SAVES		= 1,
			TEXTURES	= 2,
			FILE_TYPE_COUNT
		};

		class FilePath
		{
		public:
			static void Register(_In_ const string& FolderPath, _In_ const FileType& Type);
			static string Find(_In_ const string& FileName, _In_ const FileType& Type);

		private:
			static vector<string> _FolderPaths[int(FileType::FILE_TYPE_COUNT)];
		};
	}
}

#endif
