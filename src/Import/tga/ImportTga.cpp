#include "Import/tga/ImportTga.hpp"

#include "File/FileFactory.hpp"
#include "File/File.hpp"
#include "include/Tga.h"


namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::FileSystem;

		uint8_t* ImportTga::Import(_In_ const string& InPath, _Out_ uint32_t& OutWidth, _Out_ uint32_t& OutHeight)
		{
			if (InPath.size() <= 3 ||
				InPath[InPath.size() - 3] != 't' ||
				InPath[InPath.size() - 2] != 'g' ||
				InPath[InPath.size() - 1] != 'a')
			{
				OutHeight = OutWidth = 0;
				return nullptr;
			}

			FileContent TgaFileContent = LoadFileToMemory(InPath);

			Tga::TgaImage TgaImageLoader(TgaFileContent.Content, TgaFileContent.Size);
			uint8_t* ImageBuffer = TgaImageLoader.GetImage(OutWidth, OutHeight);
	
			UnloadFileFromMemory(TgaFileContent);

			return ImageBuffer;
		}
	}
}
