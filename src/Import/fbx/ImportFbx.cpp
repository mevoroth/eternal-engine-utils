#include "Import/fbx/ImportFbx.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include <cstdlib>

#include "UtilsSettings.hpp"
#include "Types/Types.hpp"
#include "Macros/Macros.hpp"
#include "File/FileFactory.hpp"

#include "Mesh/Bone.hpp"
#include "Mesh/BoundingBox.hpp"
#include "Log/Log.hpp"
#include "Resources/TextureFactory.hpp"
#include "GraphicData/VertexFormat.hpp"

using namespace Eternal::Import;
using namespace Eternal::Graphics;
using namespace Eternal::File;
using namespace Eternal::GraphicData;

ImportFbx* ImportFbx::_Inst = nullptr;
vector<string> ImportFbx::_IncludePaths;

ImportFbx::ImportFbx()
{
	ETERNAL_ASSERT(!_Inst);
	_Inst = this;

	_SdkMgr = FbxManager::Create();
	_Settings = FbxIOSettings::Create(_SdkMgr, IOSROOT);

	_SdkMgr->SetIOSettings(_Settings);
	_FbxImporter = FbxImporter::Create(_SdkMgr, "");

	Eternal::Log::Log::Get()->Write(Eternal::Log::Log::Warning, Eternal::Log::Log::Import, "[ImportFbx::ImportFbx]UV.y has been inversed!");
	Eternal::Log::Log::Get()->Write(Eternal::Log::Log::Warning, Eternal::Log::Log::Import, "[ImportFbx::ImportFbx]Pos.w = 1.f!");
}

ImportFbx* ImportFbx::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

void ImportFbx::RegisterPath(const std::string& IncludePath)
{
	_IncludePaths.push_back(IncludePath);
}

