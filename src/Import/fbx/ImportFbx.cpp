#include "Import/fbx/ImportFbx.hpp"

#include "d3d11/D3D11VertexPosNormTex.hpp"

using namespace Eternal::Import;

ImportFbx* ImportFbx::_inst = 0;

ImportFbx::ImportFbx()
{
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
	switch (attr->GetAttributeType())
	{
	case FbxNodeAttribute::EType::eMesh:
		FbxMesh* fbxMesh = (FbxMesh*)attr;
		FbxVector4* v = fbxMesh->GetControlPoints();
		Eternal::Graphics::D3D11VertexPosNormTex vertex;
		for (int i = 0, c = fbxMesh->GetControlPointsCount(); i < c; ++i)
		{
			vertex.Pos = XMVectorSet(v[i][0] / v[i][2], v[i][1] / v[i][2], v[i][2] / v[i][2], 1.f);
			mesh.PushVertex(vertex);
		}
		break;
	}

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		mesh.PushMesh(_ImportNode(node->GetChild(i)));
	}

	return mesh;
}
