#include "Import/tga/ImportTga.hpp"

//#define WIN32_LEAN_AND_MEAN
//#define VC_EXTRALEAN
//#include <Windows.h>
//#undef CreateFile
//#include <Shlwapi.h>

#include "Macros/Macros.hpp"
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

	uint8_t* ImageBuffer;

	File::File* TgaFile = File::CreateFile(Path);
	TgaFile->Open(File::File::READ);
	uint64_t TgaFileSize = TgaFile->GetFileSize();
	uint8_t* TgaContent = new uint8_t[TgaFileSize];
	TgaFile->Read(TgaContent, TgaFileSize);
	TgaFile->Close();
	delete TgaFile;

	Tga::TgaImage TgaImageObj(TgaContent, TgaFileSize);
	TgaImageObj.GetImage(Width, Height, ImageBuffer);
	
	delete[] TgaContent;
	TgaContent = nullptr;
	TgaFile = nullptr;

	return ImageBuffer;
}
