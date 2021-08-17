#pragma once

#include <map>
#include <string>
#include <list>

using namespace std;

namespace Eternal
{
	namespace GraphicData
	{
		class Texture;
	}
	namespace Resources
	{
		using namespace GraphicData;

		struct RawTextureData
		{
			void Initialize(
				_In_ void* InTextureData,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight,
				_In_ uint32_t InDepthOrArraySize,
				_In_ uint32_t InStride
			);
			void Release();

			void* TextureData			= nullptr;
			uint32_t Width				= 0;
			uint32_t Height				= 0;
			uint32_t DepthOrArraySize	= 0;
			uint32_t Stride				= 0;
		};

		struct TextureCache
		{
			Texture* CachedTexture = nullptr;
		};

		using TextureKey = string;
		using TextureCacheStorage = map<TextureKey, TextureCache>;

		class TextureFactory
		{
		public:
			static TextureCache EmptyTexture;

			void CreateTextureCacheEntry(_In_ const TextureKey& InKey);
			TextureCache& GetTextureCache(_In_ const TextureKey& InKey);
			bool TextureExists(_In_ const TextureKey& InKey) const;

		private:
			TextureCacheStorage _Textures;
		};
	}
}
