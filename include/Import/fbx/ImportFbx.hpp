#ifndef _IMPORT_FBX_HPP_
#define _IMPORT_FBX_HPP_

#include <string>
#include "fbxsdk.h"

#include "Mesh/Mesh.hpp"

namespace Eternal
{
	namespace Import
	{
		using namespace Components;
		class ImportFbx
		{
		private:
			static ImportFbx* _inst;
			FbxManager* _sdkMgr;
			FbxIOSettings* _settings;
			FbxImporter* _fbxImporter;

			Mesh _ImportNode(const FbxNode* node);

		public:
			ImportFbx();
			static ImportFbx* Get();
			Mesh Import(const std::string& path);
		};
	}
}

#endif
