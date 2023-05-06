#include "File/CFile.hpp"

namespace Eternal
{
	namespace FileSystem
	{
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

		void CFile::Open(_In_ const OpenMode& InOpenMode)
		{
			File::Open(InOpenMode);

			char* Flags;

			switch (InOpenMode)
			{
			case OpenMode::READ:
				Flags = "rb";
				break;
			case OpenMode::WRITE:
				Flags = "wb";
				break;
			case OpenMode::READ_WRITE:
				Flags = "r+b";
				break;

			default:
				ETERNAL_BREAK();
				return;
			}
			errno_t Err = fopen_s(&_File, _FileName.c_str(), Flags);
			ETERNAL_ASSERT(!Err);
			ETERNAL_ASSERT(_File);
		}

		void CFile::Close()
		{
			fclose(_File);
		}

		void CFile::Read(_In_ uint8_t* InBlock, _In_ uint64_t InSize)
		{
			fread(InBlock, InSize, 1, _File);
		}

		void CFile::Write(_In_ uint8_t* InBlock, _In_ uint64_t InSize)
		{
			fwrite(InBlock, InSize, 1, _File);
		}

		void CFile::Serialize(_Inout_ uint8_t* InOutBlock, _In_ uint64_t InSize)
		{
			if (IsOpenedForReadExclusive())
				Read(InOutBlock, InSize);
			else if (IsOpenedForWriteExclusive())
				Write(InOutBlock, InSize);
		}

		void CFile::Seek(_In_ uint64_t InOffset, _In_ const Origin& InOrigin)
		{
			ETERNAL_ASSERT(InOffset < 0xFFFFFFFFull);
			fseek(_File, (int)InOffset, (int)InOrigin);
		}

		uint64_t CFile::Tell() const
		{
			return ftell(_File);
		}

		uint64_t CFile::GetFileSize()
		{
			uint64_t Current = Tell();
			Seek(0, Origin::END);
			uint64_t Size = Tell();
			Seek(Current, Origin::SET);
			return Size;
		}
	}
}
