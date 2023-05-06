#pragma once

#include <map>
#include <string>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		enum class Format;
		enum class ResourceDimension;
	}
	namespace GraphicData
	{
		class Texture;
	}
	namespace Resources
	{
		using namespace GraphicData;
		using namespace Graphics;

		struct RawTextureData
		{
			void InitializeTextureData(
				_In_ void* InTextureData,
				_In_ const Format& InFormat,
				_In_ const ResourceDimension& InDimension,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight,
				_In_ uint32_t InDepthOrArraySize,
				_In_ uint32_t InElementsCount,
				_In_ uint32_t InStride
			);
			void ReleaseTextureData();

			void* TextureData			= nullptr;
			Format TextureFormat		= static_cast<Format>(0);
			ResourceDimension Dimension	= static_cast<ResourceDimension>(0);
			uint32_t Width				= 0;
			uint32_t Height				= 0;
			uint32_t DepthOrArraySize	= 0;
			uint32_t ElementsCount		= 0;
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

			const TextureCacheStorage& GetTextures() const { return _Textures; }

		private:
			TextureCacheStorage _Textures;
		};
	}
}
