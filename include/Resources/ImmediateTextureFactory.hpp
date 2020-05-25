#pragma once

#include "Resources/TextureFactory.hpp"

namespace Eternal
{
	namespace Resources
	{
		class ImmediateTextureFactoryLoadTextureCallback : public TextureFactoryLoadTextureCallback
		{
		public:
			virtual bool LoadTexture(_In_ const string& Path, _Out_ RawTextureData& OutTextureData) override;
		};

		class ImmediateTextureFactoryCreateGpuResourceCallback : public TextureFactoryCreateGpuResourceCallback
		{
		public:
			virtual bool CreateTexture(_In_ const RawTextureData& TextureData, _Out_ Texture*& OutTexture) override;
		};
	}
}
