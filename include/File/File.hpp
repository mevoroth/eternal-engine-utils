#ifndef _FILE_HPP_
#define _FILE_HPP_

namespace Eternal
{
	namespace File
	{
		class File
		{
		public:
			enum OpenMode
			{
				READ = 0x1,
				WRITE = 0x2
			};
			enum Origin
			{
				SET = 0,
				CUR = 1,
				END = 2
			};
			virtual ~File() {}
			virtual void Open(const OpenMode& OpenModeObj) = 0;
			virtual void Close() = 0;
			virtual void Read(uint8_t* Block, uint64_t Size) = 0;
			virtual void Write(uint8_t* Block, uint64_t Size) = 0;
			virtual void Seek(uint64_t Offset, const Origin& OriginObj) = 0;
			virtual uint64_t Tell() const = 0;
			virtual uint64_t GetFileSize() = 0;
		};
	}
}

#endif
