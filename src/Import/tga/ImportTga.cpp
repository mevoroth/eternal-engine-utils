#include "Import/tga/ImportTga.hpp"

#include "File/FileFactory.hpp"
#include "File/File.hpp"
#include "include/Tga.h"

using namespace Eternal::Import;
using namespace Eternal::File;

ImportTga* ImportTga::_Inst = nullptr;

ImportTga::ImportTga()
{
	ETERNAL_ASSERT(!_Inst);
	_Inst = this;
}

ImportTga* ImportTga::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

uint8_t* ImportTga::Import(_In_ const string& Path, _Out_ uint32_t& Height, _Out_ uint32_t& Width)
{
	if (Path[Path.size() - 3] != 't' ||
		Path[Path.size() - 2] != 'g' ||
		Path[Path.size() - 1] != 'a')
	{
		Height = Width = 0;
		return nullptr;
	}
	//char Dir[255];
	//GetCurrentDirectory(255, Dir);
	//ETERNAL_ASSERT(PathFileExists(Path.c_str()) == 1);

	File::File* TgaFile = CreateFileHandle(Path);
	TgaFile->Open(File::File::READ);
	uint64_t TgaFileSize = TgaFile->GetFileSize();
	uint8_t* TgaContent = new uint8_t[TgaFileSize];
	TgaFile->Read(TgaContent, TgaFileSize);
	TgaFile->Close();
	delete TgaFile;

	Tga::TgaImage TgaImageLoader(TgaContent, TgaFileSize);
	uint8_t* ImageBuffer = TgaImageLoader.GetImage(Width, Height);
	
	delete[] TgaContent;
	TgaContent = nullptr;
	TgaFile = nullptr;

	return ImageBuffer;
}
