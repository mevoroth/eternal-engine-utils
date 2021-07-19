#pragma once

#include "fbxsdk.h"

#include "Mesh/GenericMesh.hpp"
#include "Mesh/BoundingBox.hpp"
#include "Resources/Payload.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace FBXSDK_NAMESPACE;
		using namespace Eternal::Components;
		using namespace Eternal::Resources;

		class ImportFbx
		{
		public:
			ImportFbx();
			static ImportFbx* Get();
			MeshPayload Import(_In_ const std::string& InPath);

		private:
			enum Channel;

			static ImportFbx*		_Instance;
			FbxManager*				_SdkManager;
			FbxIOSettings*			_Settings;
			FbxImporter*			_FbxImporter;

			//void _ImportTextureFromFBX(_In_ FbxSurfaceMaterial* SurfaceMaterial, _In_ const Channel& ChannelIndex, _In_ const char* TextureSuffix, _Out_ Texture*& OutTexture);
			void _ImportNode(_In_ const FbxNode* InNode, _Out_ Mesh& OutMesh, _Inout_ BoundingBox& InOutBoundingBox, _Inout_ MeshPayload& InOutMeshPayload);
			void _GetUV(_In_ FbxMesh* InMesh, _In_ uint32_t PolygonIndex, _Inout_ Mesh& Out);
			void _GetNormal(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ Mesh& InOutMesh);
			void _GetTangent(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ Mesh& InOutMesh);
			void _GetBinormal(_In_ FbxMesh* InMesh, _In_ int InControlPointIndex, _In_ int InVertexIndex, _Inout_ Mesh& InOutMesh);

			void _ImportPoses(_In_ FbxScene* InScene);
			void _ImportSkeletal(_In_ FbxMesh* InMesh);
			void _GetBlendShape(_In_ FbxMesh* InMesh);
			void _GetSkinning(_In_ FbxMesh* InMesh);

			enum Channel
			{
				DIFFUSE_COLOR_CHANNEL = 0,
				SPECULAR_COLOR_CHANNEL = 1,
				NORMAL_CHANNEL = 2
			};
		};
	}
}
