#include "File/FilePath.hpp"

#include "Types/Types.hpp"
#include "File/FileFactory.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		vector<string> FilePath::_FolderPaths[int(FileType::FILE_TYPE_COUNT)];

		string FilePath::Find(_In_ const string& InFileName, _In_ const FileType& InFileType)
		{
			ETERNAL_ASSERT(static_cast<int32_t>(InFileType) < static_cast<int32_t>(FileType::FILE_TYPE_COUNT));
			ETERNAL_ASSERT(InFileName.size() > 0);
			vector<string>& FolderPaths = _FolderPaths[static_cast<int32_t>(InFileType)];

			bool FileFound = false;
			string FullPath;
			for (uint32_t FolderIndex = 0; !FileFound && FolderIndex < FolderPaths.size(); ++FolderIndex)
			{
				string TempFullPath = FolderPaths[FolderIndex] + InFileName;
				FileFound = FileExists(TempFullPath);
				if (FileFound)
					FullPath = TempFullPath;
			}
			if (FileFound)
				return FullPath;
	
			return string();
		}

		void FilePath::Register(_In_ const string& InFolderPath, _In_ const FileType& InFileType)
		{
			ETERNAL_ASSERT(static_cast<int32_t>(InFileType) < int(FileType::FILE_TYPE_COUNT));
			ETERNAL_ASSERT(InFolderPath.size() > 0);
			_FolderPaths[static_cast<int32_t>(InFileType)].push_back(InFolderPath);
		}
	}
}
