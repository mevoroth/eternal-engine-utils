#pragma once

#include "Material/Material.hpp"
#include <string>
#include <map>

namespace Eternal
{
	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Components;

		enum class SerializationMode
		{
			SERIALIZATION_MODE_READ,
			SERIALIZATION_MODE_WRITE
		};

		using MeshKey = string;

		struct MaterialDependencyEntry
		{
			string TextureKey;
			string TexturePath;
			string TextureFullPath;
		};

		struct MaterialTextures
		{
			MaterialDependencyEntry Textures[static_cast<uint32_t>(TextureType::TEXTURE_TYPE_COUNT)];
		};

		struct MaterialDependency 
		{
			map<Components::Material*, MaterialTextures> Textures;
		};

		class MeshFactory
		{
		public:
			
			bool CachedMeshIsValid(_In_ const MeshKey& InKey) const;
			template<class MeshType>
			void SerializeMesh(_In_ SerializationMode InSerializationMode, _In_ const MeshKey& InKey, _Inout_ MaterialDependency& InOutMaterialDependency, _Inout_ MeshType& InOutMesh);
		};
	}
}
