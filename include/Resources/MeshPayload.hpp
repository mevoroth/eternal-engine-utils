#pragma once

#include <vector>
#include "Resources/TextureFactory.hpp"

namespace Eternal
{
	namespace Components
	{
		class Mesh;
	}
	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Components;

		struct MeshPayload
		{
			Mesh*							MeshObject	= nullptr;
			Mesh*							BoundingBox	= nullptr;
			vector<TextureFactoryRequest>	TextureRequests;
		};

	}
}