void ImportFbx::Import(_In_ const std::string& Path, _Out_ Mesh*& OutMesh, _Out_ Mesh*& OutBoundingBox)
{
	std::string FilePath;
	bool PathFound = false;
	for (int IncludePathIndex = 0; !PathFound && IncludePathIndex < _IncludePaths.size(); ++IncludePathIndex)
	{
		FilePath = _IncludePaths[IncludePathIndex] + Path;
		PathFound = FileExists(FilePath);
	}

	ETERNAL_ASSERT(PathFound);

	if (_FbxImporter->Initialize(FilePath.c_str(), -1, _Settings))
	{
		FbxStatus& Status = _FbxImporter->GetStatus();
		std::string ErrorMessage = "[ImportFbx::Import]";
		ErrorMessage.append(Status.GetErrorString());
		Eternal::Log::Log::Get()->Write(Eternal::Log::Log::Warning, Eternal::Log::Log::Import, ErrorMessage.c_str());

		// LOG
		//ETERNAL_ASSERT(false);
	}

	OutMesh = new GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>();

	FbxScene* Scene = FbxScene::Create(_SdkMgr, "scene");
	_FbxImporter->Import(Scene);

	BoundingBox Box;
	Box.SetMin(Vector3(
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity()
	));
	Box.SetMax(Vector3(
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity()
	));

	_ImportNode(Scene->GetRootNode(), *OutMesh, Box);

	GenericMesh<PosColorVertex, uint32_t>* BoundingBoxMesh = new GenericMesh<PosColorVertex, uint32_t>();
	PosColorVertex BoundingBoxVertex[] = {
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

	OutBoundingBox = BoundingBoxMesh;
}

void ImportFbx::_ImportPoses(_In_ FbxScene* Scene)
{
	int PoseCount = Scene->GetPoseCount();
	for (int PoseIndex = 0; PoseIndex < PoseCount; ++PoseIndex)
	{
		FbxPose* Pose = Scene->GetPose(PoseIndex);
		//Pose->Get

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

void ImportFbx::_ImportNode(_In_ const FbxNode* Node, _Out_ Mesh& OutMeshObj, _Out_ BoundingBox& OutBoundingBox)
{
	GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>& OutMesh = (GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>&)OutMeshObj;

	const FbxNodeAttribute* Attribute = Node->GetNodeAttribute();
	if (Attribute)
	{
		FbxNodeAttribute::EType AttributeType = Attribute->GetAttributeType();
		switch (AttributeType)
		{
		case FbxNodeAttribute::EType::eMesh: {
			FbxMesh* FbxMeshObj = (FbxMesh*)Attribute;
			FbxVector4* V = FbxMeshObj->GetControlPoints();
			PosUVNormalTangentBinormalVertex VertexObj;
			for (int ControlPointIndex = 0, ControlPointsCount = FbxMeshObj->GetControlPointsCount(); ControlPointIndex < ControlPointsCount; ++ControlPointIndex)
			{
				//VertexObj.Pos = Vector4(V[ControlPointIndex][0], V[ControlPointIndex][1], V[ControlPointIndex][2], V[ControlPointIndex][3]);
				VertexObj.Pos = Vector4(V[ControlPointIndex][0], V[ControlPointIndex][1], V[ControlPointIndex][2], 1.f);
				VertexObj.Normal = Vector4(0.f, 0.f, 0.f, 0.f);

				Vector3 Vertex3(V[ControlPointIndex][0], V[ControlPointIndex][1], V[ControlPointIndex][2]);

				Vector3 NewMin = OutBoundingBox.GetMin();
				Vector3 NewMax = OutBoundingBox.GetMax();

				NewMin.x = min(NewMin.x, Vertex3.x);
				NewMin.y = min(NewMin.y, Vertex3.y);
				NewMin.z = min(NewMin.z, Vertex3.z);

				NewMax.x = max(NewMax.x, Vertex3.x);
				NewMax.y = max(NewMax.y, Vertex3.y);
				NewMax.z = max(NewMax.z, Vertex3.z);

				OutBoundingBox.SetMin(NewMin);
				OutBoundingBox.SetMax(NewMax);

				OutMesh.PushVertex(VertexObj);
			}

			int VertexId = 0;

			for (int PolygonIndex = 0, PolygonCount = FbxMeshObj->GetPolygonCount(); PolygonIndex < PolygonCount; ++PolygonIndex)
			{
				int PolygonSize = FbxMeshObj->GetPolygonSize(PolygonIndex);
				OutMesh.PushTriangle(
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 0),
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 1),
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 2)
				);
				if (PolygonSize == 4) // Quad
				{
					OutMesh.PushTriangle(
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 0),
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 2),
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 3)
					);
				}
				ETERNAL_ASSERT(PolygonSize <= 4);

				for (int PolygonPos = 0; PolygonPos < PolygonSize; ++PolygonPos)
				{
					int ControlPointIndex = FbxMeshObj->GetPolygonVertex(PolygonIndex, PolygonPos);

					_GetNormal(FbxMeshObj, ControlPointIndex, VertexId, OutMesh);
					_GetTangent(FbxMeshObj, ControlPointIndex, VertexId, OutMesh);
					_GetBinormal(FbxMeshObj, ControlPointIndex, VertexId, OutMesh);

					++VertexId;
				}
				_GetUV(FbxMeshObj, PolygonIndex, OutMesh);
				//_GetNormal(FbxMeshObj, PolygonIndex, Out);
			}

			_ImportSkeletal(FbxMeshObj);

			{
				Texture* Diffuse = Eternal::Resources::TextureFactory::Get()->GetTexture("_RED");
				Texture* Specular = Eternal::Resources::TextureFactory::Get()->GetTexture("_RED");
				Texture* Normal = Eternal::Resources::TextureFactory::Get()->GetTexture("_DEFAULT_NORMAL");
				if (Node->GetSrcObjectCount<FbxSurfaceMaterial>() > 0)
				{
					FbxSurfaceMaterial* Material = Node->GetSrcObject<FbxSurfaceMaterial>(0);
					if (Material)
					{
						FbxProperty ColorChannel = Material->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
						if (ColorChannel.IsValid())
						{
							FbxTexture* TextureObject = ColorChannel.GetSrcObject<FbxTexture>();
							if (TextureObject)
							{
								char TextureKey[1024];
								char FileName[255];
								char Extension[8];
								_splitpath_s(FbxCast<FbxFileTexture>(TextureObject)->GetFileName(), nullptr, 0, nullptr, 0, FileName, 255, Extension, 8);
								char* PrefixEnd = strstr(FileName, "_diff");

								if (PrefixEnd)
								{
									*PrefixEnd = 0;

									sprintf_s(TextureKey, "%s_diff%s", FileName, Extension);
									Diffuse = Eternal::Resources::TextureFactory::Get()->GetTexture(TextureKey);

									sprintf_s(TextureKey, "%s_spec%s", FileName, Extension);
									Texture* CustomSpecularMap = Eternal::Resources::TextureFactory::Get()->GetTexture(TextureKey);
									Specular = CustomSpecularMap ? CustomSpecularMap : Specular;

									sprintf_s(TextureKey, "%s_ddn%s", FileName, Extension);
									Texture* CustomNormalMap = Eternal::Resources::TextureFactory::Get()->GetTexture(TextureKey);
									Normal = CustomNormalMap ? CustomNormalMap : Normal;
								}
							}
						}
					}

					//if (Material)
					//{
					//	_ImportTextureFromFBX(Material, DIFFUSE_COLOR_CHANNEL, "", Diffuse);
					//	_ImportTextureFromFBX(Material, SPECULAR_COLOR_CHANNEL, "_spec", Specular);
					//	_ImportTextureFromFBX(Material, DIFFUSE_COLOR_CHANNEL, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)3, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)4, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)5, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)6, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)7, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)8, "_ddn", Normal);
					//	_ImportTextureFromFBX(Material, (Channel)9, "_ddn", Normal);
					//}
				}

				//FbxTexture* DiffuseColorTexture = DiffuseColorChannel.GetSrcObject<FbxLayeredTexture>(0)->GetSrcObject<FbxTexture>(0);
				//OutputDebugString("\n");
				//OutputDebugString(DiffuseColorChannel.GetName());
				//OutputDebugString(DiffuseColorTexture->GetName());
				//OutputDebugString(fbxsdk_2015_1::FbxCast<FbxFileTexture>(DiffuseColorTexture)->GetFileName());

				ETERNAL_ASSERT(false);
				//OutMesh.SetTexture(Diffuse, Specular, Normal);
			}
		} break;

		//case FbxNodeAttribute::EType::eSkeleton: {

		//} break;
		}
	}
	OutMesh.GetTransform().Translate(
		Vector3(
			Node->LclTranslation.Get()[0],
			Node->LclTranslation.Get()[1],
			Node->LclTranslation.Get()[2]
		)
	);

	OutMesh.GetTransform().Rotate(
		Vector3(
			Node->LclRotation.Get()[0],
			Node->LclRotation.Get()[1],
			Node->LclRotation.Get()[2]
		)
	);

	OutMesh.GetTransform().Scale(
		Vector3(
			Node->LclScaling.Get()[0],
			Node->LclScaling.Get()[1],
			Node->LclScaling.Get()[2]
		)
	);

	for (int NodeChildIndex = 0; NodeChildIndex < Node->GetChildCount(); ++NodeChildIndex)
	{
		Mesh* SubMeshObj = new GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>();
		ETERNAL_ASSERT(false);
		//SubMehObj->SetBoundingBox(Out.GetBoundingBox());
		_ImportNode(Node->GetChild(NodeChildIndex), *SubMeshObj, OutBoundingBox);
		
		ETERNAL_ASSERT(false);
		// REMOVE THIS
		//if (SubMehObj->IsValidNode())
		//	SubMehObj->InitializeBuffers();
		
		OutMesh.PushMesh(SubMeshObj);
	}
}

