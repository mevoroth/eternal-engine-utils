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

			OutPayload = new MeshPayload();
			*static_cast<MeshPayload*>(OutPayload) = std::move(_ImportFbx->Import(InRequest->RequestPath));
			static_cast<MeshPayload*>(OutPayload)->ComponentsToUpdate = std::move(InMeshRequest->ComponentsToUpdate);
			LogWrite(LogInfo, LogImport, string("Loaded [") + InRequest->RequestPath + "]");
		}

		void MeshLoader::DestroyPayloadLoader()
		{
			delete this;
		}
	}
}
