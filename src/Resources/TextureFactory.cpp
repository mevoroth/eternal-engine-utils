#include "Resources/TextureFactory.hpp"

namespace Eternal
{
	namespace Resources
	{
		void RawTextureData::InitializeTextureData(
			_In_ void* InTextureData,
			_In_ const Format& InFormat,
			_In_ const ResourceDimension& InDimension,
			_In_ uint32_t InWidth,
			_In_ uint32_t InHeight,
			_In_ uint32_t InDepthOrArraySize,
			_In_ uint32_t InElementsCount,
			_In_ uint32_t InStride
		)
		{
			TextureData			= InTextureData;
			TextureFormat		= InFormat;
			Dimension			= InDimension;
			Width				= InWidth;
			Height				= InHeight;
			DepthOrArraySize	= InDepthOrArraySize;
			ElementsCount		= InElementsCount;
			Stride				= InStride;
		}

		void RawTextureData::ReleaseTextureData()
		{
			if (TextureData)
			{
				delete[] TextureData;
				TextureData = nullptr;
			}
		}

		TextureCache TextureFactory::EmptyTexture;

		void TextureFactory::CreateTextureCacheEntry(_In_ const TextureKey& InKey)
		{
			TextureCacheStorage::iterator FoundTexture = _Textures.find(InKey);
			if (FoundTexture == _Textures.cend())
				_Textures.insert(std::make_pair(InKey, TextureCache()));
		}

		TextureCache& TextureFactory::GetTextureCache(_In_ const TextureKey& InKey)
		{
			TextureCacheStorage::iterator FoundTexture = _Textures.find(InKey);
			if (FoundTexture != _Textures.cend())
				return FoundTexture->second;

			ETERNAL_BREAK();
			return EmptyTexture;
		}

		bool TextureFactory::TextureExists(_In_ const TextureKey& InKey) const
		{
			TextureCacheStorage::const_iterator FoundTexture = _Textures.find(InKey);
			return FoundTexture != _Textures.cend();
		}
	}
}
