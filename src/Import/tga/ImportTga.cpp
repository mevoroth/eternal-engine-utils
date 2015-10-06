#include "Import/tga/ImportTga.hpp"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include "Macros/Macros.hpp"

#include "tga.h"

using namespace Eternal::Import;

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
	TGA* TGAFile = TGAOpen(Path.c_str(), "r");
	TGAData TGADataObj;

	char buffer[256];

	int Ret = TGAReadImage(TGAFile, &TGADataObj);
	ETERNAL_ASSERT(Ret == TGA_OK);

	Width = TGAFile->hdr.width;
	Height = TGAFile->hdr.height;

	// Only 32bits format supported now
	ETERNAL_ASSERT(TGAFile->hdr.depth == 32);

	uint8_t* ImageBuffer = new uint8_t[Width * Height * 4];

	int Pixel = 0;
	for (int Y = TGAFile->hdr.height - 1; Y >= 0; --Y)
	{
		int Row = Y * TGAFile->hdr.width;
		for (int X = 0; X < TGAFile->hdr.width; ++X)
		{
			int ImgDataPixel = (Row + X) * 4;
			int ImgBufferPixel = Pixel * 4;
			ImageBuffer[ImgBufferPixel] = TGADataObj.img_data[ImgDataPixel];
			ImageBuffer[ImgBufferPixel + 1] = TGADataObj.img_data[ImgDataPixel + 1];
			ImageBuffer[ImgBufferPixel + 2] = TGADataObj.img_data[ImgDataPixel + 2];
			ImageBuffer[ImgBufferPixel + 3] = TGADataObj.img_data[ImgDataPixel + 3];
			++Pixel;
		}
	}

	TGAClose(TGAFile);

	return ImageBuffer;
}
