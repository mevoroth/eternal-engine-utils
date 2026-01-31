#if ETERNAL_PLATFORM_ANDROID

#include "File/Android/AndroidAssetManagerFile.hpp"
#include <android/asset_manager.h>

namespace Eternal
{
	namespace FileSystem
	{
		errno_t fopen_s(_Out_ FILE** OutStreamPointer, _In_ const char* InFilePath, _In_ const char* InMode)
		{
			errno_t Result = 0;

			ETERNAL_ASSERT(OutStreamPointer);
			*OutStreamPointer = fopen(InFilePath, InMode);
			if (!*OutStreamPointer)
				Result = errno;
			return Result;
		}

		//////////////////////////////////////////////////////////////////////////

		AAssetManager* AndroidAssetManagerFile::AssetManager = nullptr;

		void AndroidAssetManagerFile::RegisterAndroidAssetManager(_In_ AAssetManager* InAssetManager)
		{
			AssetManager = InAssetManager;
		}

		bool AndroidAssetManagerFile::Exists(_In_ const std::string& InFileName)
		{
			ETERNAL_ASSERT(AssetManager);

			AAsset* Asset = AAssetManager_open(AssetManager, InFileName.c_str(), AASSET_MODE_UNKNOWN);
			if (!Asset)
				return false;

			AAsset_close(Asset);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////

		AndroidAssetManagerFile::AndroidAssetManagerFile(_In_ const std::string& InFileName)
			: _FileName(InFileName)
		{
		}

		void AndroidAssetManagerFile::Open(_In_ const FileOpenMode& InOpenMode)
		{
			File::Open(InOpenMode);

			_Asset = AAssetManager_open(AssetManager, _FileName.c_str(), AASSET_MODE_BUFFER);
			ETERNAL_ASSERT(_Asset);
		}

		void AndroidAssetManagerFile::Close()
		{
			File::Close();
			AAsset_close(_Asset);
		}

		void AndroidAssetManagerFile::Read(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize)
		{
			File::Read(OutBlock, InSize);
			int Result = AAsset_read(_Asset, OutBlock, InSize);
			ETERNAL_ASSERT(Result >= 0);
		}

		void AndroidAssetManagerFile::Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize)
		{
			ETERNAL_BREAK();
		}

		void AndroidAssetManagerFile::Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor)
		{
			off_t Result = AAsset_seek(_Asset, InOffset, static_cast<int>(InCursor));
			ETERNAL_ASSERT(Result >= 0);
		}

		uint64_t AndroidAssetManagerFile::Tell() const
		{
			return static_cast<uint64_t>(AAsset_seek(_Asset, 0, SEEK_CUR));
		}

		uint64_t AndroidAssetManagerFile::GetFileSize()
		{
			return static_cast<uint64_t>(AAsset_getLength(_Asset));
		}
	}
}

#endif
