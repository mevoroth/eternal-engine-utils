#include "File/FileFactory.hpp"

#include "UtilsSettings.hpp"
#if ETERNAL_USE_CFILE
#include "File/CFile.hpp"
#endif
#if ETERNAL_USE_ANDROID_ASSET_MANAGER_FILE
#include "File/Android/AndroidAssetManagerFile.hpp"
#endif

namespace Eternal
{
	namespace FileSystem
	{
		File* CreateFileHandle(_In_ const std::string& InFileName)
		{
#if ETERNAL_USE_CFILE
			return new CFile(InFileName);
#endif

#if ETERNAL_USE_ANDROID_ASSET_MANAGER_FILE
			return new AndroidAssetManagerFile(InFileName);
#endif

			ETERNAL_BREAK();
			return nullptr;

		}

		void DestroyFileHandle(_Inout_ File*& InFile)
		{
			delete InFile;
			InFile = nullptr;
		}

		bool FileExists(_In_ const std::string& InFileName)
		{
#if ETERNAL_USE_CFILE
			return CFile::Exists(InFileName);
#endif

#if ETERNAL_USE_ANDROID_ASSET_MANAGER_FILE
			return AndroidAssetManagerFile::Exists(InFileName);
#endif

			ETERNAL_BREAK();
			return false;
		}

		FileContent LoadFileToMemory(_In_ const std::string& InFileName)
		{
			FileContent Content;

			File* FileHandle = CreateFileHandle(InFileName);
			FileHandle->Open(FileOpenMode::FILE_OPEN_MODE_READ);
			Content.Size	= FileHandle->GetFileSize();
			Content.Content	= new uint8_t[Content.Size + 1];
			FileHandle->Read(Content.Content, Content.Size);
			Content.Content[Content.Size] = '\0';
			FileHandle->Close();
			DestroyFileHandle(FileHandle);

			return Content;
		}

		void UnloadFileFromMemory(_Inout_ FileContent& InOutFileContent)
		{
			delete[] InOutFileContent.Content;
			InOutFileContent.Content	= nullptr;
			InOutFileContent.Size		= 0;
		}

		////////////////////////////////////////////////////////////////////////////

		FileContent::~FileContent()
		{
			//ETERNAL_ASSERT(!Content);
			//ETERNAL_ASSERT(!Size);
		}

		////////////////////////////////////////////////////////////////////////////

		FileScope::FileScope(_In_ const std::string& InFileName, _In_ const FileOpenMode& InOpenMode)
			: _File(CreateFileHandle(InFileName))
		{
			_File->Open(InOpenMode);
		}

		FileScope::~FileScope()
		{
			_File->Close();
			DestroyFileHandle(_File);
		}
	}
}
