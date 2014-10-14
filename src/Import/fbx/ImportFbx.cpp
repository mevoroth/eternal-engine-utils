#include "Import/fbx/ImportFbx.hpp"

#include "UtilsSettings.hpp"
#include "Types/Types.hpp"

using namespace Eternal::Import;

ImportFbx* ImportFbx::_inst = 0;

ImportFbx::ImportFbx()
{
	assert(!_inst);
	_inst = this;

	_sdkMgr = FbxManager::Create();
	_settings = FbxIOSettings::Create(_sdkMgr, IOSROOT);

	_sdkMgr->SetIOSettings(_settings);
	_fbxImporter = FbxImporter::Create(_sdkMgr, "");

}

ImportFbx* ImportFbx::Get()
{
	assert(_inst);
	return _inst;
}

Mesh ImportFbx::Import(const std::string& path)
{
	if (_fbxImporter->Initialize(path.c_str(), -1, _settings))
	{
		// LOG
	}

	FbxScene* scene = FbxScene::Create(_sdkMgr, "scene");
	_fbxImporter->Import(scene);

	return _ImportNode(scene->GetRootNode());
}

Mesh ImportFbx::_ImportNode(const FbxNode* node)
{
	Mesh mesh;

	const FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr)
	{
		switch (attr->GetAttributeType())
		{
		case FbxNodeAttribute::EType::eMesh:
			FbxMesh* fbxMesh = (FbxMesh*)attr;
			FbxVector4* v = fbxMesh->GetControlPoints();
			Eternal::Graphics::Vertex vertex;
			for (int i = 0, c = fbxMesh->GetControlPointsCount(); i < c; ++i)
			{
				vertex.Pos = XMVectorSet(v[i][0] / v[i][2], v[i][1] / v[i][2], v[i][2] / v[i][2], 1.f);
				mesh.PushVertex(vertex);
			}
			for (int i = 0, c = fbxMesh->GetPolygonCount(); i < c; ++i)
			{
				int polygonSize = fbxMesh->GetPolygonSize(i);
				mesh.PushTriangle(
					fbxMesh->GetPolygonVertex(i, 0),
					fbxMesh->GetPolygonVertex(i, 1),
					fbxMesh->GetPolygonVertex(i, 2)
				);
				if (polygonSize == 4) // Quad
				{
					mesh.PushTriangle(
						fbxMesh->GetPolygonVertex(i, 0),
						fbxMesh->GetPolygonVertex(i, 2),
						fbxMesh->GetPolygonVertex(i, 3)
					);
				}
			}

			break;
		}
	}
	mesh.GetTransform().Translate(
		NewVector3(
			node->LclTranslation.Get()[0],
			node->LclTranslation.Get()[1],
			node->LclTranslation.Get()[2]
		)
	);

	mesh.GetTransform().Rotate(
		NewVector3(
			node->LclRotation.Get()[0],
			node->LclRotation.Get()[1],
			node->LclRotation.Get()[2]
		)
	);

	mesh.GetTransform().Scale(
		NewVector3(
			node->LclScaling.Get()[0],
			node->LclScaling.Get()[1],
			node->LclScaling.Get()[2]
		)
	);

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		mesh.PushMesh(_ImportNode(node->GetChild(i)));
	}

	return mesh;
}
