#pragma once

#include "File/File.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		struct FileContent;

		class MemoryFile final : public File
		{
		public:

			MemoryFile(_In_ FileContent& InFileContent);

			virtual void Read(_In_ uint8_t* InBlock, _In_ uint64_t InSize) override final;
			virtual void Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize) override final;
			virtual void Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor) override final;
			virtual uint64_t Tell() const override final;
			virtual uint64_t GetFileSize() override final;

		private:

			FileContent&	_FileContent;
			uint64_t		_Offset = 0ull;
		};
	}
}
