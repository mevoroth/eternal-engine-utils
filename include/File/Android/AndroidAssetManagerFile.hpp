#pragma once

#if ETERNAL_PLATFORM_ANDROID

#include "File/File.hpp"
#include <string>

struct __sFILE;
typedef struct __sFILE FILE;

struct AAssetManager;
struct AAsset;

namespace Eternal
{
	namespace FileSystem
	{
		using errno_t = int;

		errno_t fopen_s(_Out_ FILE** OutStreamPointer, _In_ const char* InFilePath, _In_ const char* InMode);

		class AndroidAssetManagerFile final : public File
		{
		public:

			static void RegisterAndroidAssetManager(_In_ AAssetManager* InAssetManager);
			static bool Exists(_In_ const std::string& InFileName);

			AndroidAssetManagerFile(_In_ const std::string& InFileName);

			virtual void Open(_In_ const FileOpenMode& InOpenMode) override final;
			virtual void Close() override final;
			virtual void Read(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize) override final;
			virtual void Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize) override final;
			virtual void Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor) override final;
			virtual uint64_t Tell() const override final;
			virtual uint64_t GetFileSize() override final;

		private:
			std::string	_FileName;
			AAsset*		_Asset = nullptr;

			static AAssetManager* AssetManager;

		};
	}
}

#endif
