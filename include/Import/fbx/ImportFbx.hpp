#ifndef _IMPORT_FBX_HPP_
#define _IMPORT_FBX_HPP_

#include <vector>
#include <string>
#include "fbxsdk.h"

#include "Mesh/GenericMesh.hpp"
#include "Mesh/BoundingBox.hpp"

namespace Eternal
{
	namespace Import
	{
		//using namespace fbxsdk_2015_1;
		using namespace FBXSDK_NAMESPACE;
		using namespace Eternal::Components;
		using namespace Eternal::Graphics;

		class ImportFbx
		{
		public:
			ImportFbx();
			static ImportFbx* Get();
			static void RegisterPath(const std::string& IncludePath);
			void Import(_In_ const std::string& Path, _Out_ Mesh*& OutMesh, _Out_ Mesh*& OutBoundingBox);

		private:
			enum Channel;

			static vector<string> _IncludePaths;
			static ImportFbx* _Inst;
			FbxManager* _SdkMgr;
			FbxIOSettings* _Settings;
			FbxImporter* _FbxImporter;

			//void _ImportTextureFromFBX(_In_ FbxSurfaceMaterial* SurfaceMaterial, _In_ const Channel& ChannelIndex, _In_ const char* TextureSuffix, _Out_ Texture*& OutTexture);
			void _ImportNode(_In_ const FbxNode* Node, _Out_ Mesh& OutMeshObj, _Out_ BoundingBox& OutBoundingBox);
			void _GetUV(_In_ FbxMesh* MeshObj, _In_ uint32_t PolygonIndex, _Inout_ Mesh& Out);
			void _GetNormal(_In_ FbxMesh* MeshObj, _In_ int ControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& Out);
			void _GetTangent(_In_ FbxMesh* MeshObj, _In_ int ControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& Out);
			void _GetBinormal(_In_ FbxMesh* MeshObj, _In_ int ControlPointIndex, _In_ int VertexIndex, _Inout_ Mesh& Out);

			void _ImportPoses(_In_ FbxScene* Scene);
			void _ImportSkeletal(_In_ FbxMesh* MeshObj);
			void _GetBlendShape(_In_ FbxMesh* MeshObj);
			void _GetSkinning(_In_ FbxMesh* MeshObj);

			enum Channel
			{
				DIFFUSE_COLOR_CHANNEL = 0,
				SPECULAR_COLOR_CHANNEL = 1,
				NORMAL_CHANNEL = 2
			};
		};
	}
}

#endif
