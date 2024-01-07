#pragma once

#if ETERNAL_PLATFORM_WINDOWS
#include "fbxsdk.h"
#endif

#include "Resources/Payload.hpp"

namespace Eternal
{
	namespace Resources
	{
		struct MaterialDependency;
	}

	namespace Types
	{
		class AxisAlignedBoundingBox;
	}

	namespace Import
	{
#if ETERNAL_PLATFORM_WINDOWS
		using namespace FBXSDK_NAMESPACE;
#endif
		using namespace Eternal::Types;
		using namespace Eternal::Resources;

		namespace ImportFbxPrivate
		{
			class Mesh;
			struct FbxTextureCache;
		}

		class ImportMat;

		class ImportFbx
		{
		public:
			ImportFbx(_In_ ImportMat* InImportMat);
			void Import(_In_ const std::string& InPath, _Out_ MeshPayload& OutMeshPayload);

		private:
#if ETERNAL_PLATFORM_WINDOWS
			FbxManager*		_SdkManager;
			FbxIOSettings*	_Settings;
			FbxImporter*	_FbxImporter;

			//void _ImportTextureFromFBX(_In_ FbxSurfaceMaterial* SurfaceMaterial, _In_ const Channel& ChannelIndex, _In_ const char* TextureSuffix, _Out_ Texture*& OutTexture);
			//void _Flatten_Combine(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload);
			void _Flatten_Split_SingleMesh(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload);
			//void _Flatten_Split_MultipleMeshes(_In_ const ImportFbxPrivate::Mesh& InMesh, _Inout_ MeshPayload& InOutMeshPayload);

			void _ImportTextures(_In_ FbxScene* InScene, _Inout_ ImportFbxPrivate::FbxTextureCache& InOutTextureCache, _Inout_ MeshPayload& InOutMeshPayload);
			void _ImportNode(_In_ FbxNode* InNode, _In_ const ImportFbxPrivate::FbxTextureCache& InTextureCache, _Inout_ ImportFbxPrivate::Mesh& InOutMesh, _Inout_ AxisAlignedBoundingBox& InOutBoundingBox, _Inout_ MaterialDependency& InOutMaterialDependency, _Inout_ MeshPayload& InOutMeshPayload);

			void _ImportPoses(_In_ FbxScene* InScene);
			void _ImportSkeletal(_In_ FbxMesh* InMesh);
			void _GetBlendShape(_In_ FbxMesh* InMesh);
			void _GetSkinning(_In_ FbxMesh* InMesh);
#endif

			void _CreateTextureRequests(_In_ const MaterialDependency& InMaterialDependency, _Inout_ MeshPayload& InOutMeshPayload);
			void _CreateRenderBuckets(_Inout_ MeshPayload& InOutMeshPayload);

			ImportMat* _ImportMat = nullptr;
		};
	}
}
