#pragma once

#include "Resources/Streaming.hpp"

namespace Eternal
{
	namespace Import
	{
		class ImportTga;
	}

	namespace Resources
	{
		using namespace Eternal::Import;

		struct TextureLoader final : public StreamingLoader
		{
			TextureLoader();
			~TextureLoader();

			virtual void LoadPayload(_In_ const StreamingRequest& InRequest, _Out_ Payload*& OutPayload) const override final;
			virtual void DestroyPayloadLoader() override final;

		private:
			ImportTga* _ImportTga = nullptr;
		};

	}
}
