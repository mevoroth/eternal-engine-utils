#ifndef _IMPORT_FBX_HPP_
#define _IMPORT_FBX_HPP_

#include <vector>
#include <string>
#include "fbxsdk.h"

#include "Mesh/GenericMesh.hpp"
#include "d3d11/D3D11PosUVNormalVertexBuffer.hpp"
#include "d3d11/D3D11UInt32IndexBuffer.hpp"

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
			void Import(_In_ const std::string& Path, _Out_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out);

		private:
			static vector<string> _IncludePaths;
			static ImportFbx* _Inst;
			FbxManager* _SdkMgr;
			FbxIOSettings* _Settings;
			FbxImporter* _FbxImporter;

			void _ImportNode(_In_ const FbxNode* Node, _Out_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out);
			void _GetUV(_In_ FbxMesh* MeshObj, _In_ uint32_t PolygonIndex, _Inout_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out);
			void _GetNormal(_In_ FbxMesh* MeshObj, _In_ uint32_t PolygonIndex, _Inout_ GenericMesh<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex, D3D11PosUVNormalVertexBuffer, D3D11UInt32IndexBuffer>& Out);

			void _ImportPoses(_In_ FbxScene* Scene);
			void _ImportSkeletal(_In_ FbxMesh* MeshObj);
			void _GetBlendShape(_In_ FbxMesh* MeshObj);
			void _GetSkinning(_In_ FbxMesh* MeshObj);

			enum
			{
				DIFFUSE_COLOR_CHANNEL = 0
			};
		};
	}
}

#endif
