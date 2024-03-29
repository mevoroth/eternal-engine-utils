#pragma once

#include "Resources/Streaming.hpp"
#include "Resources/TextureFactory.hpp"
#include "Material/Material.hpp"
#include <vector>
#include <unordered_map>

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
			TextureType Slot			= TextureType::TEXTURE_TYPE_INVALID;
			TextureKey Key;
			string TextureName;
		};

		//////////////////////////////////////////////////////////////////////////

		struct MeshPayload : public Payload
		{
			MeshCollection*			LoadedMesh		= nullptr;
			Mesh*					BoundingBoxMesh	= nullptr;
			vector<MeshComponent*>	ComponentsToUpdate;
		};

		struct TexturePayload : public Payload
		{
			~TexturePayload();

			TextureKey Key;
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

		//////////////////////////////////////////////////////////////////////////

		struct MaterialUpdateBatch
		{
			RawTextureData TextureData;

			vector<MaterialUpdate> Materials;
		};

		struct MaterialUpdateBatcher
		{
			unordered_map<TextureKey, MaterialUpdateBatch> MaterialUpdates;
		};
	}
}
