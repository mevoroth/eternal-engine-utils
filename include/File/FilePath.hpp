#ifndef _FILE_PATH_HPP_
#define _FILE_PATH_HPP_

#include <string>
#include <vector>

namespace Eternal
{
	namespace File
	{
		using namespace std;

		class FilePath
		{
		public:
			enum FileType
			{
				SHADERS		= 0,
				SAVES		= 1,
				TEXTURES	= 2,
				FILE_TYPE_COUNT
			};
			static void Register(_In_ const string& FolderPath, _In_ const FileType& Type);
			static string Find(_In_ const string& FileName, _In_ const FileType& Type);

		private:
			static vector<string> _FolderPaths[FILE_TYPE_COUNT];
		};
	}
}

#endif
