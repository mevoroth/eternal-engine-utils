#include "Resources/TextureLoader.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Import/dds/ImportDds.hpp"
#include "Resources/Payload.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/Resource.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Resources
	{
		using namespace Eternal::Import;

		TextureLoader::TextureLoader(_In_ TextureFactory& InTextureFactory, _In_ MaterialUpdateBatcher& InMaterialUpdateBatcher)
			: _ImportTga(new ImportTga())
			, _ImportDds(new ImportDds())
			, _Factory(InTextureFactory)
			, _Batcher(InMaterialUpdateBatcher)
		{
		}

		TextureLoader::~TextureLoader()
		{
			delete _ImportTga;
			_ImportTga = nullptr;

			delete  _ImportDds;
			_ImportDds = nullptr;
		}

		void TextureLoader::LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const
		{
			ETERNAL_PROFILER(BASIC)();
			OutPayload = new TexturePayload();

			const TextureRequest* InTextureRequest	= static_cast<const TextureRequest*>(InRequest);

			TexturePayload& OutPayloadIntermediate	= *static_cast<TexturePayload*>(OutPayload);
			OutPayloadIntermediate.Key				= InTextureRequest->MaterialToUpdate.Key;

			auto CurrentMaterialUpdateBatch = _Batcher.MaterialUpdates.find(InTextureRequest->MaterialToUpdate.Key);

			if (CurrentMaterialUpdateBatch == _Batcher.MaterialUpdates.end())
				CurrentMaterialUpdateBatch = _Batcher.MaterialUpdates.insert({ InTextureRequest->MaterialToUpdate.Key, MaterialUpdateBatch() }).first;

			MaterialUpdateBatch& CurrentBatch = CurrentMaterialUpdateBatch->second;
			CurrentBatch.Materials.push_back(std::move(InTextureRequest->MaterialToUpdate));

			if (_Factory.TextureExists(InTextureRequest->MaterialToUpdate.Key))
				return;

			_Factory.CreateTextureCacheEntry(InTextureRequest->MaterialToUpdate.Key);

			uint32_t Height		= 0;
			uint32_t Width		= 0;
			uint32_t Depth		= 0;
			uint8_t* ImageData	= nullptr;

			ETERNAL_ASSERT(InRequest->RequestPath.size() >= 4);
			
			const uint32_t Extension = *reinterpret_cast<const uint32_t*>(&InRequest->RequestPath[InRequest->RequestPath.size() - 4]);

			switch (Extension)
			{
			case 'agt.': // tga
			{
				ImageData = _ImportTga->Import(InRequest->RequestPath, Width, Height);
				Depth = 1;

				CurrentBatch.TextureData.InitializeTextureData(
					ImageData,
					Graphics::Format::FORMAT_BGRA8888_UNORM,
					Graphics::ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D,
					Width,
					Height,
					Depth,
					Width * Height * Depth,
					4
				);
			} break;
			case 'sdd.': // dds
			{
				Graphics::Format Format					= Graphics::Format::FORMAT_UNKNOWN;
				Graphics::ResourceDimension Dimension	= Graphics::ResourceDimension::RESOURCE_DIMENSION_UNKNOWN;

				ImageData = _ImportDds->Import(InRequest->RequestPath, Format, Dimension, Width, Height, Depth);

				CurrentBatch.TextureData.InitializeTextureData(
					ImageData,
					Format,
					Dimension,
					Width,
					Height,
					Depth,
					Width * Height * Depth,
					4
				);
			} break;
			}
			LogWrite(LogInfo, LogImport, string("Loaded [") + InRequest->RequestPath + "]");
			ETERNAL_ASSERT(CurrentBatch.TextureData.TextureData);
		}

		void TextureLoader::DestroyPayloadLoader()
		{
			delete this;
		}
	}
}
