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
		class Material;
	}
	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Components;
		using namespace Eternal::Core;

		//////////////////////////////////////////////////////////////////////////

		struct MaterialUpdate
		{
			MaterialUpdate() {}
			MaterialUpdate(_In_ const TextureKey& InKey, _In_ const string& InTextureName)
				: Key(InKey)
				, TextureName(InTextureName)
			{
			}

			Material* MaterialToUpdate	= nullptr;
			uint32_t Slot				= ~0u;
			TextureKey Key;
			string TextureName;
		};

		//////////////////////////////////////////////////////////////////////////

		struct MeshPayload : public Payload
		{
			MeshCollection*			LoadedMesh	= nullptr;
			Mesh*					BoundingBox	= nullptr;
			vector<MeshComponent*>	ComponentsToUpdate;
		};

		struct TexturePayload : public Payload
		{
			~TexturePayload();

			RawTextureData TextureData;

			MaterialUpdate MaterialToUpdate;
		};

		//////////////////////////////////////////////////////////////////////////

		struct MeshRequest : public StreamingRequest
		{
			MeshRequest(_In_ const string& InPath);

			vector<MeshComponent*> ComponentsToUpdate;
		};

		struct TextureRequest : public StreamingRequest
		{
			TextureRequest(_In_ const string& InPath, _In_ const TextureKey& InKey, _In_ const string& InTextureName);

			MaterialUpdate MaterialToUpdate;
		};
	}
}