void ImportFbx::_GetUV(_In_ FbxMesh * MeshObj, _In_ uint32_t PolygonIndex, _Inout_ Mesh& Out)
{
	GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>& OutMesh = (GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>&)Out;
	for (uint32_t VertexIndex = 0, VerticesCount = MeshObj->GetPolygonSize(PolygonIndex); VertexIndex < VerticesCount; ++VertexIndex)
	{
		uint32_t Vertex = MeshObj->GetPolygonVertex(PolygonIndex, VertexIndex);

		FbxVector2 UV(0.0, 0.0);

		if (MeshObj->GetLayer(0) && MeshObj->GetLayer(0)->GetUVSets().GetCount() > 0)
		{
			bool UnMapped;
			bool Ret = MeshObj->GetPolygonVertexUV(PolygonIndex, VertexIndex, MeshObj->GetLayer(0)->GetUVSets()[0]->GetName(), UV, UnMapped);
			ETERNAL_ASSERT(Ret);
			ETERNAL_ASSERT(!UnMapped);
		}
		//ETERNAL_ASSERT(false);

		OutMesh.GetVertex(Vertex).UV = Vector2(UV[0], 1.f - UV[1]);
	}
}

void ImportFbx::_GetNormal(_In_ FbxMesh * MeshObj, _In_ int ControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& Out)
{
	GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>& OutMesh = (GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>&)Out;
	for (int NormalIndex = 0, NormalCount = MeshObj->GetElementNormalCount(); NormalIndex < NormalCount; ++NormalIndex)
	{
		FbxGeometryElementNormal* NormalElement = MeshObj->GetElementNormal(NormalIndex);
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
				ETERNAL_ASSERT(false);
			}
		}
		else if (NormalElement->GetMappingMode() == FbxLayerElement::eByControlPoint)
		{
			switch (NormalElement->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				Normal = NormalElement->GetDirectArray().GetAt(ControlPointIndex);
				break;
			default:
				ETERNAL_ASSERT(false);
			}
		}
		OutMesh.GetVertex(ControlPointIndex).Normal += Vector4(Normal[0], Normal[1], Normal[2], Normal[3]);
	}
}

