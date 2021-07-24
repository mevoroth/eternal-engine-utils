#include "Resources/TextureLoader.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Resources/Payload.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Resources
	{
		using namespace Eternal::Import;

		TextureLoader::TextureLoader()
			: _ImportTga(new ImportTga())
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
			uint32_t Height	= 0;
			uint32_t Width	= 0;
			uint8_t* ImageData = _ImportTga->Import(InRequest->RequestPath, Height, Width);

			OutPayload = new TexturePayload();
			TexturePayload& PayLoadIntermediate = *static_cast<TexturePayload*>(OutPayload);
			PayLoadIntermediate.TextureData.Initialize(
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
