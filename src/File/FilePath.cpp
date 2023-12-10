#include "File/FilePath.hpp"

#include "Types/Types.hpp"
#include "File/FileFactory.hpp"
#include <cstdlib>
#include <algorithm>
#include <filesystem>

namespace Eternal
{
	namespace FileSystem
	{
		vector<string> FilePath::_FolderPaths[static_cast<int32_t>(FileType::FILE_TYPE_COUNT)];

		template<typename FallbackFunction>
		static string FilePath_FindOrFallback(_In_ const string& InFileName, _In_ const FileType& InFileType, _In_ const FallbackFunction& InFallbackFunctor)
		{
			ETERNAL_ASSERT(static_cast<int32_t>(InFileType) < static_cast<int32_t>(FileType::FILE_TYPE_COUNT));
			ETERNAL_ASSERT(InFileName.size() > 0);
			const vector<string>& FolderPaths = FilePath::GetFolderPaths(InFileType);

			bool FileFound = false;
			string FullPath;
			for (uint32_t FolderIndex = 0; !FileFound && FolderIndex < FolderPaths.size(); ++FolderIndex)
			{
				string TempFullPath = FolderPaths[FolderIndex] + InFileName;
				FileFound = FileExists(TempFullPath);
				if (FileFound)
					FullPath = TempFullPath;
			}

			if (!FileFound)
				FullPath = InFallbackFunctor(FolderPaths, InFileName);

			return FullPath;
		}

		const vector<string>& FilePath::GetFolderPaths(_In_ const FileType& InFileType)
		{
			return _FolderPaths[static_cast<int32_t>(InFileType)];
		}

		string FilePath::Find(_In_ const string& InFileName, _In_ const FileType& InFileType)
		{
			return FilePath_FindOrFallback(
				InFileName,
				InFileType,
				[](_In_ const vector<string>& InFolderPaths, _In_ const string& InFileName) -> string { return string(); }
			);
		}

		string FilePath::FindOrCreate(_In_ const string& InFileName, _In_ const FileType& InFileType)
		{
			return FilePath_FindOrFallback(
				InFileName,
				InFileType,
				[](_In_ const vector<string>& InFolderPaths, _In_ const string& InFileName) -> string
				{
					string FullPath = InFolderPaths[0] + InFileName;
					File* NewFile = CreateFileHandle(FullPath);
					DestroyFileHandle(NewFile);
					return FullPath;
				}
			);
		}

		void FilePath::Register(_In_ const string& InFolderPath, _In_ const FileType& InFileType)
		{
			ETERNAL_ASSERT(static_cast<uint32_t>(InFileType) < static_cast<uint32_t>(FileType::FILE_TYPE_COUNT));
			ETERNAL_ASSERT(InFolderPath.size() > 0);
			_FolderPaths[static_cast<int32_t>(InFileType)].push_back(InFolderPath);
		}

		void FilePath::NormalizePath(_Inout_ string& InOutPath)
		{
			replace(InOutPath.begin(), InOutPath.end(), '/', '\\');
		}

		string FilePath::AbsolutePath(_In_ const string& InFileName)
		{
			return filesystem::absolute(InFileName).generic_string();
		}
	}
}
