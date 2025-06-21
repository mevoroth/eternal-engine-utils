#include "File/CFile.hpp"

#include "Types/Enums.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		using namespace Eternal::Types;

		ETERNAL_STATIC_ASSERT(ToUInt(FileCursor::FILE_CURSOR_SET) == SEEK_SET, "FileOrigin::FILE_CURSOR_SET incorrect mapping");
		ETERNAL_STATIC_ASSERT(ToUInt(FileCursor::FILE_CURSOR_CUR) == SEEK_CUR, "FileOrigin::FILE_CURSOR_CUR incorrect mapping");
		ETERNAL_STATIC_ASSERT(ToUInt(FileCursor::FILE_CURSOR_END) == SEEK_END, "FileOrigin::FILE_CURSOR_END incorrect mapping");

		bool CFile::Exists(_In_ const std::string& InFileName)
		{
			bool FileExists = false;
			if (InFileName.size() > 0)
			{
				std::FILE* File = nullptr;
				errno_t Err = fopen_s(&File, InFileName.c_str(), "rb");
				FileExists = !Err && File;
				if (File)
					fclose(File);
			}
			return FileExists;
		}

		CFile::CFile(_In_ const std::string& InFileName)
			: _FileName(InFileName)
		{
		}

		void CFile::Open(_In_ const FileOpenMode& InOpenMode)
		{
			File::Open(InOpenMode);

			const char* Flags = [&InOpenMode]()
			{
				switch (InOpenMode)
				{
				case FileOpenMode::FILE_OPEN_MODE_READ:
					return "rb";

				case FileOpenMode::FILE_OPEN_MODE_WRITE:
					return "wb";

				case FileOpenMode::FILE_OPEN_MODE_READ_WRITE:
					return "r+b";

				default:
					ETERNAL_BREAK();
					return "";
				}
			}();

			errno_t Err = fopen_s(&_File, _FileName.c_str(), Flags);
			ETERNAL_ASSERT(!Err);
			ETERNAL_ASSERT(_File);
		}

		void CFile::Close()
		{
			File::Close();

			fclose(_File);
		}

		void CFile::Read(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize)
		{
			File::Read(OutBlock, InSize);

			fread(OutBlock, InSize, 1, _File);
		}

		void CFile::Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize)
		{
			File::Write(InBlock, InSize);

			fwrite(InBlock, InSize, 1, _File);
		}

		void CFile::Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor)
		{
			ETERNAL_ASSERT(InOffset < 0xFFFFFFFFull);
			fseek(_File, static_cast<long>(InOffset), ToInt(InCursor));
		}

		uint64_t CFile::Tell() const
		{
			return ftell(_File);
		}

		uint64_t CFile::GetFileSize()
		{
			uint64_t Current = Tell();
			Seek(0, FileCursor::FILE_CURSOR_END);
			uint64_t Size = Tell();
			Seek(Current, FileCursor::FILE_CURSOR_SET);
			return Size;
		}
	}
}
