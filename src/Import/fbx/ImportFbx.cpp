#include "Import/fbx/ImportFbx.hpp"

//#define VC_EXTRALEAN
//#define WIN32_LEAN_AND_MEAN
//#define WIN32_EXTRA_LEAN
//#include <windows.h>
//
//#include <cstdlib>
//
//#include "UtilsSettings.hpp"
//#include "File/FileFactory.hpp"

//#include "Mesh/Bone.hpp"
//#include "Mesh/BoundingBox.hpp"
#include <cstdio>
#include "GraphicData/MeshVertexFormat.hpp"
#include "File/FilePath.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::FileSystem;
		using namespace Eternal::GraphicData;

		ImportFbx* ImportFbx::_Instance = nullptr;

		ImportFbx::ImportFbx()
		{
			ETERNAL_ASSERT(!_Instance);
			_Instance = this;

			_SdkManager = FbxManager::Create();
			_Settings = FbxIOSettings::Create(_SdkManager, IOSROOT);

			_SdkManager->SetIOSettings(_Settings);
			_FbxImporter = FbxImporter::Create(_SdkManager, "");

			LogWrite(LogWarning, LogImport, "[ImportFbx::ImportFbx]UV.y has been inversed!");
			LogWrite(LogWarning, LogImport, "[ImportFbx::ImportFbx]Pos.w = 1.f!");
		}

		ImportFbx* ImportFbx::Get()
		{
			ETERNAL_ASSERT(_Instance);
			return _Instance;
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

			OutMeshPayload.LoadedMesh = new GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>();

			FbxScene* Scene = FbxScene::Create(_SdkManager, "scene");
			_FbxImporter->Import(Scene);

			BoundingBox Box;
			Box.SetMin(Vector3(std::numeric_limits<float>::infinity()));
			Box.SetMax(Vector3(-std::numeric_limits<float>::infinity()));

			_ImportNode(Scene->GetRootNode(), *OutMeshPayload.LoadedMesh, Box, OutMeshPayload);

			GenericMesh<PositionColorVertex, uint16_t>* BoundingBoxMesh = new GenericMesh<PositionColorVertex, uint16_t>();
			PositionColorVertex BoundingBoxVertex[] = {
				{ Vector4(Box.GetMin().x, Box.GetMin().y, Box.GetMin().z, 1.0f), 0x000000FF },
				{ Vector4(Box.GetMin().x, Box.GetMin().y, Box.GetMax().z, 1.0f), 0x0000FFFF },
				{ Vector4(Box.GetMin().x, Box.GetMax().y, Box.GetMin().z, 1.0f), 0x00FF00FF },
				{ Vector4(Box.GetMin().x, Box.GetMax().y, Box.GetMax().z, 1.0f), 0x00FFFFFF },
				{ Vector4(Box.GetMax().x, Box.GetMin().y, Box.GetMin().z, 1.0f), 0xFF0000FF },
				{ Vector4(Box.GetMax().x, Box.GetMin().y, Box.GetMax().z, 1.0f), 0xFF00FFFF },
				{ Vector4(Box.GetMax().x, Box.GetMax().y, Box.GetMin().z, 1.0f), 0xFFFF00FF },
				{ Vector4(Box.GetMax().x, Box.GetMax().y, Box.GetMax().z, 1.0f), 0xFFFFFFFF },
			};
			for (int BoundingBoxVertexIndex = 0; BoundingBoxVertexIndex < ETERNAL_ARRAYSIZE(BoundingBoxVertex); ++BoundingBoxVertexIndex)
				BoundingBoxMesh->PushVertex(BoundingBoxVertex[BoundingBoxVertexIndex]);

			// -X
			BoundingBoxMesh->PushTriangle(2, 0, 3);
			BoundingBoxMesh->PushTriangle(0, 1, 3);

			// +X
			BoundingBoxMesh->PushTriangle(4, 6, 5);
			BoundingBoxMesh->PushTriangle(6, 7, 5);
	
			// -Y
			BoundingBoxMesh->PushTriangle(0, 4, 1);
			BoundingBoxMesh->PushTriangle(4, 5, 1);

			// +Y
			BoundingBoxMesh->PushTriangle(6, 2, 7);
			BoundingBoxMesh->PushTriangle(2, 3, 7);

			// -Z
			BoundingBoxMesh->PushTriangle(2, 6, 0);
			BoundingBoxMesh->PushTriangle(6, 4, 0);

			// +Z
			BoundingBoxMesh->PushTriangle(1, 5, 3);
			BoundingBoxMesh->PushTriangle(5, 7, 3);

			OutMeshPayload.BoundingBox = BoundingBoxMesh;

			Scene->Destroy(true);

			return OutMeshPayload;
		}

		void ImportFbx::_ImportPoses(_In_ FbxScene* Scene)
		{
			int PoseCount = Scene->GetPoseCount();
			for (int PoseIndex = 0; PoseIndex < PoseCount; ++PoseIndex)
			{
				FbxPose* Pose = Scene->GetPose(PoseIndex);
			}
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

		void ImportFbx::_ImportNode(_In_ const FbxNode* InNode, _Out_ Mesh& OutMesh, _Inout_ BoundingBox& InOutBoundingBox, _Inout_ MeshPayload& InOutMeshPayload)
		{
			GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>& OutGenericMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>&>(OutMesh);

			OutGenericMesh.SetName(InNode->GetName());

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

						OutGenericMesh.PushVertex(Vertex);
					}

					int VertexId = 0;

					for (int PolygonIndex = 0, PolygonCount = InFbxMesh->GetPolygonCount(); PolygonIndex < PolygonCount; ++PolygonIndex)
					{
						int PolygonSize = InFbxMesh->GetPolygonSize(PolygonIndex);
						OutGenericMesh.PushTriangle(
							static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 0)),
							static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 1)),
							static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 2))
						);
						if (PolygonSize == 4) // Quad
						{
							OutGenericMesh.PushTriangle(
								static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 0)),
								static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 2)),
								static_cast<uint16_t>(InFbxMesh->GetPolygonVertex(PolygonIndex, 3))
							);
						}
						ETERNAL_ASSERT(PolygonSize <= 4);

						for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
						{
							int ControlPointIndex = InFbxMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);

							_GetNormal(InFbxMesh, ControlPointIndex, VertexId, OutGenericMesh);
							_GetTangent(InFbxMesh, ControlPointIndex, VertexId, OutGenericMesh);
							_GetBinormal(InFbxMesh, ControlPointIndex, VertexId, OutGenericMesh);

							++VertexId;
						}
						_GetUV(InFbxMesh, PolygonIndex, OutGenericMesh);
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

											auto RequestTexture = [&FileName, &Extension, &OutMesh, &InOutMeshPayload](_In_ const char* MapName, _In_ const TextureType& InTextureType)
											{
												char TextureKey[1024];
												char TexturePath[1024];

												sprintf_s(TextureKey, "%s_%s", FileName, MapName);
												sprintf_s(TexturePath, "%s_%s%s", FileName, MapName, Extension);

												TextureFactoryRequest TextureRequest(TextureKey, TexturePath);
												if (FilePath::Find(TexturePath, FileType::FILE_TYPE_TEXTURES).size() > 0)
												{
													InOutMeshPayload.TextureRequests.push_back(TextureRequest);
													OutMesh.SetTexture(InTextureType, TextureKey);
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
			OutGenericMesh.GetTransform().Translate(
				Vector3(
					static_cast<float>(InNode->LclTranslation.Get()[0]),
					static_cast<float>(InNode->LclTranslation.Get()[1]),
					static_cast<float>(InNode->LclTranslation.Get()[2])
				)
			);

			OutGenericMesh.GetTransform().Rotate(
				Vector3(
					static_cast<float>(InNode->LclRotation.Get()[0]),
					static_cast<float>(InNode->LclRotation.Get()[1]),
					static_cast<float>(InNode->LclRotation.Get()[2])
				)
			);

			OutGenericMesh.GetTransform().Scale(
				Vector3(
					static_cast<float>(InNode->LclScaling.Get()[0]),
					static_cast<float>(InNode->LclScaling.Get()[1]),
					static_cast<float>(InNode->LclScaling.Get()[2])
				)
			);

			for (int NodeChildIndex = 0; NodeChildIndex < InNode->GetChildCount(); ++NodeChildIndex)
			{
				Mesh* SubMesh = new GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>();
		
				_ImportNode(InNode->GetChild(NodeChildIndex), *SubMesh, InOutBoundingBox, InOutMeshPayload);
		
				OutGenericMesh.PushMesh(SubMesh);
			}
		}

		void ImportFbx::_GetUV(_In_ FbxMesh * InMesh, _In_ uint32_t InPolygonIndex, _Inout_ Mesh& OutMesh)
		{
			GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>& OutGenericMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>&>(OutMesh);
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

				OutGenericMesh.GetVertex(Vertex).UV = Vector2(
					static_cast<float>(UV[0]),
					static_cast<float>(1.0f - UV[1])
				);
			}
		}

		void ImportFbx::_GetNormal(_In_ FbxMesh * InMesh, _In_ int InControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& OutMesh)
		{
			GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>& OutGenericMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>&>(OutMesh);
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
				OutGenericMesh.GetVertex(InControlPointIndex).Normal += Vector3(
					static_cast<float>(Normal[0]),
					static_cast<float>(Normal[1]),
					static_cast<float>(Normal[2])
				);
				
				ETERNAL_ASSERT(Normal[3] == 1.0);
			}
		}

		void ImportFbx::_GetTangent(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ Mesh& OutMesh)
		{
			GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>& OutGenericMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>&>(OutMesh);
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
					OutGenericMesh.GetVertex(InControlPointIndex).Tangent = Vector3(
						static_cast<float>(Tangent[0]),
						static_cast<float>(Tangent[1]),
						static_cast<float>(Tangent[2])
					);
					ETERNAL_ASSERT(Tangent[3] == 1.0);
				}
			}
		}

		void ImportFbx::_GetBinormal(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ Mesh& OutMesh)
		{
			GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>& OutGenericMesh = static_cast<GenericMesh<PositionNormalTangentBinormalUVVertex, uint16_t>&>(OutMesh);
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
					OutGenericMesh.GetVertex(InControlPointIndex).Binormal = Vector3(
						static_cast<float>(Binormal[0]),
						static_cast<float>(Binormal[1]),
						static_cast<float>(Binormal[2])
					);
					ETERNAL_ASSERT(Binormal[3] == 1.0);
				}
			}
		}

		void ImportFbx::_ImportSkeletal(_In_ FbxMesh* MeshObj)
		{
			bool HasVertexCache = MeshObj->GetDeformerCount(FbxDeformer::eVertexCache) && ((FbxVertexCacheDeformer*)MeshObj->GetDeformer(0, FbxDeformer::eVertexCache))->Active.Get();
			bool HasShape = MeshObj->GetShapeCount() > 0;
			bool HasDeformation = MeshObj->GetDeformerCount(FbxDeformer::eSkin) > 0;


			if (HasVertexCache)
			{
				// Vertex cache?
			}
			else
			{
				if (HasShape)
				{
					_GetBlendShape(MeshObj);
				}
				_GetSkinning(MeshObj);
			}
		}

		void ImportFbx::_GetBlendShape(_In_ FbxMesh* MeshObj)
		{
		}

		void ImportFbx::_GetSkinning(_In_ FbxMesh* MeshObj)
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
