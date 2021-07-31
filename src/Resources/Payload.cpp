#include "Resources/Payload.hpp"

#include "File/FilePath.hpp"

namespace Eternal
{
	namespace Resources
	{
		MeshRequest::MeshRequest(_In_ const string& InPath)
			: StreamingRequest(InPath, FileType::FILE_TYPE_MESHES)
		{
		}

		TextureRequest::TextureRequest(_In_ const string& InPath)
			: StreamingRequest(InPath, FileType::FILE_TYPE_TEXTURES)
		{
		}
	}
}
