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
	}
	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Components;
		using namespace Eternal::Core;

		struct MeshPayload : public Payload
		{
			Mesh*							LoadedMesh	= nullptr;
			Mesh*							BoundingBox	= nullptr;
			vector<TextureFactoryRequest>	TextureRequests;
		};

		struct TexturePayload : public Payload
		{
			RawTextureData TextureData;
		};
	}
}
