#pragma once

#include <string>
#include "File/File.hpp"
#include <cstdio>

namespace Eternal
{
	namespace FileSystem
	{
		class CFile final : public File
		{
		public:
			static bool Exists(_In_ const std::string& InFileName);

			CFile(_In_ const std::string& InFileName);
			virtual void Open(_In_ const FileOpenMode& InOpenMode) override final;
			virtual void Close() override final;
			virtual void Read(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize) override final;
			virtual void Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize) override final;
			virtual void Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor) override final;
			virtual uint64_t Tell() const override final;
			virtual uint64_t GetFileSize() override final;

		private:
			std::string _FileName;
			std::FILE* _File = nullptr;

		};
	}
}
