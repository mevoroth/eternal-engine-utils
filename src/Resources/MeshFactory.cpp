#include "Resources/MeshFactory.hpp"
#include "File/FileFactory.hpp"
#include "File/FilePath.hpp"
#include "File/File.hpp"
#include "Mesh/GenericMesh.hpp"
#include "Resources/Payload.hpp"
#include <string>

namespace Eternal
{
	namespace Resources
	{
		using namespace Eternal::GraphicData;
		using namespace Eternal::Components;
		using namespace Eternal::FileSystem;

		namespace MeshFactoryPrivate
		{
			enum class CachedMeshFileVersion : uint32_t
			{
				CACHED_MESH_VERSION_INITIAL,
				CACHED_MESH_VERSION_INVALID,
				CACHED_MESH_VERSION_LAST = CACHED_MESH_VERSION_INVALID - 1
			};

			struct CachedMeshFileMetaData
			{
				CachedMeshFileMetaData(_In_ SerializationMode InSerializationMode)
					: FileVersion(InSerializationMode == SerializationMode::SERIALIZATION_MODE_READ ? CachedMeshFileVersion::CACHED_MESH_VERSION_INVALID : CachedMeshFileVersion::CACHED_MESH_VERSION_LAST)
				{
				}

				CachedMeshFileMetaData() = delete;

				CachedMeshFileVersion FileVersion = CachedMeshFileVersion::CACHED_MESH_VERSION_INVALID;
			};
		}

		static std::string MeshFactory_KeyToCachedMeshFilePath(_In_ const MeshKey& InKey)
		{
			string Dummy;
			string FileName;
			string Extension;

			FilePath::SplitPath(InKey, Dummy, FileName, Extension);

			(void)Dummy;

			char CachedMeshFilePath[1024];
			sprintf_s(CachedMeshFilePath, "%s%scache", FileName.c_str(), Extension.c_str());

			return CachedMeshFilePath;
		}

		bool MeshFactory::CachedMeshIsValid(_In_ const MeshKey& InKey) const
		{
			using namespace Eternal::Resources::MeshFactoryPrivate;

			std::string CachedMeshFilePath = FilePath::Find(MeshFactory_KeyToCachedMeshFilePath(InKey), FileType::FILE_TYPE_CACHED_MESHES);
			bool IsCachedMeshValid = FileSystem::FileExists(CachedMeshFilePath);

			if (IsCachedMeshValid)
			{
				FileScope CachedMeshFile(CachedMeshFilePath, File::OpenMode::READ);

				CachedMeshFileMetaData FileMetaData(SerializationMode::SERIALIZATION_MODE_READ);
				CachedMeshFile->Serialize(reinterpret_cast<uint8_t*>(&FileMetaData), sizeof(CachedMeshFileMetaData));
				IsCachedMeshValid &= FileMetaData.FileVersion == MeshFactoryPrivate::CachedMeshFileVersion::CACHED_MESH_VERSION_LAST;
			}

			return IsCachedMeshValid;
		}

		template<> void MeshFactory::SerializeMesh<MeshPayload>(_In_ SerializationMode InSerializationMode, _In_ const MeshKey& InKey, _Inout_ MaterialDependency& InOutMaterialDependency, _Inout_ MeshPayload& InOutMesh)
		{
			using namespace Eternal::Resources::MeshFactoryPrivate;

			std::string CachedMeshFilePath = FilePath::FindOrCreate(MeshFactory_KeyToCachedMeshFilePath(InKey), FileType::FILE_TYPE_CACHED_MESHES);
			FileScope CachedMeshFile(
				CachedMeshFilePath,
				InSerializationMode == SerializationMode::SERIALIZATION_MODE_READ ? File::OpenMode::READ : File::OpenMode::WRITE
			);
			
			CachedMeshFileMetaData FileMetaData(InSerializationMode);
			CachedMeshFile->Serialize(FileMetaData);

			ETERNAL_ASSERT(FileMetaData.FileVersion >= MeshFactoryPrivate::CachedMeshFileVersion::CACHED_MESH_VERSION_INITIAL);

			CachedMeshFile->Serialize(
				InOutMesh.LoadedMesh->Meshes,
				[]() -> GenericMesh<PositionNormalTangentBinormalUVVertex>* { return new GenericMesh<PositionNormalTangentBinormalUVVertex>(); },
				[&CachedMeshFile, &InSerializationMode, &InOutMaterialDependency](_Inout_ Mesh* InOutMesh)
				{
					GenericMesh<PositionNormalTangentBinormalUVVertex>* InOutGenericMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex>*>(InOutMesh);

					uint32_t PerDrawInformationCount = static_cast<uint32_t>(InOutGenericMesh->GetGPUMesh().PerDrawInformations.size());
					CachedMeshFile->Serialize(PerDrawInformationCount);
					InOutGenericMesh->GetGPUMesh().PerDrawInformations.resize(PerDrawInformationCount);
					InOutGenericMesh->GetGPUMesh().BoundingBoxes.resize(PerDrawInformationCount);

					for (uint32_t PerDrawInformationIndex = 0; PerDrawInformationIndex < InOutGenericMesh->GetGPUMesh().PerDrawInformations.size(); ++PerDrawInformationIndex)
					{
						GPUMesh::PerDrawInformation& CurrentPerDrawInformation = InOutGenericMesh->GetGPUMesh().PerDrawInformations[PerDrawInformationIndex];
						CachedMeshFile->Serialize(InOutGenericMesh->GetGPUMesh().BoundingBoxes[PerDrawInformationIndex]);
						CachedMeshFile->Serialize(CurrentPerDrawInformation.IndicesOffset);
						CachedMeshFile->Serialize(CurrentPerDrawInformation.IndicesCount);
						CachedMeshFile->Serialize(CurrentPerDrawInformation.VerticesOffset);

						if (!CurrentPerDrawInformation.PerDrawMaterial)
						{
							ETERNAL_ASSERT(InSerializationMode == SerializationMode::SERIALIZATION_MODE_READ);
							CurrentPerDrawInformation.PerDrawMaterial = new Material();
						}

						auto TexturesDependency = InOutMaterialDependency.Textures.find(CurrentPerDrawInformation.PerDrawMaterial);
						if (TexturesDependency == InOutMaterialDependency.Textures.end())
							TexturesDependency = InOutMaterialDependency.Textures.emplace(CurrentPerDrawInformation.PerDrawMaterial, MaterialTextures()).first;

						MaterialTextures& CurrentMaterialTextures = TexturesDependency->second;
						for (uint32_t TextureIndex = 0; TextureIndex < static_cast<uint32_t>(TextureType::TEXTURE_TYPE_COUNT); ++TextureIndex)
						{
							CachedMeshFile->Serialize(CurrentMaterialTextures.Textures[TextureIndex].TextureKey);
							CachedMeshFile->Serialize(CurrentMaterialTextures.Textures[TextureIndex].TexturePath);
							CachedMeshFile->Serialize(CurrentMaterialTextures.Textures[TextureIndex].TextureFullPath);
						}
					}

					CachedMeshFile->Serialize(InOutGenericMesh->GetPerDrawConstants());
					CachedMeshFile->Serialize(InOutGenericMesh->GetIndices());
					CachedMeshFile->Serialize(InOutGenericMesh->GetVertices());
				}
			);
		}
	}
}
