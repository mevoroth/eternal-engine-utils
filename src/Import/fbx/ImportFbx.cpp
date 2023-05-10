#include "Import/fbx/ImportFbx.hpp"

#include <cstdio>
#include <functional>
#include "GraphicData/MeshVertexFormat.hpp"
#include "Transform/Transform.hpp"
#include "Material/Material.hpp"
#include "Mesh/GenericMesh.hpp"
#include "Mesh/AxisAlignedBoundingBox.hpp"
#include "File/FilePath.hpp"
#include "Math/Math.hpp"
#include "Log/Log.hpp"
#include "Resources/MeshFactory.hpp"

#define ETERNAL_IMPORT_FBX_LEFT_HANDED	(1)

namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::FileSystem;
		using namespace Eternal::GraphicData;
		using namespace Eternal::Components;
		using namespace Eternal::Math;
		using namespace Eternal::Import::ImportFbxPrivate;
		using namespace Eternal::Resources;

		namespace ImportFbxPrivate
		{
			static constexpr const char* DiffuseTextureName	= "3dsMax|Parameters|base_color_map";
			static constexpr const char* NormalTextureName	= "3dsMax|Parameters|bump_map";

			enum class MissingTexturePolicy
			{
				MISSING_TEXTURE_POLICY_NOTHING,
				MISSING_TEXTURE_POLICY_DEFAULT_TO_BLACK
			};

			using TextureKeyPathBuilderFunction = void (*)(_Inout_ char InOutTextureKey[255], _Inout_ char InOutTexturePath[1024]);

			static void TextureKeyPathBuilderDefault(_Inout_ char InOutTextureKey[255], _Inout_ char InOutTexturePath[1024])
			{
				sprintf_s(InOutTexturePath, sizeof(char) * 1024, "%s.tga", InOutTextureKey);
			}

			static void TextureKeyPathBuilderRoughnessMetalness(_Inout_ char InOutTextureKey[255], _Inout_ char InOutTexturePath[1024])
			{
				if (char* PrefixEnd = strstr(InOutTextureKey, "_BaseColor"))
					*PrefixEnd = 0;

				sprintf_s(InOutTexturePath, sizeof(char) * 1024, "%s_Roughness%s_Metalness.tga", InOutTextureKey, InOutTextureKey);
				sprintf_s(InOutTextureKey, sizeof(char) * 255, "%s_RoughnessMetalness", InOutTextureKey);
			}

			template<TextureKeyPathBuilderFunction TextureKeyPathBuilderFunctor>
			static string TextureKeyPathFullPathBuilder(_In_ const FbxFileTexture* FileTexture, _Inout_ char InOutFileName[255], _Inout_ char InOutTexturePath[1024])
			{
				string TextureFullPath;

				const FbxString TextureFileName = FileTexture->GetFileName();

				const char* FilePathCandidates[] =
				{
					TextureFileName.Buffer(),
					FileTexture->GetName()
				};

				for (int FilePathCandidateIndex = 0; TextureFullPath.size() == 0 && FilePathCandidateIndex < ETERNAL_ARRAYSIZE(FilePathCandidates); ++FilePathCandidateIndex)
				{
					char Extension[8];
					_splitpath_s(
						FilePathCandidates[FilePathCandidateIndex],
						nullptr, 0,
						nullptr, 0,
						InOutFileName, 255,
						Extension, 8
					);

					TextureKeyPathBuilderFunctor(InOutFileName, InOutTexturePath);

					TextureFullPath = FilePath::Find(InOutTexturePath, FileType::FILE_TYPE_TEXTURES);
				}

				return TextureFullPath;
			}

			template<MissingTexturePolicy Policy, TextureKeyPathBuilderFunction TextureKeyPathBuilderFunctor = TextureKeyPathBuilderDefault>
			static void ReadTextureDependencyFromFbxFileTexture(_In_ const FbxFileTexture* FileTexture, _In_ const FbxTextureCache& InTextureCache, _In_ TextureType InTextureType, _In_ Components::Material* InMaterial, _Inout_ MaterialDependency& InOutMaterialDependency)
			{
				char FileName[255];
				char TexturePath[1024];
				string TextureFullPath;

				uint32_t PathLength = static_cast<uint32_t>(strlen(FileTexture->GetName()));
				if (PathLength > 0)
				{
					memcpy(FileName, FileTexture->GetName(), PathLength + 1);
					memcpy(TexturePath, FileTexture->GetName(), PathLength + 1);
					TextureKeyPathBuilderFunctor(FileName, TexturePath);
				}

				auto TextureCacheEntry = InTextureCache.TextureCache.find(FileName);

				if (TextureCacheEntry != InTextureCache.TextureCache.end())
				{
					memcpy(FileName, TextureCacheEntry->second.TextureKey.c_str(), TextureCacheEntry->second.TextureKey.length() + 1);
					memcpy(TexturePath, TextureCacheEntry->second.TexturePath.c_str(), TextureCacheEntry->second.TexturePath.length() + 1);
					TextureFullPath	= TextureCacheEntry->second.TextureFullPath;
				}
				else
				{
					TextureFullPath = TextureKeyPathFullPathBuilder<TextureKeyPathBuilderFunctor>(FileTexture, FileName, TexturePath);
				}

				auto TextureDependencyEntry = InOutMaterialDependency.Textures.find(InMaterial);
				if (TextureDependencyEntry == InOutMaterialDependency.Textures.end())
					TextureDependencyEntry = InOutMaterialDependency.Textures.emplace(InMaterial, MaterialTextures()).first;

				MaterialDependencyEntry& Textures = TextureDependencyEntry->second.Textures[static_cast<uint32_t>(InTextureType)];

				if (TextureFullPath.size() > 0)
				{
					Textures.TextureFullPath	= TextureFullPath;
					Textures.TextureKey			= FileName;
					Textures.TexturePath		= TexturePath;
				}
				else
				{
					if (Policy == MissingTexturePolicy::MISSING_TEXTURE_POLICY_DEFAULT_TO_BLACK)
					{
						Textures.TextureFullPath	= FilePath::Find("black.tga", FileType::FILE_TYPE_TEXTURES);
						Textures.TextureKey			= "black";
						Textures.TexturePath		= "black";
					}
				}

				//TextureRequest* NewTextureRequest = nullptr;
				//if (TextureFullPath.size() > 0)
				//{
				//	NewTextureRequest = new TextureRequest(TextureFullPath, FileName, TexturePath);

				//	//LogWrite(LogWarning, LogImport, TextureFullPath);
				//}
				//else
				//{
				//	if (Policy == MissingTexturePolicy::MISSING_TEXTURE_POLICY_DEFAULT_TO_BLACK)
				//		NewTextureRequest = new TextureRequest(FilePath::Find("black.tga", FileType::FILE_TYPE_TEXTURES), "black", "black");

				//	LogWrite(LogInfo, LogImport, string("Missing texture: [") + FileName + "] " + TexturePath);
				//}

				//if (NewTextureRequest)
				//	InOutMeshPayload.AddRequest(NewTextureRequest);
				//
				//return NewTextureRequest;
			}

			template<TextureKeyPathBuilderFunction TextureKeyPathBuilderFunctor>
			void ReadMaterialDependencyFromFbxMaterialProperty(
				_In_ const FbxSurfaceMaterial* InFbxMaterial,
				_In_ const FbxTextureCache& InTextureCache,
				_In_ const char* InPropertyName,
				_In_ TextureType InTextureType,
				_In_ Components::Material* InMaterial,
				_Inout_ MaterialDependency& InOutMaterialDependency
			)
			{
				//TextureRequest* NewTextureRequest = nullptr;
				bool HasFoundTexture = false;
				const FbxProperty Property = InFbxMaterial->FindPropertyHierarchical(InPropertyName);
				if (Property.IsValid())
				{
					if (Property.GetSrcObjectCount<FbxFileTexture>() > 0)
					{
						const FbxFileTexture* FileTexture = Property.GetSrcObject<FbxFileTexture>();

						ReadTextureDependencyFromFbxFileTexture<MissingTexturePolicy::MISSING_TEXTURE_POLICY_DEFAULT_TO_BLACK, TextureKeyPathBuilderFunctor>(
							FileTexture,
							InTextureCache,
							InTextureType,
							InMaterial,
							InOutMaterialDependency
						);
						HasFoundTexture = true;

						//if (NewTextureRequest->MaterialToUpdate.Key == "black")
						{
							//int SrcObjectCount = Property.GetSrcObjectCount();
							//for (int SrcObjectIndex = 0; SrcObjectIndex < SrcObjectCount; ++SrcObjectIndex)
							//{
							//	FbxObject* FbxObj = Property.GetSrcObject(SrcObjectIndex);
							//	LogWrite(LogWarning, LogImport, std::string("Property: ") + FbxObj->GetName());
							//}

							//FbxProperty Property = InFbxMaterial->GetFirstProperty();
							//for (; Property.IsValid(); Property = InFbxMaterial->GetNextProperty(Property))
							//{
							//	if (Property.IsValid())
							//	{
							//		FbxTexture* lTexture = NULL;

							//		//Here we have to check if it's layered textures, or just textures:
							//		int LayeredTextureCount = Property.GetSrcObjectCount<FbxLayeredTexture>();
							//		FbxString PropertyName = Property.GetName();

							//		FbxProperty FindProperty = InFbxMaterial->FindProperty(Property.GetNameAsCStr(), false);

							//		if (FindProperty.IsValid())
							//		{
							//			LogWrite(LogWarning, LogImport, "VALID");
							//		}

							//		LogWrite(LogWarning, LogImport, Property.GetHierarchicalName().Buffer());

							//		if (LayeredTextureCount > 0)
							//		{
							//			for (int LayerIndex = 0; LayerIndex < LayeredTextureCount; ++LayerIndex)
							//			{
							//				FbxLayeredTexture* lLayeredTexture = Property.GetSrcObject<FbxLayeredTexture>(LayerIndex);
							//				int NbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
							//				for (int TexIndex = 0; TexIndex < NbTextures; ++TexIndex)
							//				{
							//					FbxFileTexture* Texture = lLayeredTexture->GetSrcObject<FbxFileTexture>(TexIndex);
							//					if (Texture)
							//					{
							//						LogWrite(LogWarning, LogImport, std::string(PropertyName.Buffer()) + std::string(" ") + Texture->GetName());
							//					}
							//				}
							//				if (NbTextures == 0)
							//					LogWrite(LogWarning, LogImport, std::string(PropertyName.Buffer()) + " EMPTY");
							//			}
							//		}
							//		else
							//		{
							//			//no layered texture simply get on the property
							//			int NbTextures = Property.GetSrcObjectCount<FbxTexture>();
							//			for (int TexIndex = 0; TexIndex < NbTextures; ++TexIndex)
							//			{

							//				FbxFileTexture* Texture = Property.GetSrcObject<FbxFileTexture>(TexIndex);
							//				if (Texture)
							//				{
							//					LogWrite(LogWarning, LogImport, std::string(PropertyName.Buffer()) + std::string(" ") + Texture->GetName());
							//				}
							//			}
							//			if (NbTextures == 0)
							//				LogWrite(LogWarning, LogImport, std::string(PropertyName.Buffer()) + " EMPTY");
							//		}
							//	}
							//}
						}
					}
				}

				if (!HasFoundTexture)
				{
					auto TextureDependencyEntry = InOutMaterialDependency.Textures.find(InMaterial);
					if (TextureDependencyEntry == InOutMaterialDependency.Textures.end())
						TextureDependencyEntry = InOutMaterialDependency.Textures.emplace(InMaterial, MaterialTextures()).first;

					MaterialDependencyEntry& Textures = TextureDependencyEntry->second.Textures[static_cast<uint32_t>(InTextureType)];
					Textures.TextureFullPath	= FilePath::Find("black.tga", FileType::FILE_TYPE_TEXTURES);
					Textures.TextureKey			= "black";
					Textures.TexturePath		= "black";
				}

				//if (!NewTextureRequest)
				//{
				//	NewTextureRequest = new TextureRequest(FilePath::Find("black.tga", FileType::FILE_TYPE_TEXTURES), "black", "black");
				//	InOutMeshPayload.AddRequest(NewTextureRequest);
				//}

				//NewTextureRequest->MaterialToUpdate.MaterialToUpdate	= InMaterial;
				//NewTextureRequest->MaterialToUpdate.Slot				= InTextureType;
			}

			static void LeftHandedTransformVector(_Inout_ Vector3& InOutVector3)
			{
				InOutVector3.x = ETERNAL_IMPORT_FBX_LEFT_HANDED ? -InOutVector3.x : InOutVector3.x;
			}

			static void LeftHandedTransformUV(_Inout_ Vector2& InOutUV)
			{
				if (ETERNAL_IMPORT_FBX_LEFT_HANDED)
					InOutUV.y = 1.0f - InOutUV.y;
			}

			static void LeftHandedTransformEulerRotation(_Inout_ Vector3& InOutRotation)
			{
				InOutRotation.y = ETERNAL_IMPORT_FBX_LEFT_HANDED ? -InOutRotation.y : InOutRotation.y;
				InOutRotation.z = ETERNAL_IMPORT_FBX_LEFT_HANDED ? -InOutRotation.z : InOutRotation.z;
			}

			class Mesh
			{
			public:

				void ReserveVertices(_In_ uint32_t VerticesCount) { _Vertices.reserve(VerticesCount); }
				void ReserveIndices(_In_ uint32_t IndicesCount) { _Indices.reserve(IndicesCount); }
				void PushVertex(_In_ const PositionNormalTangentBinormalUVVertex& InVertex)
				{
					_Vertices.push_back(InVertex);
					_BoundingBox.Extend(InVertex.Position);
				}

				void PushTriangle(_In_ uint16_t InVertex0, _In_ uint16_t InVertex1, _In_ uint16_t InVertex2)
				{
					_Indices.push_back(InVertex0);
					_Indices.push_back(InVertex1);
					_Indices.push_back(InVertex2);
				}

				Transform& GetTransform() { return _Transform; }
				const Transform& GetTransform() const { return _Transform; }
				Transform& GetLocalTransform() { return _LocalTransform; }
				const Transform& GetLocalTransform() const { return _LocalTransform; }
				const AxisAlignedBoundingBox& GetBoundingBox() const { return _BoundingBox; }

				const Mesh& GetSubMesh(_In_ uint32_t InIndex) const
				{
					ETERNAL_ASSERT(InIndex < _SubMeshes.size());
					return _SubMeshes[InIndex];
				}
				Mesh& GetSubMesh(_In_ uint32_t InIndex)
				{
					ETERNAL_ASSERT(InIndex < _SubMeshes.size());
					return _SubMeshes[InIndex];
				}
				Mesh& GetLastSubMesh()
				{
					ETERNAL_ASSERT(_SubMeshes.size() > 0);
					return _SubMeshes.back();
				}

				size_t GetSubMeshesCount() const { return _SubMeshes.size(); }
				void PushMesh(_In_ const Mesh& InSubMesh) { _SubMeshes.push_back(InSubMesh); }
				void SetMaterial(_In_ Components::Material* InMaterial) { _Material = InMaterial; }
				Components::Material* GetMaterial() const { return _Material; }
				bool IsValid() const { return _Vertices.size() > 0; }
				const vector<uint16_t>& GetIndices() const { return _Indices; }
				const vector<PositionNormalTangentBinormalUVVertex>& GetVertices() const { return _Vertices; }
				PositionNormalTangentBinormalUVVertex& GetVertex(uint32_t InIndex) { return _Vertices[InIndex]; }

			protected:
				vector<uint16_t>								_Indices;
				vector<PositionNormalTangentBinormalUVVertex>	_Vertices;
				vector<Mesh>									_SubMeshes;
				Transform										_Transform;
				Transform										_LocalTransform;
				AxisAlignedBoundingBox							_BoundingBox;
				Components::Material*							_Material = nullptr;
			};

			struct FbxTextureCache
			{
				map<string, MaterialDependencyEntry> TextureCache;
			};
		}

		ImportFbx::ImportFbx()
		{
			_SdkManager = FbxManager::Create();
			_Settings = FbxIOSettings::Create(_SdkManager, IOSROOT);

			_SdkManager->SetIOSettings(_Settings);
			_FbxImporter = FbxImporter::Create(_SdkManager, "");

			LogWrite(LogWarning, LogImport, "[ImportFbx::ImportFbx]UV.y has been inversed!");
			LogWrite(LogWarning, LogImport, "[ImportFbx::ImportFbx]Pos.w = 1.f!");
		}

		MeshPayload ImportFbx::Import(_In_ const std::string& InPath)
		{
			MeshPayload OutMeshPayload;
			MeshFactory MeshCache;
			MaterialDependency MaterialTextures;

			std::string InFullFilePath = FilePath::Find(InPath, FileType::FILE_TYPE_MESHES);

			if (MeshCache.CachedMeshIsValid(InFullFilePath))
			{
				OutMeshPayload.LoadedMesh = new MeshCollection();
				MeshCache.SerializeMesh(SerializationMode::SERIALIZATION_MODE_READ, InFullFilePath, MaterialTextures, OutMeshPayload);
			}
			else
			{
				if (_FbxImporter->Initialize(InFullFilePath.c_str(), -1, _Settings))
				{
					FbxStatus& Status = _FbxImporter->GetStatus();
					if (Status.GetCode() != FbxStatus::eSuccess)
					{
						std::string ErrorMessage = "[ImportFbx::Import]";
						ErrorMessage.append(Status.GetErrorString());
						LogWrite(LogWarning, LogImport, ErrorMessage.c_str());
						return OutMeshPayload;
					}
				}

				ImportFbxPrivate::FbxTextureCache TextureCache;
				ImportFbxPrivate::Mesh IntermediateMesh;

				AxisAlignedBoundingBox Box;
				Box.SetMin(Vector3(std::numeric_limits<float>::infinity()));
				Box.SetMax(Vector3(-std::numeric_limits<float>::infinity()));

				FbxScene* Scene = FbxScene::Create(_SdkManager, "scene");
				_FbxImporter->Import(Scene);

				FbxAxisSystem EternalAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
				FbxAxisSystem SceneAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

				if (SceneAxisSystem != EternalAxisSystem)
					EternalAxisSystem.ConvertScene(Scene);

				FbxSystemUnit SceneSystemUnit = Scene->GetGlobalSettings().GetSystemUnit();
				if (SceneSystemUnit.GetScaleFactor() != 1.0)
					FbxSystemUnit::m.ConvertScene(Scene);

				FbxGeometryConverter GeometryConverter(_SdkManager);
				bool TriangulateSuccess = GeometryConverter.Triangulate(Scene, /*pReplace=*/ true);

				if (!TriangulateSuccess)
				{
					LogWrite(LogWarning, LogImport, "[ImportFbx::Import]Triangulate failed");
					return OutMeshPayload;
				}

				_ImportTextures(Scene, TextureCache, OutMeshPayload);
				_ImportNode(Scene->GetRootNode(), TextureCache, IntermediateMesh, Box, MaterialTextures, OutMeshPayload);

				Scene->Destroy(true);

				//_Flatten_Combine(IntermediateMesh, OutMeshPayload);
				_Flatten_Split_SingleMesh(IntermediateMesh, OutMeshPayload);
				//_Flatten_Split_MultipleMeshes(IntermediateMesh, OutMeshPayload);

				//GenericMesh<PositionColorVertex>* BoundingBoxMesh = new GenericMesh<PositionColorVertex>();
				//BoundingBoxMesh->AddMesh(
				//	{
				//		2, 0, 3,	0, 1, 3, // -X
				//		4, 6, 5,	6, 7, 5, // +X
				//		0, 4, 1,	4, 5, 1, // -Y
				//		6, 2, 7,	2, 3, 7, // +Y
				//		2, 6, 0,	6, 4, 0, // -Z
				//		1, 5, 3,	5, 7, 3  // +Z
				//	},
				//	{
				//		{ Vector4(Box.GetMin().x, Box.GetMin().y, Box.GetMin().z, 1.0f), 0x000000FF },
				//		{ Vector4(Box.GetMin().x, Box.GetMin().y, Box.GetMax().z, 1.0f), 0x0000FFFF },
				//		{ Vector4(Box.GetMin().x, Box.GetMax().y, Box.GetMin().z, 1.0f), 0x00FF00FF },
				//		{ Vector4(Box.GetMin().x, Box.GetMax().y, Box.GetMax().z, 1.0f), 0x00FFFFFF },
				//		{ Vector4(Box.GetMax().x, Box.GetMin().y, Box.GetMin().z, 1.0f), 0xFF0000FF },
				//		{ Vector4(Box.GetMax().x, Box.GetMin().y, Box.GetMax().z, 1.0f), 0xFF00FFFF },
				//		{ Vector4(Box.GetMax().x, Box.GetMax().y, Box.GetMin().z, 1.0f), 0xFFFF00FF },
				//		{ Vector4(Box.GetMax().x, Box.GetMax().y, Box.GetMax().z, 1.0f), 0xFFFFFFFF },
				//	},
				//	IntermediateMesh.GetLocalTransform().GetViewToWorld() * IntermediateMesh.GetTransform().GetViewToWorld(),
				//	nullptr
				//);

				//OutMeshPayload.BoundingBoxMesh = BoundingBoxMesh;

				MeshCache.SerializeMesh(SerializationMode::SERIALIZATION_MODE_WRITE, InFullFilePath, MaterialTextures, OutMeshPayload);
			}

			_CreateTextureRequests(MaterialTextures, OutMeshPayload);

			for (uint32_t MeshIndex = 0; MeshIndex < OutMeshPayload.LoadedMesh->Meshes.size(); ++MeshIndex)
				OutMeshPayload.LoadedMesh->Meshes[MeshIndex]->SetName(InPath);

			return OutMeshPayload;
		}

		void ImportFbx::_ImportTextures(_In_ FbxScene* InScene, _Inout_ ImportFbxPrivate::FbxTextureCache& InOutTextureCache, _Inout_ MeshPayload& InOutMeshPayload)
		{
			const int TextureCount = InScene->GetTextureCount();
			for (int TextureIndex = 0; TextureIndex < TextureCount; ++TextureIndex)
			{
				FbxTexture* Texture = InScene->GetTexture(TextureIndex);
				FbxFileTexture* FileTexture = FbxCast<FbxFileTexture>(Texture);

				if (InOutTextureCache.TextureCache.find(FileTexture->GetName()) == InOutTextureCache.TextureCache.end())
				{
					char FileName[255];
					char TexturePath[1024];
					string TextureFullPath = TextureKeyPathFullPathBuilder<TextureKeyPathBuilderDefault>(FileTexture, FileName, TexturePath);

					if (!TextureFullPath.empty())
					{
						MaterialDependencyEntry NewTextureCacheEntry = { FileName, TexturePath, TextureFullPath };
						InOutTextureCache.TextureCache.emplace(FileTexture->GetName(), NewTextureCacheEntry);
					}
				}

				//LogWrite(LogWarning, LogImport, FileTexture->GetName());
				//LogWrite(LogWarning, LogImport, FileTexture->GetFileName());
				//CreateTextureRequestFromFbxFileTexture<MissingTexturePolicy::MISSING_TEXTURE_POLICY_NOTHING>(FileTexture, InOutMeshPayload);
			}
		}

		void ImportFbx::_CreateTextureRequests(_In_ const MaterialDependency& InMaterialDependency, _Inout_ MeshPayload& InOutMeshPayload)
		{
			for (auto TextureDependencyIterator = InMaterialDependency.Textures.begin(); TextureDependencyIterator != InMaterialDependency.Textures.end(); ++TextureDependencyIterator)
			{
				//TextureDependencyIterator->first->
				const MaterialTextures& Textures = TextureDependencyIterator->second;
				for (uint32_t TextureIndex = 0; TextureIndex < static_cast<uint32_t>(TextureType::TEXTURE_TYPE_COUNT); ++TextureIndex)
				{
					const MaterialDependencyEntry& CurrentEntry = Textures.Textures[TextureIndex];
					TextureRequest* NewTextureRequest = new TextureRequest(
						CurrentEntry.TextureFullPath,
						CurrentEntry.TextureKey,
						CurrentEntry.TexturePath
					);
					NewTextureRequest->MaterialToUpdate.MaterialToUpdate	= TextureDependencyIterator->first;
					NewTextureRequest->MaterialToUpdate.Slot				= static_cast<TextureType>(TextureIndex);
					InOutMeshPayload.AddRequest(NewTextureRequest);
				}
			}
		}

		struct MeshNodeContext
		{
			Matrix4x4 WorldTransformMatrix = Matrix4x4::Identity;
		};

		static void ImportFbx_Flatten_Combine_Node(_In_ const ImportFbxPrivate::Mesh& InMesh, _In_ const MeshNodeContext& InMeshNodeContext, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext CurrentNodeContext =
			{
				InMesh.GetLocalTransform().GetViewToWorld()* InMesh.GetTransform().GetViewToWorld()
			};

			if (InMesh.IsValid())
			{
				GenericMesh<PositionNormalTangentBinormalUVVertex>* InOutMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex>*>(InOutMeshPayload.LoadedMesh->Meshes.back());

				uint32_t FlattenedVerticesCount = InOutMesh->GetVerticesCount();
				ETERNAL_ASSERT((InMesh.GetVertices().size() + FlattenedVerticesCount) < InOutMesh->GetIndicesMaxCount());
				InOutMesh->AddMergeMesh(InMesh.GetIndices(), InMesh.GetVertices(), CurrentNodeContext.WorldTransformMatrix, InMesh.GetMaterial(), InMesh.GetBoundingBox());
			}

			for (uint32_t SubMeshIndex = 0; SubMeshIndex < InMesh.GetSubMeshesCount(); ++SubMeshIndex)
				ImportFbx_Flatten_Combine_Node(InMesh.GetSubMesh(SubMeshIndex), CurrentNodeContext, InOutMeshPayload);
		}

		void ImportFbx::_Flatten_Combine(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext RootNodeContext;
			InOutMeshPayload.LoadedMesh = new MeshCollection();
			InOutMeshPayload.LoadedMesh->Meshes.push_back(new GenericMesh<PositionNormalTangentBinormalUVVertex>());
			ImportFbx_Flatten_Combine_Node(InMesh, RootNodeContext, InOutMeshPayload);
		}

		template<bool IsMultipleMeshes = false>
		static void ImportFbx_Flatten_Split_Node(_In_ const ImportFbxPrivate::Mesh& InMesh, _In_ const MeshNodeContext& InMeshNodeContext, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext CurrentNodeContext =
			{
				InMesh.GetLocalTransform().GetViewToWorld() * InMesh.GetTransform().GetViewToWorld()
			};

			if (InMesh.IsValid())
			{
				if (IsMultipleMeshes)
					InOutMeshPayload.LoadedMesh->Meshes.push_back(new GenericMesh<PositionNormalTangentBinormalUVVertex>());
				GenericMesh<PositionNormalTangentBinormalUVVertex>* InOutMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex>*>(InOutMeshPayload.LoadedMesh->Meshes.back());
				InOutMesh->AddMesh(InMesh.GetIndices(), InMesh.GetVertices(), CurrentNodeContext.WorldTransformMatrix, InMesh.GetMaterial(), InMesh.GetBoundingBox());
			}

			for (uint32_t SubMeshIndex = 0; SubMeshIndex < InMesh.GetSubMeshesCount(); ++SubMeshIndex)
				ImportFbx_Flatten_Split_Node<IsMultipleMeshes>(InMesh.GetSubMesh(SubMeshIndex), CurrentNodeContext, InOutMeshPayload);
		}

		void ImportFbx::_Flatten_Split_SingleMesh(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext RootNodeContext;
			InOutMeshPayload.LoadedMesh = new MeshCollection();
			InOutMeshPayload.LoadedMesh->Meshes.push_back(new GenericMesh<PositionNormalTangentBinormalUVVertex>());
			ImportFbx_Flatten_Split_Node(InMesh, RootNodeContext, InOutMeshPayload);
		}

		void ImportFbx::_Flatten_Split_MultipleMeshes(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext RootNodeContext;
			InOutMeshPayload.LoadedMesh = new MeshCollection();
			ImportFbx_Flatten_Split_Node<true>(InMesh, RootNodeContext, InOutMeshPayload);
		}

		void ImportFbx::_ImportNode(_In_ FbxNode* InNode, _In_ const ImportFbxPrivate::FbxTextureCache& InTextureCache, _Inout_ ImportFbxPrivate::Mesh& InOutMesh, _Inout_ AxisAlignedBoundingBox& InOutBoundingBox, _Inout_ MaterialDependency& InOutMaterialDependency, _Inout_ MeshPayload& InOutMeshPayload)
		{
			static constexpr int VERTICES_PER_TRIANGLE = 3;

			FbxNodeAttribute* Attribute = InNode->GetNodeAttribute();

			FbxAMatrix& GlobalTransform = InNode->EvaluateGlobalTransform();
			FbxVector4 GlobalTranslation = GlobalTransform.GetT();
			FbxVector4 GlobalRotation = GlobalTransform.GetR();
			FbxVector4 GlobalScaling = GlobalTransform.GetS();

			Vector3 GlobalTranslationTransformed(
				static_cast<float>(GlobalTranslation[0]),
				static_cast<float>(GlobalTranslation[1]),
				static_cast<float>(GlobalTranslation[2])
			);
			LeftHandedTransformVector(GlobalTranslationTransformed);
			InOutMesh.GetTransform().Translate(GlobalTranslationTransformed);

			Vector3 GlobalRotationRadians(
				static_cast<float>(GlobalRotation[0] * DegreesToRadians),
				static_cast<float>(GlobalRotation[1] * DegreesToRadians),
				static_cast<float>(GlobalRotation[2] * DegreesToRadians)
			);
			LeftHandedTransformEulerRotation(GlobalRotationRadians);
			InOutMesh.GetTransform().Rotate(GlobalRotationRadians);

			InOutMesh.GetTransform().Scale(
				Vector3(
					static_cast<float>(GlobalScaling[0]),
					static_cast<float>(GlobalScaling[1]),
					static_cast<float>(GlobalScaling[2])
				)
			);

			if (Attribute)
			{
				FbxVector4 LocalTranslation = InNode->GetGeometricTranslation(FbxNode::eSourcePivot);
				FbxVector4 LocalRotation = InNode->GetGeometricRotation(FbxNode::eSourcePivot);
				FbxVector4 LocalScaling = InNode->GetGeometricScaling(FbxNode::eSourcePivot);

				Vector3 LocalTranslationTransformed(
					static_cast<float>(LocalTranslation[0]),
					static_cast<float>(LocalTranslation[1]),
					static_cast<float>(LocalTranslation[2])
				);
				LeftHandedTransformVector(LocalTranslationTransformed);
				InOutMesh.GetLocalTransform().Translate(LocalTranslationTransformed);

				Vector3 LocalRotationRadians(
					static_cast<float>(LocalRotation[0] * DegreesToRadians),
					static_cast<float>(LocalRotation[1] * DegreesToRadians),
					static_cast<float>(LocalRotation[2] * DegreesToRadians)
				);
				LeftHandedTransformVector(LocalRotationRadians);
				InOutMesh.GetLocalTransform().Rotate(LocalRotationRadians);

				InOutMesh.GetLocalTransform().Scale(
					Vector3(
						static_cast<float>(LocalScaling[0]),
						static_cast<float>(LocalScaling[1]),
						static_cast<float>(LocalScaling[2])
					)
				);

				FbxNodeAttribute::EType AttributeType = Attribute->GetAttributeType();
				switch (AttributeType)
				{
				case FbxNodeAttribute::EType::eMesh: {
					FbxMesh* InFbxMesh = static_cast<FbxMesh*>(Attribute);

					const int PolygonsCount = InFbxMesh->GetPolygonCount();

					struct FbxMeshMetaData
					{
						int IndexOffset		= 0;
						int TriangleCount	= 0;
					};

					vector<FbxMeshMetaData> FbxSubMeshes;
					FbxSubMeshes.reserve(8);

					FbxLayerElementArrayTemplate<int>* MaterialIndices		= nullptr;
					FbxGeometryElement::EMappingMode MaterialMappingMode	= FbxGeometryElement::eNone;

					if (InFbxMesh->GetElementMaterial())
					{
						MaterialIndices		= &InFbxMesh->GetElementMaterial()->GetIndexArray();
						MaterialMappingMode	= InFbxMesh->GetElementMaterial()->GetMappingMode();

						if (MaterialIndices && MaterialMappingMode == FbxGeometryElement::eByPolygon)
						{
							ETERNAL_ASSERT(MaterialIndices->GetCount() == PolygonsCount);
							for (int PolygonIndex = 0; PolygonIndex < PolygonsCount; ++PolygonIndex)
							{
								const int MaterialIndex = MaterialIndices->GetAt(PolygonIndex);
								if (FbxSubMeshes.size() < MaterialIndex + 1)
									FbxSubMeshes.resize(MaterialIndex + 1);

								++FbxSubMeshes[MaterialIndex].TriangleCount;
							}

							const int MaterialCount = static_cast<int>(FbxSubMeshes.size());

							int FbxIndexOffset = 0;
							for (int MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
							{
								FbxSubMeshes[MaterialIndex].IndexOffset = FbxIndexOffset;
								FbxIndexOffset += FbxSubMeshes[MaterialIndex].TriangleCount * VERTICES_PER_TRIANGLE;
								FbxSubMeshes[MaterialIndex].TriangleCount = 0;
							}
							ETERNAL_ASSERT(FbxIndexOffset == PolygonsCount * VERTICES_PER_TRIANGLE);
						}
					}

					if (FbxSubMeshes.size() == 0)
						FbxSubMeshes.push_back({});

					bool AllByControlPoints	= true;
					bool HasNormals			= InFbxMesh->GetElementNormalCount() > 0;
					bool HasTangents		= InFbxMesh->GetElementTangentCount() > 0;
					bool HasBinormals		= InFbxMesh->GetElementBinormalCount() > 0;
					bool HasUVs				= InFbxMesh->GetElementUVCount() > 0;

					if (HasNormals && !(HasTangents && HasBinormals))
					{
						bool GenerateTangentsAndBinormalsSuccess = InFbxMesh->GenerateTangentsDataForAllUVSets();
						ETERNAL_ASSERT(GenerateTangentsAndBinormalsSuccess);
						HasTangents		= GenerateTangentsAndBinormalsSuccess && InFbxMesh->GetElementTangentCount() > 0;
						HasBinormals	= GenerateTangentsAndBinormalsSuccess && InFbxMesh->GetElementBinormalCount() > 0;
					}

					FbxGeometryElement::EMappingMode NormalMappingMode		= FbxGeometryElement::eNone;
					FbxGeometryElement::EMappingMode TangentMappingMode		= FbxGeometryElement::eNone;
					FbxGeometryElement::EMappingMode BinormalMappingMode	= FbxGeometryElement::eNone;
					FbxGeometryElement::EMappingMode UVMappingMode			= FbxGeometryElement::eNone;
					
					auto SetupSubMeshComponentElement = [&AllByControlPoints](_In_ const std::function<FbxGeometryElement::EMappingMode()>& InGetMappingModeFunction, _Inout_ bool& HasComponent, _Inout_ FbxGeometryElement::EMappingMode& InOutComponentMappingMode)
					{
						if (HasComponent)
						{
							InOutComponentMappingMode = InGetMappingModeFunction();
							if (InOutComponentMappingMode == FbxGeometryElement::eNone)
							{
								HasComponent = false;
								ETERNAL_BREAK();
							}

							if (HasComponent && InOutComponentMappingMode != FbxGeometryElement::eByControlPoint)
								AllByControlPoints = false;
						}
					};

					SetupSubMeshComponentElement(
						[InFbxMesh]() -> FbxGeometryElement::EMappingMode
						{
							return InFbxMesh->GetElementNormal()->GetMappingMode();
						},
						HasNormals, NormalMappingMode
					);
					SetupSubMeshComponentElement(
						[InFbxMesh]() -> FbxGeometryElement::EMappingMode
						{
							return InFbxMesh->GetElementTangent()->GetMappingMode();
						},
						HasTangents, TangentMappingMode
					);
					SetupSubMeshComponentElement(
						[InFbxMesh]() -> FbxGeometryElement::EMappingMode
						{
							return InFbxMesh->GetElementBinormal()->GetMappingMode();
						},
						HasBinormals, BinormalMappingMode
					);
					SetupSubMeshComponentElement(
						[InFbxMesh]() -> FbxGeometryElement::EMappingMode
						{
							return InFbxMesh->GetElementUV()->GetMappingMode();
						},
						HasUVs, UVMappingMode
					);

					FbxStringList UVNames;
					InFbxMesh->GetUVSetNames(UVNames);
					const char* UVName = NULL;
					if (HasUVs && UVNames.GetCount())
					{
						UVName = UVNames[0];
					}

					FbxVector4* FbxControlPoints = InFbxMesh->GetControlPoints();

					auto GetSubMeshComponentElement = [](_In_ bool HasComponentElement, _In_ const auto* InComponentElement, _In_ int InControlPointIndex, _Inout_ Vector3& InOutVertexComponent) -> void
					{
						if (!HasComponentElement)
							return;

						int ComponentIndex = InControlPointIndex;
						if (InComponentElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
							ComponentIndex = InComponentElement->GetIndexArray().GetAt(InControlPointIndex);
						FbxVector4 FbxVertexComponent = InComponentElement->GetDirectArray().GetAt(ComponentIndex);
						InOutVertexComponent = Vector3(
							static_cast<float>(FbxVertexComponent[0]),
							static_cast<float>(FbxVertexComponent[1]),
							static_cast<float>(FbxVertexComponent[2])
						);
						LeftHandedTransformVector(InOutVertexComponent);
					};

					if (AllByControlPoints)
					{
						ETERNAL_BREAK();

						ETERNAL_ASSERT(FbxSubMeshes.size() == 1);

						for (int ControlPointIndex = 0, ControlPointsCount = InFbxMesh->GetControlPointsCount(); ControlPointIndex < ControlPointsCount; ++ControlPointIndex)
						{
							PositionNormalTangentBinormalUVVertex Vertex;
							Vertex.Position = Vector3(
								static_cast<float>(FbxControlPoints[ControlPointIndex][0]),
								static_cast<float>(FbxControlPoints[ControlPointIndex][1]),
								static_cast<float>(FbxControlPoints[ControlPointIndex][2])
							);
							LeftHandedTransformVector(Vertex.Position);

							GetSubMeshComponentElement(HasNormals, InFbxMesh->GetElementNormal(), ControlPointIndex, Vertex.Normal);
							GetSubMeshComponentElement(HasTangents, InFbxMesh->GetElementTangent(), ControlPointIndex, Vertex.Tangent);
							GetSubMeshComponentElement(HasBinormals, InFbxMesh->GetElementBinormal(), ControlPointIndex, Vertex.Binormal);

							if (HasUVs)
							{
								int ComponentIndex = ControlPointIndex;
								if (InFbxMesh->GetElementUV()->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
									ComponentIndex = InFbxMesh->GetElementUV()->GetIndexArray().GetAt(ControlPointIndex);
								FbxVector2 FbxVertexUV = InFbxMesh->GetElementUV()->GetDirectArray().GetAt(ComponentIndex);
								Vertex.UV = Vector2(
									static_cast<float>(FbxVertexUV[0]),
									static_cast<float>(FbxVertexUV[1])
								);
								LeftHandedTransformUV(Vertex.UV);
							}

							InOutMesh.PushVertex(Vertex);
						}
					}
					else
					{
						auto CreateTextureRequestFromMaterialProperty = [&InTextureCache, &InOutMaterialDependency](_In_ const FbxSurfaceMaterial* InFbxMaterial, _In_ const char* InPropertyName, _In_ TextureType InTextureType, _In_ Components::Material* InMaterial) -> void
						{
							ReadMaterialDependencyFromFbxMaterialProperty<TextureKeyPathBuilderDefault>(InFbxMaterial, InTextureCache, InPropertyName, InTextureType, InMaterial, InOutMaterialDependency);
						};

						auto CreateTextureRequestFromMaterialProperty_RoughnessMetallic = [&InTextureCache, &InOutMaterialDependency](_In_ const FbxSurfaceMaterial* InFbxMaterial, _In_ TextureType InTextureType, _In_ Components::Material* InMaterial) -> void
						{
							ReadMaterialDependencyFromFbxMaterialProperty<TextureKeyPathBuilderRoughnessMetalness>(InFbxMaterial, InTextureCache, ImportFbxPrivate::DiffuseTextureName, InTextureType, InMaterial, InOutMaterialDependency);
						};

						Mesh* SubMeshes		= reinterpret_cast<Mesh*>(alloca(sizeof(Mesh) * FbxSubMeshes.size()));
						int* VerticesCount	= reinterpret_cast<int*>(alloca(sizeof(int) * FbxSubMeshes.size()));

						memset(SubMeshes, 0x0, sizeof(Mesh) * FbxSubMeshes.size());

						for (uint32_t SubMeshIndex = 0; SubMeshIndex < FbxSubMeshes.size(); ++SubMeshIndex)
						{
							SubMeshes[SubMeshIndex]						= Mesh();
							SubMeshes[SubMeshIndex].GetTransform()		= InOutMesh.GetTransform();
							SubMeshes[SubMeshIndex].GetLocalTransform()	= InOutMesh.GetLocalTransform();

							SubMeshes[SubMeshIndex].PushMesh(Mesh());
							Mesh& CurrentSubMesh		= SubMeshes[SubMeshIndex].GetLastSubMesh();
							CurrentSubMesh.GetTransform()				= InOutMesh.GetTransform();
							CurrentSubMesh.GetLocalTransform()			= InOutMesh.GetLocalTransform();
							CurrentSubMesh.ReserveVertices(PolygonsCount * VERTICES_PER_TRIANGLE);
							CurrentSubMesh.ReserveIndices(PolygonsCount * VERTICES_PER_TRIANGLE);

							VerticesCount[SubMeshIndex]					= 0;
						}

						//int VertexCount = 0;
						for (int PolygonIndex = 0; PolygonIndex < PolygonsCount; ++PolygonIndex)
						{
							int MaterialIndex = 0;
							if (MaterialIndices && MaterialMappingMode == FbxGeometryElement::eByPolygon)
							{
								MaterialIndex = MaterialIndices->GetAt(PolygonIndex);
							}

							const int IndexOffset = FbxSubMeshes[MaterialIndex].IndexOffset +
								FbxSubMeshes[MaterialIndex].TriangleCount * VERTICES_PER_TRIANGLE;

							int PolygonSize = InFbxMesh->GetPolygonSize(PolygonIndex);

							if (VerticesCount[MaterialIndex] + PolygonSize * VERTICES_PER_TRIANGLE >= std::numeric_limits<uint16_t>::max())
							{
								SubMeshes[MaterialIndex].PushMesh(Mesh());
								Mesh& CurrentSubMesh	= SubMeshes[MaterialIndex].GetLastSubMesh();
								CurrentSubMesh.GetTransform()			= InOutMesh.GetTransform();
								CurrentSubMesh.GetLocalTransform()		= InOutMesh.GetLocalTransform();

								CurrentSubMesh.ReserveVertices((PolygonsCount - PolygonIndex) * VERTICES_PER_TRIANGLE);
								CurrentSubMesh.ReserveIndices((PolygonsCount - PolygonIndex) * VERTICES_PER_TRIANGLE);

								VerticesCount[MaterialIndex]			= 0;
							}

							Mesh& CurrentSplitSubMesh = SubMeshes[MaterialIndex].GetLastSubMesh();

							uint16_t* Indices = reinterpret_cast<uint16_t*>(alloca(sizeof(uint16_t) * PolygonSize));
							for (int VertexIndex = 0; VertexIndex < PolygonSize; ++VertexIndex)
							{
								const int ControlPointIndex = InFbxMesh->GetPolygonVertex(PolygonIndex, VertexIndex);
								
								PositionNormalTangentBinormalUVVertex Vertex;
								if (ControlPointIndex >= 0)
								{
									Indices[VertexIndex] = static_cast<uint16_t>(VerticesCount[MaterialIndex]);

									Vertex.Position = Vector3(
										static_cast<float>(FbxControlPoints[ControlPointIndex][0]),
										static_cast<float>(FbxControlPoints[ControlPointIndex][1]),
										static_cast<float>(FbxControlPoints[ControlPointIndex][2])
									);
									LeftHandedTransformVector(Vertex.Position);

									if (HasNormals)
									{
										FbxVector4 FbxVertexNormal;
										bool IsValidNormal = InFbxMesh->GetPolygonVertexNormal(PolygonIndex, VertexIndex, FbxVertexNormal);
										ETERNAL_ASSERT(IsValidNormal);
										Vertex.Normal = Vector3(
											static_cast<float>(FbxVertexNormal[0]),
											static_cast<float>(FbxVertexNormal[1]),
											static_cast<float>(FbxVertexNormal[2])
										);
										LeftHandedTransformVector(Vertex.Normal);
									}

									GetSubMeshComponentElement(HasTangents, InFbxMesh->GetElementTangent(), ControlPointIndex, Vertex.Tangent);
									GetSubMeshComponentElement(HasBinormals, InFbxMesh->GetElementBinormal(), ControlPointIndex, Vertex.Binormal);

									if (HasUVs)
									{
										bool UnmappedUV = false;
										FbxVector2 FbxVertexUV;
										bool IsValidUV = InFbxMesh->GetPolygonVertexUV(PolygonIndex, VertexIndex, UVName, FbxVertexUV, UnmappedUV);
										ETERNAL_ASSERT(!UnmappedUV && IsValidUV);
										Vertex.UV = Vector2(
											static_cast<float>(FbxVertexUV[0]),
											static_cast<float>(FbxVertexUV[1])
										);
										LeftHandedTransformUV(Vertex.UV);
									}

									CurrentSplitSubMesh.PushVertex(Vertex);
								}

								//LogWrite(LogWarning, LogImport, string("[") + to_string(Vertex.Position.x) + ":" + to_string(Vertex.Position.y) + ":" + to_string(Vertex.Position.z) + "] " + to_string(ControlPointIndex) + " : " + to_string(Indices[VertexIndex]) + " : " + to_string(MaterialIndex) + " : " + to_string(PolygonIndex) + " : " + to_string(VertexIndex));

								++VerticesCount[MaterialIndex];
							}
							for (int TriangleIndex = 0, TriangleCount = PolygonSize - 2; TriangleIndex < TriangleCount; ++TriangleIndex)
							{
								CurrentSplitSubMesh.PushTriangle(
									Indices[0],
									Indices[TriangleIndex + 1],
									Indices[TriangleIndex + 2]
								);
							}

							++FbxSubMeshes[MaterialIndex].TriangleCount;
						}

						for (uint32_t SubMeshIndex = 0; SubMeshIndex < FbxSubMeshes.size(); ++SubMeshIndex)
						{
							Mesh& CurrentSubMesh = SubMeshes[SubMeshIndex];

							bool IsValidSubMesh = false;
							for (uint32_t SplitSubMeshIndex = 0; !IsValidSubMesh && SplitSubMeshIndex < CurrentSubMesh.GetSubMeshesCount(); ++SplitSubMeshIndex)
								IsValidSubMesh |= CurrentSubMesh.GetSubMesh(SplitSubMeshIndex).IsValid();

							if (IsValidSubMesh)
							{
								Components::Material* MeshMaterial = new Components::Material();
								
								const FbxSurfaceMaterial* InFbxMaterial = InNode->GetMaterial(SubMeshIndex);

								CreateTextureRequestFromMaterialProperty(InFbxMaterial, ImportFbxPrivate::DiffuseTextureName,	TextureType::TEXTURE_TYPE_DIFFUSE,						MeshMaterial);
								CreateTextureRequestFromMaterialProperty(InFbxMaterial, ImportFbxPrivate::NormalTextureName,	TextureType::TEXTURE_TYPE_NORMAL,						MeshMaterial);
								CreateTextureRequestFromMaterialProperty_RoughnessMetallic(InFbxMaterial,						TextureType::TEXTURE_TYPE_ROUGHNESS_METALLIC_SPECULAR,	MeshMaterial);

								for (uint32_t SplitSubMeshIndex = 0; SplitSubMeshIndex < CurrentSubMesh.GetSubMeshesCount(); ++SplitSubMeshIndex)
								{
									Mesh& CurrentSplitSubMesh = CurrentSubMesh.GetSubMesh(SplitSubMeshIndex);
									CurrentSplitSubMesh.SetMaterial(MeshMaterial);
									InOutMesh.PushMesh(CurrentSplitSubMesh);
								}
							}
						}
					}
				} break;

				//case FbxNodeAttribute::EType::eSkeleton: {

				//} break;
				}
			}

			for (int NodeChildIndex = 0, NodeChildCount = InNode->GetChildCount(); NodeChildIndex < NodeChildCount; ++NodeChildIndex)
			{
				ImportFbxPrivate::Mesh SubMesh;

				_ImportNode(InNode->GetChild(NodeChildIndex), InTextureCache, SubMesh, InOutBoundingBox, InOutMaterialDependency, InOutMeshPayload);

				InOutMesh.PushMesh(SubMesh);
			}
		}

		void ImportFbx::_ImportPoses(_In_ FbxScene* Scene)
		{
			int PoseCount = Scene->GetPoseCount();
			for (int PoseIndex = 0; PoseIndex < PoseCount; ++PoseIndex)
			{
				FbxPose* Pose = Scene->GetPose(PoseIndex);
			}
		}

		void ImportFbx::_ImportSkeletal(_In_ FbxMesh* InMesh)
		{
			bool HasVertexCache = InMesh->GetDeformerCount(FbxDeformer::eVertexCache) && ((FbxVertexCacheDeformer*)InMesh->GetDeformer(0, FbxDeformer::eVertexCache))->Active.Get();
			bool HasShape = InMesh->GetShapeCount() > 0;
			bool HasDeformation = InMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;


			if (HasVertexCache)
			{
				// Vertex cache?
			}
			else
			{
				if (HasShape)
				{
					_GetBlendShape(InMesh);
				}
				_GetSkinning(InMesh);
			}
		}

		void ImportFbx::_GetBlendShape(_In_ FbxMesh* InMesh)
		{
		}

		void ImportFbx::_GetSkinning(_In_ FbxMesh* InMesh)
		{
			return;
			//int DeformerCount = MeshObj->GetDeformerCount(FbxDeformer::eSkin);
			//for (int DeformerIndex = 0; DeformerIndex < DeformerCount; ++DeformerIndex)
			//{
			//	FbxStatus Status;
			//	FbxSkin* Skin = (FbxSkin*)MeshObj->GetDeformer(DeformerIndex, FbxDeformer::eSkin, &Status);
			//	Bone* BoneObj = new Bone();

			//	int ClusterCount = Skin->GetClusterCount();
			//	for (int ClusterIndex = 0; ClusterIndex < ClusterCount; ++ClusterIndex)
			//	{

			//		FbxCluster* Cluster = Skin->GetCluster(ClusterIndex);
			//		int ControlPointIndicesCount = Cluster->GetControlPointIndicesCount();

			//		for (int ControlPointIndicesIndex = 0; ControlPointIndicesIndex < ControlPointIndicesCount; ++ControlPointIndicesIndex)
			//		{
			//			BoneObj->PushInfluence(
			//				Cluster->GetControlPointIndices()[ControlPointIndicesIndex],
			//				static_cast<float>(Cluster->GetControlPointWeights()[ControlPointIndicesIndex])
			//			);
			//		}
			//	}
			//}
		}
	}
}
