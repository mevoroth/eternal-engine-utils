#include "Resources/ImmediateTextureFactory.hpp"

#include "File/FileFactory.hpp"
#include "Import/tga/ImportTga.hpp"

namespace Eternal
{
	namespace Resources
	{
		bool ImmediateTextureFactoryLoadTextureCallback::LoadTexture(_In_ const string& Path, _Out_ RawTextureData& OutTextureData)
		{
			using namespace Eternal::File;
			using namespace Eternal::Import;

			if (FileExists(Path))
			{
				OutTextureData.TextureData = ImportTga::Get()->Import(Path, OutTextureData.Height, OutTextureData.Width);
				return true;
			}

			return false;
		}

		bool ImmediateTextureFactoryCreateGpuResourceCallback::CreateTexture(_In_ const RawTextureData& TextureData, _Out_ Texture*& OutTexture)
		{
			__debugbreak();
			return false;
		}
	}
}
