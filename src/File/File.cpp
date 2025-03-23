#include "File/File.hpp"

#include "Types/Enums.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		using namespace Eternal::Types;

		void File::Open(_In_ const FileOpenMode& InOpenMode)
		{
			_OpenMode = InOpenMode;
		}

		void File::Close()
		{
			_OpenMode = FileOpenMode::FILE_OPEN_MODE_CLOSED;
		}

		void File::Read(_In_ uint8_t* InBlock, _In_ uint64_t InSize)
		{
			ETERNAL_ASSERT(IsOpenedForRead());
			(void)InBlock;
			(void)InSize;
		}

		void File::Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize)
		{
			ETERNAL_ASSERT(IsOpenedForWrite());
			(void)InBlock;
			(void)InSize;
		}

		void File::Serialize(_Inout_ uint8_t* InOutBlock, _In_ uint64_t InSize)
		{
			ETERNAL_ASSERT(_OpenMode != FileOpenMode::FILE_OPEN_MODE_CLOSED);
			if (IsOpenedForReadExclusive())
				Read(InOutBlock, InSize);
			else if (IsOpenedForWriteExclusive())
				Write(InOutBlock, InSize);
		}

		bool File::IsOpenedForRead() const
		{
			return (ToUInt(_OpenMode) & ToUInt(FileOpenMode::FILE_OPEN_MODE_READ)) != ToUInt(FileOpenMode::FILE_OPEN_MODE_CLOSED);
		}

		bool File::IsOpenedForWrite() const
		{
			return (ToUInt(_OpenMode) & ToUInt(FileOpenMode::FILE_OPEN_MODE_WRITE)) != ToUInt(FileOpenMode::FILE_OPEN_MODE_CLOSED);
		}
	}
}
