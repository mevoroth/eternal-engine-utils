#include "File/FileFactory.hpp"

#include "UtilsSettings.hpp"
#ifdef ETERNAL_USE_CFILE
#include "File/CFile.hpp"
#endif

namespace Eternal
{
	namespace FileSystem
	{
		File* CreateFileHandle(_In_ const std::string& InFileName)
		{
#ifdef ETERNAL_USE_CFILE
			return new CFile(InFileName);
#else
			return nullptr;
#endif
		}

		void DestroyFileHandle(_Inout_ File*& InFile)
		{
			delete InFile;
			InFile = nullptr;
		}

		bool FileExists(_In_ const std::string& InFileName)
		{
#ifdef ETERNAL_USE_CFILE
			return CFile::Exists(InFileName);
#else
			return false;
#endif
		}

		FileContent LoadFileToMemory(_In_ const std::string& InFileName)
		{
			FileContent Content;

			File* FileHandle = CreateFileHandle(InFileName);
			FileHandle->Open(File::OpenMode::READ);
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

		FileScope::FileScope(_In_ const std::string& InFileName, _In_ const File::OpenMode& InOpenMode)
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
