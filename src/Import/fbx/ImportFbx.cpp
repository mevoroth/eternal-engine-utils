#include "Import/fbx/ImportFbx.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include <cstdlib>

#include "UtilsSettings.hpp"
#include "Types/Types.hpp"
#include "Macros/Macros.hpp"

using namespace Eternal::Import;
using namespace Eternal::Graphics;

ImportFbx* ImportFbx::_Inst = nullptr;

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

void ImportFbx::Import(_In_ const std::string& Path, _Out_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out)
{
	if (_FbxImporter->Initialize(Path.c_str(), -1, _Settings))
	{
		FbxStatus& Status = _FbxImporter->GetStatus();
		OutputDebugString(Status.GetErrorString());
		// LOG
		//assert(false);
	}

	FbxScene* scene = FbxScene::Create(_SdkMgr, "scene");
	_FbxImporter->Import(scene);

	_ImportNode(scene->GetRootNode(), Out);
}

void ImportFbx::_ImportNode(_In_ const FbxNode* Node, _Out_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out)
{
	const FbxNodeAttribute* Attribute = Node->GetNodeAttribute();
	if (Attribute)
	{
		switch (Attribute->GetAttributeType())
		{
		case FbxNodeAttribute::EType::eMesh:
			fbxsdk_2015_1::FbxMesh* FbxMeshObj = (fbxsdk_2015_1::FbxMesh*)Attribute;
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
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 2),
					FbxMeshObj->GetPolygonVertex(PolygonIndex, 1)
				);
				if (PolygonSize == 4) // Quad
				{
					Out.PushTriangle(
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 0),
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 3),
						FbxMeshObj->GetPolygonVertex(PolygonIndex, 2)
					);
				}

				_GetUV(FbxMeshObj, PolygonIndex, Out);
				_GetNormal(FbxMeshObj, PolygonIndex, Out);
			}
			{
				ETERNAL_ASSERT(Node->GetSrcObjectCount<fbxsdk_2015_1::FbxSurfaceMaterial>() > 0);
				fbxsdk_2015_1::FbxSurfaceMaterial* Material = Node->GetSrcObject<fbxsdk_2015_1::FbxSurfaceMaterial>(0);
				FbxProperty DiffuseColorChannel = Material->FindProperty(fbxsdk_2015_1::FbxLayerElement::sTextureChannelNames[DIFFUSE_COLOR_CHANNEL]);
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
				_splitpath_s(fbxsdk_2015_1::FbxCast<FbxFileTexture>(DiffuseColorTexture)->GetFileName(), nullptr, 0, nullptr, 0, FileName, 255, Extension, 8);
				sprintf_s(FileName, "%s%s", FileName, Extension);
				Out.SetTexture(FileName);
			}
			break;
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
