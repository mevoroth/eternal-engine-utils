#ifndef _IMPORT_FBX_HPP_
#define _IMPORT_FBX_HPP_

#include <string>
#include "fbxsdk.h"

#include "Mesh/Mesh.hpp"
#include "d3d11/D3D11PosUVVertexBuffer.hpp"
#include "d3d11/D3D11UInt32IndexBuffer.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::Components;
		using namespace Eternal::Graphics;

		class ImportFbx
		{
		private:
			static ImportFbx* _Inst;
			FbxManager* _SdkMgr;
			FbxIOSettings* _Settings;
			FbxImporter* _FbxImporter;

			void _ImportNode(const FbxNode* Node, Mesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer>& Out);

		public:
			ImportFbx();
			static ImportFbx* Get();
			void Import(const std::string& Path, Mesh<D3D11PosUVVertexBuffer::PosUVVertex, D3D11PosUVVertexBuffer, D3D11UInt32IndexBuffer>& Out);
		};
	}
}

#endif
