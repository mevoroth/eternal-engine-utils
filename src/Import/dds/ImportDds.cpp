#include "Import/dds/ImportDds.hpp"
#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
#include "DirectXTex/DirectXTex.h"
#endif
#include "File/FileFactory.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::Graphics;

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
		static Format ConvertDDSFormatToFormat(_In_ const DXGI_FORMAT& InDDSFormat)
		{
			switch (InDDSFormat)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
				return Format::FORMAT_RGBA8888_UNORM;
			//case DXGI_FORMAT_R16G16_UNORM:
			//case DXGI_FORMAT_R8G8_UNORM:
			//case DXGI_FORMAT_R16_UNORM:
			//case DXGI_FORMAT_R8_UNORM:

			case DXGI_FORMAT_A8_UNORM:
				return Format::FORMAT_R8_UNORM;

			//case DXGI_FORMAT_R8G8_B8G8_UNORM:
			//case DXGI_FORMAT_G8R8_G8B8_UNORM:
			//case DXGI_FORMAT_BC1_UNORM:
			//case DXGI_FORMAT_BC2_UNORM:
			//case DXGI_FORMAT_BC3_UNORM:
			//case DXGI_FORMAT_BC4_SNORM:
			//case DXGI_FORMAT_BC5_SNORM:
			//case DXGI_FORMAT_B5G6R5_UNORM:
			//case DXGI_FORMAT_B5G5R5A1_UNORM:
			//case DXGI_FORMAT_R8G8_SNORM:
			case DXGI_FORMAT_R8G8B8A8_SNORM:
				return Format::FORMAT_RGBA8888_SNORM;
			//case DXGI_FORMAT_R16G16_SNORM:
			//case DXGI_FORMAT_B8G8R8A8_UNORM:
			//case DXGI_FORMAT_B8G8R8X8_UNORM:
			//case DXGI_FORMAT_B4G4R4A4_UNORM:
			//case DXGI_FORMAT_YUY2:
			//case DXGI_FORMAT_R32G32B32A32_FLOAT:
			//case DXGI_FORMAT_R16G16B16A16_FLOAT:
			//case DXGI_FORMAT_R16G16B16A16_UNORM:
			//case DXGI_FORMAT_R16G16B16A16_SNORM:
			//case DXGI_FORMAT_R32G32_FLOAT:
			//case DXGI_FORMAT_R16G16_FLOAT:
			//case DXGI_FORMAT_R32_FLOAT:
			//case DXGI_FORMAT_R16_FLOAT:
			//case DXGI_FORMAT_R10G10B10A2_UNORM:
			//case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			//case DXGI_FORMAT_BC1_UNORM_SRGB:
			//case DXGI_FORMAT_BC2_UNORM_SRGB:
			//case DXGI_FORMAT_BC3_UNORM_SRGB:
			//case DXGI_FORMAT_BC4_UNORM:
			//case DXGI_FORMAT_BC5_UNORM:
			//case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			//case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:

			default:
				ETERNAL_BREAK();
				return Format::FORMAT_UNKNOWN;
			}
		}
#endif

		uint8_t* ImportDds::Import(_In_ const string& InPath, _Out_ Format& OutFormat, _Out_ ResourceDimension& OutResourceDimension, _Out_ uint32_t& OutWidth, _Out_ uint32_t& OutHeight, _Out_ uint32_t& OutDepth)
		{
			(void)InPath;
			(void)OutFormat;
			(void)OutResourceDimension;
			(void)OutWidth;
			(void)OutHeight;
			(void)OutDepth;
#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
			using namespace DirectX;
			using namespace Eternal::FileSystem;

			if (InPath.size() <= 3 ||
				InPath[InPath.size() - 3] != 'd' ||
				InPath[InPath.size() - 2] != 'd' ||
				InPath[InPath.size() - 1] != 's')
			{
				OutFormat				= Format::FORMAT_UNKNOWN;
				OutResourceDimension	= ResourceDimension::RESOURCE_DIMENSION_UNKNOWN;
				OutHeight = OutWidth = OutDepth = 0;
				return nullptr;
			}

			FileContent DdsFileContent = LoadFileToMemory(InPath);

			TexMetadata DdsTextureMetaData;
			ScratchImage DdsImagePayload;
			HRESULT hr = LoadFromDDSMemory(DdsFileContent.Content, DdsFileContent.Size, DDS_FLAGS_NONE, &DdsTextureMetaData, DdsImagePayload);
			ETERNAL_ASSERT(hr == S_OK);

			OutFormat	= ConvertDDSFormatToFormat(DdsTextureMetaData.format);
			OutWidth	= static_cast<uint32_t>(DdsTextureMetaData.width);
			OutHeight	= static_cast<uint32_t>(DdsTextureMetaData.height);
			OutDepth	= static_cast<uint32_t>(DdsTextureMetaData.depth);
			
			if (DdsTextureMetaData.IsCubemap())
				OutResourceDimension = DdsTextureMetaData.arraySize / 6 > 1 ? ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY : ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE;
			else if (DdsTextureMetaData.IsVolumemap())
				OutResourceDimension = ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D;
			else if (DdsTextureMetaData.dimension == TEX_DIMENSION_TEXTURE2D)
				OutResourceDimension = DdsTextureMetaData.arraySize > 1 ? ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY : ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D;
			else if (DdsTextureMetaData.dimension == TEX_DIMENSION_TEXTURE1D)
				OutResourceDimension = DdsTextureMetaData.arraySize > 1 ? ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY : ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D;
			else
			{
				ETERNAL_BREAK();
				OutResourceDimension = ResourceDimension::RESOURCE_DIMENSION_UNKNOWN;
			}

			UnloadFileFromMemory(DdsFileContent);

			uint8_t* ImageBytes = new uint8_t[DdsImagePayload.GetPixelsSize()];
			memcpy(ImageBytes, DdsImagePayload.GetPixels(), DdsImagePayload.GetPixelsSize());
			return ImageBytes;
#endif
			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
