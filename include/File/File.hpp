#pragma once

namespace Eternal
{
	namespace FileSystem
	{
		class File
		{
		public:
			enum OpenMode
			{
				READ		= 0x1,
				WRITE		= 0x2,
				READ_WRITE	= READ | WRITE
			};
			enum Origin
			{
				SET = 0,
				CUR = 1,
				END = 2
			};
			virtual ~File() {}
			virtual void Open(_In_ const OpenMode& InOpenMode) = 0;
			virtual void Close() = 0;
			virtual void Read(_In_ uint8_t* InBlock, _In_ uint64_t InSize) = 0;
			virtual void Write(_In_ uint8_t* InBlock, _In_ uint64_t InSize) = 0;
			virtual void Seek(_In_ uint64_t InOffset, _In_ const Origin& InOrigin) = 0;
			virtual uint64_t Tell() const = 0;
			virtual uint64_t GetFileSize() = 0;
		};
	}
}
