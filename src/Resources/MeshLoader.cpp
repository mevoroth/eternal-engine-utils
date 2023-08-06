#include "Resources/MeshLoader.hpp"
#include "Import/fbx/ImportFbx.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Resources
	{
		MeshLoader::MeshLoader()
			: _ImportFbx(new ImportFbx())
		{
		}

		MeshLoader::~MeshLoader()
		{
			delete _ImportFbx;
			_ImportFbx = nullptr;
		}

		void MeshLoader::LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const
		{
			ETERNAL_PROFILER(BASIC)();
			const MeshRequest* InMeshRequest = static_cast<const MeshRequest*>(InRequest);

			MeshPayload* NewMeshPayload = new MeshPayload();
			_ImportFbx->Import(InRequest->RequestPath, *NewMeshPayload);
			NewMeshPayload->ComponentsToUpdate = std::move(InMeshRequest->ComponentsToUpdate);
			LogWrite(LogInfo, LogImport, string("Loaded [") + InRequest->RequestPath + "]");
			OutPayload = NewMeshPayload;
		}

		void MeshLoader::DestroyPayloadLoader()
		{
			delete this;
		}
	}
}
