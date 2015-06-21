#include "Import/fbx/ImportFbx.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "UtilsSettings.hpp"
#include "Types/Types.hpp"

using namespace Eternal::Import;
using namespace Eternal::Graphics;

ImportFbx* ImportFbx::_Inst = nullptr;

ImportFbx::ImportFbx()
{
	assert(!_Inst);
	_Inst = this;

	_SdkMgr = FbxManager::Create();
	_Settings = FbxIOSettings::Create(_SdkMgr, IOSROOT);

	_SdkMgr->SetIOSettings(_Settings);
	_FbxImporter = FbxImporter::Create(_SdkMgr, "");

}

ImportFbx* ImportFbx::Get()
{
	assert(_Inst);
	return _Inst;
}

void ImportFbx::Import(const std::string& Path, Mesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer>& Out)
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

void ImportFbx::_ImportNode(const FbxNode* Node, Mesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer>& Out)
{
	const FbxNodeAttribute* Attribute = Node->GetNodeAttribute();
	if (Attribute)
	{
		switch (Attribute->GetAttributeType())
		{
		case FbxNodeAttribute::EType::eMesh:
			FbxMesh* FbxMeshObj = (FbxMesh*)Attribute;
			FbxVector4* V = FbxMeshObj->GetControlPoints();
			D3D11PosUVVertexBuffer::PosUVVertex VertexObj;
			for (int i = 0, c = FbxMeshObj->GetControlPointsCount(); i < c; ++i)
			{
				VertexObj.Pos = NewVector4(V[i][0], V[i][1], V[i][2], 1.f);
				Out.PushVertex(VertexObj);
			}
			for (int i = 0, c = FbxMeshObj->GetPolygonCount(); i < c; ++i)
			{
				int PolygonSize = FbxMeshObj->GetPolygonSize(i);
				Out.PushTriangle(
					FbxMeshObj->GetPolygonVertex(i, 0),
					FbxMeshObj->GetPolygonVertex(i, 1),
					FbxMeshObj->GetPolygonVertex(i, 2)
				);
				if (PolygonSize == 4) // Quad
				{
					Out.PushTriangle(
						FbxMeshObj->GetPolygonVertex(i, 0),
						FbxMeshObj->GetPolygonVertex(i, 2),
						FbxMeshObj->GetPolygonVertex(i, 3)
					);
				}
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
		Mesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer> SubMehObj;
		_ImportNode(Node->GetChild(NodeChildIndex), SubMehObj);
		Out.PushMesh(SubMehObj);
	}
}
