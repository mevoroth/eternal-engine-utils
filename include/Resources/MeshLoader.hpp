#pragma once

#include "Resources/Streaming.hpp"

namespace Eternal
{
	namespace Import
	{
		class ImportFbx;
		class ImportMat;
	}

	namespace Resources
	{
		using namespace Eternal::Import;

		struct MeshLoader final : public StreamingLoader
		{
			MeshLoader(_In_ ImportMat* InImportMat);
			~MeshLoader();

			virtual void LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const override final;
			virtual void DestroyPayloadLoader() override final;

		private:
			ImportFbx* _ImportFbx = nullptr;
		};
	}
}
