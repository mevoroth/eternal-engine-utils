#pragma once

#include <vector>
#include "Resources/Streaming.hpp"
#include "Resources/TextureFactory.hpp"

namespace Eternal
{
	namespace Core
	{
		class Level;
	}
	namespace Components
	{
		class Mesh;
		class MeshCollection;
	}
	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Components;
		using namespace Eternal::Core;

		//////////////////////////////////////////////////////////////////////////

		struct MeshPayload : public Payload
		{
			MeshPayload(_In_ const MeshRequest* InMeshRequest);

			MeshCollection*					LoadedMesh	= nullptr;
			Mesh*							BoundingBox	= nullptr;
			vector<TextureFactoryRequest>	TextureRequests;
			vector<MeshComponent*> ComponentsToUpdate;
		};

		struct TexturePayload : public Payload
		{
			RawTextureData TextureData;
		};

		//////////////////////////////////////////////////////////////////////////

		struct MeshRequest : public StreamingRequest
		{
			MeshRequest(_In_ const string& InPath);

			vector<MeshComponent*> ComponentsToUpdate;
		};

		struct TextureRequest : public StreamingRequest
		{
			TextureRequest(_In_ const string& InPath);
		};
	}
}
