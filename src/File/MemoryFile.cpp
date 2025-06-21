#include "File/MemoryFile.hpp"
#include "File/FileFactory.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		MemoryFile::MemoryFile(_In_ FileContent& InFileContent)
			: _FileContent(InFileContent)
		{
		}

		void MemoryFile::Read(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize)
		{
			File::Read(OutBlock, InSize);
			ETERNAL_ASSERT(_Offset < GetFileSize());
			memcpy(OutBlock, _FileContent.Content + _Offset, InSize);
			_Offset += InSize;
		}

		void MemoryFile::Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize)
		{
			File::Write(InBlock, InSize);
			ETERNAL_ASSERT(_Offset < GetFileSize());
			memcpy(_FileContent.Content + _Offset, InBlock, InSize);
			_Offset += InSize;
		}

		void MemoryFile::Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor)
		{
			switch (InCursor)
			{
			case FileCursor::FILE_CURSOR_CUR:
			{
				ETERNAL_ASSERT(_Offset + InOffset <= GetFileSize());
				_Offset += InOffset;
			} break;
			case FileCursor::FILE_CURSOR_END:
			{
				ETERNAL_ASSERT(InOffset < GetFileSize());
				_Offset = GetFileSize() - InOffset - 1ull;
			} break;
			default:
				ETERNAL_BREAK();
				ETERNAL_FALLTHROUGH();
			case FileCursor::FILE_CURSOR_SET:
			{
				ETERNAL_ASSERT(InOffset <= GetFileSize());
				_Offset = InOffset;
			} break;
			}
		}

		uint64_t MemoryFile::Tell() const
		{
			return _Offset;
		}

		uint64_t MemoryFile::GetFileSize()
		{
			return _FileContent.Size;
		}
	}
}
