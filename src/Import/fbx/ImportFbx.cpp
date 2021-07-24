#include "Import/fbx/ImportFbx.hpp"

#include <cstdio>
#include "GraphicData/MeshVertexFormat.hpp"
#include "Transform/Transform.hpp"
#include "Mesh/GenericMesh.hpp"
#include "File/FilePath.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::FileSystem;
		using namespace Eternal::GraphicData;

		namespace ImportFbxPrivate
		{
			class Mesh
			{
			public:

				void PushVertex(_In_ const PositionNormalTangentBinormalUVVertex& V)
				{
					_Vertices.push_back(V);
				}

				void PushTriangle(_In_ uint16_t V0, _In_ uint16_t V1, _In_ uint16_t V2)
				{
					_Indices.push_back(V0);
					_Indices.push_back(V1);
					_Indices.push_back(V2);
				}

				Transform& GetTransform()
				{
					return _Transform;
				}
				const Transform& GetTransform() const
				{
					return _Transform;
				}

				const Mesh& GetSubMesh(_In_ uint32_t Index) const
				{
					ETERNAL_ASSERT(Index < _SubMeshes.size());
					return _SubMeshes[Index];
				}

				size_t GetSubMeshesCount() const
				{
					return _SubMeshes.size();
				}

				void PushMesh(_In_ Mesh SubMesh)
				{
					_SubMeshes.push_back(SubMesh);
				}

				bool IsValid() const { return _Vertices.size() > 0; }

				void SetTexture(_In_ const TextureType& InTextureType, _In_ const string& InTextureKey)
				{
					_Maps[static_cast<uint32_t>(InTextureType)] = InTextureKey;
				}

				const vector<uint16_t>& GetIndices() const { return _Indices; }
				const vector<PositionNormalTangentBinormalUVVertex>& GetVertices() const { return _Vertices; }
				PositionNormalTangentBinormalUVVertex& GetVertex(uint32_t Index)
				{
					return _Vertices[Index];
				}

			protected:
				vector<uint16_t>														_Indices;
				vector<PositionNormalTangentBinormalUVVertex>							_Vertices;
				vector<Mesh>															_SubMeshes;
				Transform																_Transform;
				array<string, static_cast<uint32_t>(TextureType::TEXTURE_TYPE_COUNT)>	_Maps;
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

			std::string InFullFilePath = FilePath::Find(InPath, FileType::FILE_TYPE_MESHES);

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

			ImportFbxPrivate::Mesh IntermediateMesh;

			BoundingBox Box;
			Box.SetMin(Vector3(std::numeric_limits<float>::infinity()));
			Box.SetMax(Vector3(-std::numeric_limits<float>::infinity()));

			FbxScene* Scene = FbxScene::Create(_SdkManager, "scene");
			_FbxImporter->Import(Scene);

			_ImportNode(Scene->GetRootNode(), IntermediateMesh, Box, OutMeshPayload);

			Scene->Destroy(true);

			//_Flatten_Combine(IntermediateMesh, OutMeshPayload);
			_Flatten_Split(IntermediateMesh, OutMeshPayload);

			GenericMesh<PositionColorVertex>* BoundingBoxMesh = new GenericMesh<PositionColorVertex>();
			BoundingBoxMesh->Add(
				{
					2, 0, 3,	0, 1, 3, // -X
					4, 6, 5,	6, 7, 5, // +X
					0, 4, 1,	4, 5, 1, // -Y
					6, 2, 7,	2, 3, 7, // +Y
					2, 6, 0,	6, 4, 0, // -Z
					1, 5, 3,	5, 7, 3  // +Z
				},
				{
					{ Vector4(Box.GetMin().x, Box.GetMin().y, Box.GetMin().z, 1.0f), 0x000000FF },
					{ Vector4(Box.GetMin().x, Box.GetMin().y, Box.GetMax().z, 1.0f), 0x0000FFFF },
					{ Vector4(Box.GetMin().x, Box.GetMax().y, Box.GetMin().z, 1.0f), 0x00FF00FF },
					{ Vector4(Box.GetMin().x, Box.GetMax().y, Box.GetMax().z, 1.0f), 0x00FFFFFF },
					{ Vector4(Box.GetMax().x, Box.GetMin().y, Box.GetMin().z, 1.0f), 0xFF0000FF },
					{ Vector4(Box.GetMax().x, Box.GetMin().y, Box.GetMax().z, 1.0f), 0xFF00FFFF },
					{ Vector4(Box.GetMax().x, Box.GetMax().y, Box.GetMin().z, 1.0f), 0xFFFF00FF },
					{ Vector4(Box.GetMax().x, Box.GetMax().y, Box.GetMax().z, 1.0f), 0xFFFFFFFF },
				},
				IntermediateMesh.GetTransform().GetModelMatrix()
			);

			OutMeshPayload.BoundingBox = BoundingBoxMesh;

			return OutMeshPayload;
		}

		struct MeshNodeContext
		{
			Matrix4x4 WorldTransformMatrix = Matrix4x4::Identity;
		};

		static void ImportFbx_Flatten_Combine_Node(_In_ const ImportFbxPrivate::Mesh& InMesh, _In_ const MeshNodeContext& InMeshNodeContext, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext CurrentNodeContext =
			{
				InMeshNodeContext.WorldTransformMatrix * InMesh.GetTransform().GetModelMatrix()
			};

			if (InMesh.IsValid())
			{
				GenericMesh<PositionNormalTangentBinormalUVVertex>* InOutMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex>*>(InOutMeshPayload.LoadedMesh->Meshes.back());

				uint32_t FlattenedVerticesCount = InOutMesh->GetVerticesCount();
				ETERNAL_ASSERT((InMesh.GetVertices().size() + FlattenedVerticesCount) < InOutMesh->GetIndicesMaxCount());
				InOutMesh->AddMerge(InMesh.GetIndices(), InMesh.GetVertices(), CurrentNodeContext.WorldTransformMatrix);
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

		static void ImportFbx_Flatten_Split_Node(_In_ const ImportFbxPrivate::Mesh& InMesh, _In_ const MeshNodeContext& InMeshNodeContext, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext CurrentNodeContext =
			{
				InMeshNodeContext.WorldTransformMatrix * InMesh.GetTransform().GetModelMatrix()
			};

			if (InMesh.IsValid())
			{
				GenericMesh<PositionNormalTangentBinormalUVVertex>* InOutMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex>*>(InOutMeshPayload.LoadedMesh->Meshes.back());
				InOutMesh->Add(InMesh.GetIndices(), InMesh.GetVertices(), CurrentNodeContext.WorldTransformMatrix);
			}

			for (uint32_t SubMeshIndex = 0; SubMeshIndex < InMesh.GetSubMeshesCount(); ++SubMeshIndex)
				ImportFbx_Flatten_Split_Node(InMesh.GetSubMesh(SubMeshIndex), CurrentNodeContext, InOutMeshPayload);
		}

		void ImportFbx::_Flatten_Split(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload)
		{
			MeshNodeContext RootNodeContext;
			InOutMeshPayload.LoadedMesh = new MeshCollection();
			InOutMeshPayload.LoadedMesh->Meshes.push_back(new GenericMesh<PositionNormalTangentBinormalUVVertex>());
			ImportFbx_Flatten_Split_Node(InMesh, RootNodeContext, InOutMeshPayload);
		}

		//void ImportFbx::_ImportTextureFromFBX(_In_ FbxSurfaceMaterial* SurfaceMaterial, _In_ const Channel& ChannelIndex, _In_ const char* TextureSuffix, _Out_ Texture*& OutTexture)
		//{
		//	FbxProperty ColorChannel = SurfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[ChannelIndex]);
		//	if (ColorChannel.IsValid())
		//	{
		//		FbxTexture* TextureObj = ColorChannel.GetSrcObject<FbxTexture>(0);
		//		if (TextureObj)
		//		{
		//			char FileName[255];
		//			char Extension[8];
		//			_splitpath_s(FbxCast<FbxFileTexture>(TextureObj)->GetFileName(), nullptr, 0, nullptr, 0, FileName, 255, Extension, 8);
		//			sprintf_s(FileName, "%s%s%s", FileName, Extension, TextureSuffix);
		//			OutTexture = Eternal::Resources::TextureFactory::Get()->GetTexture(FileName);
		//		}
		//	}
		//}

		void ImportFbx::_ImportNode(_In_ const FbxNode* InNode, _Inout_ ImportFbxPrivate::Mesh& InOutMesh, _Inout_ BoundingBox& InOutBoundingBox, _Inout_ MeshPayload& InOutMeshPayload)
		{
			const FbxNodeAttribute* Attribute = InNode->GetNodeAttribute();
			if (Attribute)
			{
				FbxNodeAttribute::EType AttributeType = Attribute->GetAttributeType();
				switch (AttributeType)
				{
				case FbxNodeAttribute::EType::eMesh: {
					FbxMesh* InFbxMesh = (FbxMesh*)Attribute;
					FbxVector4* FbxControlPoint = InFbxMesh->GetControlPoints();
					PositionNormalTangentBinormalUVVertex Vertex;
					for (int ControlPointIndex = 0, ControlPointsCount = InFbxMesh->GetControlPointsCount(); ControlPointIndex < ControlPointsCount; ++ControlPointIndex)
					{
						Vertex.Position = Vector4(
							static_cast<float>(FbxControlPoint[ControlPointIndex][0]),
							static_cast<float>(FbxControlPoint[ControlPointIndex][1]), 
							static_cast<float>(FbxControlPoint[ControlPointIndex][2]),
							1.0f
						);
						Vertex.Normal = Vector3::Up;

						Vector3 Vertex3(
							static_cast<float>(FbxControlPoint[ControlPointIndex][0]),
							static_cast<float>(FbxControlPoint[ControlPointIndex][1]),
							static_cast<float>(FbxControlPoint[ControlPointIndex][2])
						);

						Vector3 NewMin = Min(InOutBoundingBox.GetMin(), InOutBoundingBox.GetMax());
						Vector3 NewMax = Max(InOutBoundingBox.GetMin(), InOutBoundingBox.GetMax());

						InOutBoundingBox.SetMin(NewMin);
						InOutBoundingBox.SetMax(NewMax);

						InOutMesh.PushVertex(Vertex);
					}

					int VertexId = 0;

					for (int PolygonIndex = 0, PolygonCount = InFbxMesh->GetPolygonCount(); PolygonIndex < PolygonCount; ++PolygonIndex)
					{
						int PolygonSize = InFbxMesh->GetPolygonSize(PolygonIndex);
						InOutMesh.PushTriangle(
							static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 0)),
							static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 1)),
							static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 2))
						);
						if (PolygonSize == 4) // Quad
						{
							InOutMesh.PushTriangle(
								static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 0)),
								static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 2)),
								static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 3))
							);
						}
						ETERNAL_ASSERT(PolygonSize <= 4);

						for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
						{
							int ControlPointIndex = InFbxMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);

							_ImportNode_GetNormal(InFbxMesh, ControlPointIndex, VertexId, InOutMesh);
							_ImportNode_GetTangent(InFbxMesh, ControlPointIndex, VertexId, InOutMesh);
							_ImportNode_GetBinormal(InFbxMesh, ControlPointIndex, VertexId, InOutMesh);

							++VertexId;
						}
						_ImportNode_GetUV(InFbxMesh, PolygonIndex, InOutMesh);
					}

					//_ImportSkeletal(InFbxMesh);

					{
						int MaterialCount = InNode->GetSrcObjectCount<FbxSurfaceMaterial>();
						ETERNAL_ASSERT(MaterialCount == 1);
						if (InNode->GetSrcObjectCount<FbxSurfaceMaterial>() > 0)
						{
							FbxSurfaceMaterial* Material = InNode->GetSrcObject<FbxSurfaceMaterial>(0);
							if (Material)
							{
								//for (int ChannelIndex = 0; ChannelIndex < FbxLayerElement::sTypeTextureCount; ++ChannelIndex)
								int ChannelIndex = 0;
								{
									FbxProperty ChannelProperty = Material->FindProperty(FbxLayerElement::sTextureChannelNames[ChannelIndex]);
									if (ChannelProperty.IsValid())
									{
										
										FbxTexture* TextureObject = ChannelProperty.GetSrcObject<FbxTexture>();
										if (TextureObject)
										{
											ETERNAL_ASSERT(ChannelProperty.GetSrcObjectCount() == 1);

											char FileName[255];
											char Extension[8];
											_splitpath_s(
												FbxCast<FbxFileTexture>(TextureObject)->GetFileName(),
												nullptr, 0,
												nullptr, 0,
												FileName, 255,
												Extension, 8
											);
											char* PrefixEnd = strstr(FileName, "_diff");

											if (PrefixEnd)
												*PrefixEnd = 0;

											auto RequestTexture = [&FileName, &Extension, &InOutMesh, &InOutMeshPayload](_In_ const char* MapName, _In_ const TextureType& InTextureType)
											{
												char TextureKey[1024];
												char TexturePath[1024];

												sprintf_s(TextureKey, "%s_%s", FileName, MapName);
												sprintf_s(TexturePath, "%s_%s%s", FileName, MapName, Extension);

												TextureFactoryRequest TextureRequest(TextureKey, TexturePath);
												if (FilePath::Find(TexturePath, FileType::FILE_TYPE_TEXTURES).size() > 0)
												{
													InOutMeshPayload.TextureRequests.push_back(TextureRequest);
													InOutMesh.SetTexture(InTextureType, TextureKey);
												}
												else
												{
													LogWrite(LogInfo, LogImport, string("Missing texture: [") + TextureKey + "] " + TexturePath);
												}
											};

											RequestTexture("diffuse", TextureType::TEXTURE_TYPE_DIFFUSE);
											RequestTexture("normal", TextureType::TEXTURE_TYPE_NORMAL);
										}
										else
										{
											LogWrite(LogInfo, LogImport, string("Failed to get texture on: ") + ChannelProperty.GetNameAsCStr());
										}
									}
								}
							}
						}
					}
				} break;

				//case FbxNodeAttribute::EType::eSkeleton: {

				//} break;
				}
			}
			InOutMesh.GetTransform().Translate(
				Vector3(
					static_cast<float>(InNode->LclTranslation.Get()[0]),
					static_cast<float>(InNode->LclTranslation.Get()[1]),
					static_cast<float>(InNode->LclTranslation.Get()[2])
				)
			);

			InOutMesh.GetTransform().Rotate(
				Vector3(
					static_cast<float>(InNode->LclRotation.Get()[0]),
					static_cast<float>(InNode->LclRotation.Get()[1]),
					static_cast<float>(InNode->LclRotation.Get()[2])
				)
			);

			InOutMesh.GetTransform().Scale(
				Vector3(
					static_cast<float>(InNode->LclScaling.Get()[0]),
					static_cast<float>(InNode->LclScaling.Get()[1]),
					static_cast<float>(InNode->LclScaling.Get()[2])
				)
			);

			for (int NodeChildIndex = 0; NodeChildIndex < InNode->GetChildCount(); ++NodeChildIndex)
			{
				ImportFbxPrivate::Mesh SubMesh;
		
				_ImportNode(InNode->GetChild(NodeChildIndex), SubMesh, InOutBoundingBox, InOutMeshPayload);
		
				InOutMesh.PushMesh(SubMesh);
			}
		}

		void ImportFbx::_ImportNode_GetUV(_In_ FbxMesh * InMesh, _In_ uint32_t InPolygonIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh)
		{
			for (uint32_t VertexIndex = 0, VerticesCount = InMesh->GetPolygonSize(InPolygonIndex); VertexIndex < VerticesCount; ++VertexIndex)
			{
				uint32_t Vertex = InMesh->GetPolygonVertex(InPolygonIndex, VertexIndex);

				FbxVector2 UV(0.0, 0.0);

				if (InMesh->GetLayer(0) && InMesh->GetLayer(0)->GetUVSets().GetCount() > 0)
				{
					bool UnMapped;
					bool Ret = InMesh->GetPolygonVertexUV(InPolygonIndex, VertexIndex, InMesh->GetLayer(0)->GetUVSets()[0]->GetName(), UV, UnMapped);
					ETERNAL_ASSERT(Ret);
					ETERNAL_ASSERT(!UnMapped);
				}
				//ETERNAL_BREAK();

				InOutMesh.GetVertex(Vertex).UV = Vector2(
					static_cast<float>(UV[0]),
					static_cast<float>(1.0f - UV[1])
				);
			}
		}

		void ImportFbx::_ImportNode_GetNormal(_In_ FbxMesh * InMesh, _In_ int InControlPointIndex, _In_ int VertexIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh)
		{
			for (int NormalIndex = 0, NormalCount = InMesh->GetElementNormalCount(); NormalIndex < NormalCount; ++NormalIndex)
			{
				FbxGeometryElementNormal* NormalElement = InMesh->GetElementNormal(NormalIndex);
				FbxVector4 Normal;
				if (NormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					int Index;
					switch (NormalElement->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Normal = NormalElement->GetDirectArray().GetAt(VertexIndex);
						break;
					case FbxGeometryElement::eIndexToDirect:
						Index = NormalElement->GetIndexArray().GetAt(VertexIndex);
						Normal = NormalElement->GetDirectArray().GetAt(Index);
						break;
					default:
						ETERNAL_BREAK();
					}
				}
				else if (NormalElement->GetMappingMode() == FbxLayerElement::eByControlPoint)
				{
					switch (NormalElement->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Normal = NormalElement->GetDirectArray().GetAt(InControlPointIndex);
						break;
					default:
						ETERNAL_BREAK();
					}
				}
				InOutMesh.GetVertex(InControlPointIndex).Normal = Vector3(
					static_cast<float>(Normal[0]),
					static_cast<float>(Normal[1]),
					static_cast<float>(Normal[2])
				);
				
				ETERNAL_ASSERT(Normal[3] == 1.0);
			}
		}

		void ImportFbx::_ImportNode_GetTangent(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh)
		{
			for (int TangentIndex = 0, TangentCount = InMesh->GetElementTangentCount(); TangentIndex < TangentCount; ++TangentIndex)
			{
				FbxGeometryElementTangent* TangentElement = InMesh->GetElementTangent(TangentIndex);
				ETERNAL_ASSERT(TangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex);
				if (TangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					FbxVector4 Tangent;
					int Index;
					switch (TangentElement->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Tangent = TangentElement->GetDirectArray().GetAt(InControlPointIndex);
						break;
					case FbxGeometryElement::eIndexToDirect:
						Index = TangentElement->GetIndexArray().GetAt(InControlPointIndex);
						Tangent = TangentElement->GetDirectArray().GetAt(Index);
						break;
					default:
						ETERNAL_BREAK();
					}
					InOutMesh.GetVertex(InControlPointIndex).Tangent = Vector3(
						static_cast<float>(Tangent[0]),
						static_cast<float>(Tangent[1]),
						static_cast<float>(Tangent[2])
					);
					ETERNAL_ASSERT(Tangent[3] == 1.0);
				}
			}
		}

		void ImportFbx::_ImportNode_GetBinormal(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh)
		{
			for (int BinormalIndex = 0, BinormalCount = InMesh->GetElementBinormalCount(); BinormalIndex < BinormalCount; ++BinormalIndex)
			{
				FbxGeometryElementBinormal* BinormalElement = InMesh->GetElementBinormal(BinormalIndex);
				ETERNAL_ASSERT(BinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex);
				if (BinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					FbxVector4 Binormal;
					int Index;
					switch (BinormalElement->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Binormal = BinormalElement->GetDirectArray().GetAt(InControlPointIndex);
						break;
					case FbxGeometryElement::eIndexToDirect:
						Index = BinormalElement->GetIndexArray().GetAt(InControlPointIndex);
						Binormal = BinormalElement->GetDirectArray().GetAt(Index);
						break;
					default:
						ETERNAL_BREAK();
					}
					InOutMesh.GetVertex(InControlPointIndex).Binormal = Vector3(
						static_cast<float>(Binormal[0]),
						static_cast<float>(Binormal[1]),
						static_cast<float>(Binormal[2])
					);
					ETERNAL_ASSERT(Binormal[3] == 1.0);
				}
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
