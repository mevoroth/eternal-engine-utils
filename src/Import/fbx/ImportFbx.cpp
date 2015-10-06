#include "Import/fbx/ImportFbx.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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
			FbxMesh* FbxMeshObj = (FbxMesh*)Attribute;
			FbxVector4* V = FbxMeshObj->GetControlPoints();
			D3D11PosUVNormalVertexBuffer::PosUVNormalVertex VertexObj;
			for (int ControlPointIndex = 0, c = FbxMeshObj->GetControlPointsCount(); ControlPointIndex < c; ++ControlPointIndex)
			{
				VertexObj.Pos = Vector4(V[ControlPointIndex][0], V[ControlPointIndex][2], V[ControlPointIndex][1], V[ControlPointIndex][3]);
				
				Out.PushVertex(VertexObj);
			}
			for (int PolygonIndex = 0, c = FbxMeshObj->GetPolygonCount(); PolygonIndex < c; ++PolygonIndex)
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

		Out.GetVertex(Vertex).UV = Vector2(UV[0], UV[1]);
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
