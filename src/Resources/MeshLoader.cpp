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

		void MeshLoader::LoadPayload(_In_ const StreamingRequest& InRequest, _Out_ Payload*& OutPayload) const
		{
			ETERNAL_PROFILER(BASIC)();
			OutPayload = new MeshPayload();
			*OutPayload = std::move(_ImportFbx->Import(InRequest.RequestPath));
			LogWrite(LogInfo, LogImport, string("Loaded [") + InRequest.RequestPath + "]");
		}

		void MeshLoader::DestroyPayloadLoader()
		{
			delete this;
		}
	}
}