void ImportFbx::_GetTangent(_In_ FbxMesh* MeshObj, _In_ int ControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& Out)
{
	GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>& OutMesh = (GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>&)Out;
	for (int TangentIndex = 0, TangentCount = MeshObj->GetElementTangentCount(); TangentIndex < TangentCount; ++TangentIndex)
	{
		FbxGeometryElementTangent* TangentElement = MeshObj->GetElementTangent(TangentIndex);
		ETERNAL_ASSERT(TangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex);
		if (TangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			FbxVector4 Tangent;
			int Index;
			switch (TangentElement->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				Tangent = TangentElement->GetDirectArray().GetAt(ControlPointIndex);
				break;
			case FbxGeometryElement::eIndexToDirect:
				Index = TangentElement->GetIndexArray().GetAt(ControlPointIndex);
				Tangent = TangentElement->GetDirectArray().GetAt(Index);
				break;
			default:
				ETERNAL_ASSERT(false);
			}
			OutMesh.GetVertex(ControlPointIndex).Tangent = Vector4(Tangent[0], Tangent[1], Tangent[2], Tangent[3]);
		}
	}
}

void ImportFbx::_GetBinormal(_In_ FbxMesh* MeshObj, _In_ int ControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& Out)
{
	GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>& OutMesh = (GenericMesh<PosUVNormalTangentBinormalVertex, uint32_t>&)Out;
	for (int BinormalIndex = 0, BinormalCount = MeshObj->GetElementBinormalCount(); BinormalIndex < BinormalCount; ++BinormalIndex)
	{
		FbxGeometryElementBinormal* BinormalElement = MeshObj->GetElementBinormal(BinormalIndex);
		ETERNAL_ASSERT(BinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex);
		if (BinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			FbxVector4 Binormal;
			int Index;
			switch (BinormalElement->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				Binormal = BinormalElement->GetDirectArray().GetAt(ControlPointIndex);
				break;
			case FbxGeometryElement::eIndexToDirect:
				Index = BinormalElement->GetIndexArray().GetAt(ControlPointIndex);
				Binormal = BinormalElement->GetDirectArray().GetAt(Index);
				break;
			default:
				ETERNAL_ASSERT(false);
			}
			OutMesh.GetVertex(ControlPointIndex).Binormal = Vector4(Binormal[0], Binormal[1], Binormal[2], Binormal[3]);
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
	int DeformerCount = MeshObj->GetDeformerCount(FbxDeformer::eSkin);
	for (int DeformerIndex = 0; DeformerIndex < DeformerCount; ++DeformerIndex)
	{
		FbxStatus Status;
		FbxSkin* Skin = (FbxSkin*)MeshObj->GetDeformer(DeformerIndex, FbxDeformer::eSkin, &Status);
		Bone* BoneObj = new Bone();

		int ClusterCount = Skin->GetClusterCount();
		for (int ClusterIndex = 0; ClusterIndex < ClusterCount; ++ClusterIndex)
		{

			FbxCluster* Cluster = Skin->GetCluster(ClusterIndex);
			int ControlPointIndicesCount = Cluster->GetControlPointIndicesCount();

			for (int ControlPointIndicesIndex = 0; ControlPointIndicesIndex < ControlPointIndicesCount; ++ControlPointIndicesIndex)
			{
				BoneObj->PushInfluence(
					Cluster->GetControlPointIndices()[ControlPointIndicesIndex],
					Cluster->GetControlPointWeights()[ControlPointIndicesIndex]
				);
			}
		}
	}
}
