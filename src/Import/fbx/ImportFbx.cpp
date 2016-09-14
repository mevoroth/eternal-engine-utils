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

using namespace Eternal::Import;
using namespace Eternal::Graphics;
using namespace Eternal::File;

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

	OutputDebugString("WARNING: UV.y has been inversed!\n");
	OutputDebugString("WARNING: Pos.w = 1.f\n");
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

void ImportFbx::Import(_In_ const std::string& Path, _Out_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out)
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
		OutputDebugString(Status.GetErrorString());
		// LOG
		//assert(false);
	}

	FbxScene* Scene = FbxScene::Create(_SdkMgr, "scene");
	_FbxImporter->Import(Scene);

	_ImportNode(Scene->GetRootNode(), Out);

	//_ImportPoses();
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

void ImportFbx::_ImportNode(_In_ const FbxNode* Node, _Out_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out)
{
	const FbxNodeAttribute* Attribute = Node->GetNodeAttribute();
	if (Attribute)
	{
		switch (Attribute->GetAttributeType())
		{
		case FbxNodeAttribute::EType::eMesh: {
			FbxMesh* FbxMeshObj = (FbxMesh*)Attribute;
			FbxVector4* V = FbxMeshObj->GetControlPoints();
			D3D11PosUVNormalVertexBuffer::PosUVNormalVertex VertexObj;
			for (int ControlPointIndex = 0, ControlPointCount = FbxMeshObj->GetControlPointsCount(); ControlPointIndex < ControlPointCount; ++ControlPointIndex)
			{
				//VertexObj.Pos = Vector4(V[ControlPointIndex][0], V[ControlPointIndex][1], V[ControlPointIndex][2], V[ControlPointIndex][3]);
				VertexObj.Pos = Vector4(V[ControlPointIndex][0], V[ControlPointIndex][1], V[ControlPointIndex][2], 1.f);

				Out.PushVertex(VertexObj);
			}
			for (int PolygonIndex = 0, ControlPointCount = FbxMeshObj->GetPolygonCount(); PolygonIndex < ControlPointCount; ++PolygonIndex)
			{
				int PolygonSize = FbxMeshObj->GetPolygonSize(PolygonIndex);
				Out.PushTriangle(
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 0),
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 1),
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 2)
					);
				if (PolygonSize == 4) // Quad
				{
					Out.PushTriangle(
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 0),
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 2),
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 3)
						);
				}

				_GetUV(FbxMeshObj, PolygonIndex, Out);
				_GetNormal(FbxMeshObj, PolygonIndex, Out);
			}

			_ImportSkeletal(FbxMeshObj);

			{
				ETERNAL_ASSERT(Node->GetSrcObjectCount<FbxSurfaceMaterial>() > 0);
				FbxSurfaceMaterial* Material = Node->GetSrcObject<FbxSurfaceMaterial>(0);
				FbxProperty DiffuseColorChannel = Material->FindProperty(FbxLayerElement::sTextureChannelNames[DIFFUSE_COLOR_CHANNEL]);
				ETERNAL_ASSERT(DiffuseColorChannel.IsValid());

				FbxTexture* DiffuseColorTexture = DiffuseColorChannel.GetSrcObject<FbxTexture>(0);
				if (!DiffuseColorTexture)
					break;
				//FbxTexture* DiffuseColorTexture = DiffuseColorChannel.GetSrcObject<FbxLayeredTexture>(0)->GetSrcObject<FbxTexture>(0);
				//OutputDebugString("\n");
				//OutputDebugString(DiffuseColorChannel.GetName());
				//OutputDebugString(DiffuseColorTexture->GetName());
				//OutputDebugString(fbxsdk_2015_1::FbxCast<FbxFileTexture>(DiffuseColorTexture)->GetFileName());
				char FileName[255];
				char Extension[8];
				_splitpath_s(FbxCast<FbxFileTexture>(DiffuseColorTexture)->GetFileName(), nullptr, 0, nullptr, 0, FileName, 255, Extension, 8);
				sprintf_s(FileName, "%s%s", FileName, Extension);
				Out.SetTexture(FileName);
			}
		} break;

		//case FbxNodeAttribute::EType::eSkeleton: {

		//} break;
		}
	}
	Out.GetTransform().Translate(
		Vector3(
			Node->LclTranslation.Get()[0],
			Node->LclTranslation.Get()[1],
			Node->LclTranslation.Get()[2]
		)
	);

	Out.GetTransform().Rotate(
		Vector3(
			Node->LclRotation.Get()[0],
			Node->LclRotation.Get()[1],
			Node->LclRotation.Get()[2]
		)
	);

	Out.GetTransform().Scale(
		Vector3(
			Node->LclScaling.Get()[0],
			Node->LclScaling.Get()[1],
			Node->LclScaling.Get()[2]
		)
	);

	for (int NodeChildIndex = 0; NodeChildIndex < Node->GetChildCount(); ++NodeChildIndex)
	{
		GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer> SubMehObj;
		_ImportNode(Node->GetChild(NodeChildIndex), SubMehObj);
		Out.PushMesh(SubMehObj);
	}
}

void ImportFbx::_GetUV(_In_ FbxMesh * MeshObj, _In_ uint32_t PolygonIndex, _Inout_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out)
{
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

		Out.GetVertex(Vertex).UV = Vector2(UV[0], 1.f - UV[1]);
	}
}

void ImportFbx::_GetNormal(_In_ FbxMesh * MeshObj, _In_ uint32_t PolygonIndex, _Inout_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out)
{
	for (uint32_t VertexIndex = 0, VerticesCount = MeshObj->GetPolygonSize(PolygonIndex); VertexIndex < VerticesCount; ++VertexIndex)
	{
		uint32_t Vertex = MeshObj->GetPolygonVertex(PolygonIndex, VertexIndex);

		FbxVector4 Normal;
		bool Ret = MeshObj->GetPolygonVertexNormal(PolygonIndex, VertexIndex, Normal);
		ETERNAL_ASSERT(Ret);

		Out.GetVertex(Vertex).Normal = Vector4(Normal[0], Normal[1], Normal[2], Normal[3]);
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
