#include "File/FilePath.hpp"

#include "Macros/Macros.hpp"
#include "Types/Types.hpp"
#include "File/FileFactory.hpp"

using namespace Eternal::File;
using namespace Eternal::Types;

vector<string> FilePath::_FolderPaths[int(FileType::FILE_TYPE_COUNT)];

string FilePath::Find(_In_ const string& FileName, _In_ const FileType& Type)
{
	ETERNAL_ASSERT(int(Type) < int(FileType::FILE_TYPE_COUNT));
	ETERNAL_ASSERT(FileName.size() > 0);
	vector<string>& FolderPaths = _FolderPaths[int(Type)];

	bool FileFound = false;
	string FullPath;
	for (u32 FolderIndex = 0; !FileFound && FolderIndex < FolderPaths.size(); ++FolderIndex)
	{
		string TempFullPath = FolderPaths[FolderIndex] + FileName;
		FileFound = FileExists(TempFullPath);
		if (FileFound)
			FullPath = TempFullPath;
	}
	if (FileFound)
		return FullPath;
	
	ETERNAL_ASSERT(false);
	return string();
}

void FilePath::Register(_In_ const string& FolderPath, _In_ const FileType& Type)
{
	ETERNAL_ASSERT(int(Type) < int(FileType::FILE_TYPE_COUNT));
	ETERNAL_ASSERT(FolderPath.size() > 0);
	_FolderPaths[int(Type)].push_back(FolderPath);
}
