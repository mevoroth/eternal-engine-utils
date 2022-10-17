#pragma once

#include "Resources/Streaming.hpp"

namespace Eternal
{
	namespace Import
	{
		class ImportTga;
		class ImportDds;
	}

	namespace Resources
	{
		using namespace Eternal::Import;

		class TextureFactory;

		struct TextureLoader final : public StreamingLoader
		{
			TextureLoader(_In_ TextureFactory& InTextureFactory);
			~TextureLoader();

			virtual void LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const override final;
			virtual void DestroyPayloadLoader() override final;

		private:
			ImportTga* _ImportTga = nullptr;
			ImportDds* _ImportDds = nullptr;
			TextureFactory& _Factory;
		};

	}
}
