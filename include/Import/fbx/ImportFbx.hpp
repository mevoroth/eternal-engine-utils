#pragma once

#include "fbxsdk.h"

#include "Mesh/BoundingBox.hpp"
#include "Resources/Payload.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace FBXSDK_NAMESPACE;
		using namespace Eternal::Components;
		using namespace Eternal::Resources;

		namespace ImportFbxPrivate
		{
			class Mesh;
		}

		class ImportFbx
		{
		public:
			ImportFbx();
			MeshPayload Import(_In_ const std::string& InPath);

		private:
			FbxManager*		_SdkManager;
			FbxIOSettings*	_Settings;
			FbxImporter*	_FbxImporter;

			//void _ImportTextureFromFBX(_In_ FbxSurfaceMaterial* SurfaceMaterial, _In_ const Channel& ChannelIndex, _In_ const char* TextureSuffix, _Out_ Texture*& OutTexture);
			void _Flatten_Combine(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload);
			void _Flatten_Split_SingleMesh(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload);
			void _Flatten_Split_MultipleMeshes(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload);

			void _ImportNode(_In_ const FbxNode* InNode, _Inout_ ImportFbxPrivate::Mesh& InOutMesh, _Inout_ BoundingBox& InOutBoundingBox, _Inout_ MeshPayload& InOutMeshPayload);
			void _ImportNode_GetUV(_In_ FbxMesh* InMesh, _In_ uint32_t PolygonIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh);
			void _ImportNode_GetNormal(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh);
			void _ImportNode_GetTangent(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh);
			void _ImportNode_GetBinormal(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ ImportFbxPrivate::Mesh& InOutMesh);

			void _ImportPoses(_In_ FbxScene* InScene);
			void _ImportSkeletal(_In_ FbxMesh* InMesh);
			void _GetBlendShape(_In_ FbxMesh* InMesh);
			void _GetSkinning(_In_ FbxMesh* InMesh);
		};
	}
}
