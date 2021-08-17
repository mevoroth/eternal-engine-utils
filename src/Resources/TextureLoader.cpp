#include "Resources/TextureLoader.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Resources/Payload.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Resources
	{
		using namespace Eternal::Import;

		TextureLoader::TextureLoader(_In_ TextureFactory& InTextureFactory)
			: _ImportTga(new ImportTga())
			, _Factory(InTextureFactory)
		{
		}

		TextureLoader::~TextureLoader()
		{
			delete _ImportTga;
			_ImportTga = nullptr;
		}

		void TextureLoader::LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const
		{
			ETERNAL_PROFILER(BASIC)();
			OutPayload = new TexturePayload();

			const TextureRequest* InTextureRequest	= static_cast<const TextureRequest*>(InRequest);

			TexturePayload& PayloadIntermediate		= *static_cast<TexturePayload*>(OutPayload);
			PayloadIntermediate.MaterialToUpdate	= std::move(InTextureRequest->MaterialToUpdate);

			if (_Factory.TextureExists(InTextureRequest->MaterialToUpdate.Key))
				return;

			_Factory.CreateTextureCacheEntry(InTextureRequest->MaterialToUpdate.Key);

			uint32_t Height	= 0;
			uint32_t Width	= 0;
			uint8_t* ImageData = _ImportTga->Import(InRequest->RequestPath, Height, Width);

			PayloadIntermediate.TextureData.Initialize(
				ImageData, Width, Height, 1, 4
			);
			LogWrite(LogInfo, LogImport, string("Loaded [") + InRequest->RequestPath + "]");
		}

		void TextureLoader::DestroyPayloadLoader()
		{
			delete this;
		}
	}
}
