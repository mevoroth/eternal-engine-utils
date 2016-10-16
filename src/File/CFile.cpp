#include "File/CFile.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::File;

bool CFile::Exists(const std::string& FileName)
{
	ETERNAL_ASSERT(FileName.size());
	std::FILE* File = nullptr;
	errno_t Err = fopen_s(&File, FileName.c_str(), "rb");
	bool FileExists = !Err && File;
	if (File)
		fclose(File);
	return FileExists;
}

CFile::CFile(const std::string& FileName)
	: _FileName(FileName)
{
}

void CFile::Open(const OpenMode& OpenModeObj)
{
	char* Flags;

	switch (OpenModeObj)
	{
	case READ:
		Flags = "rb";
		break;
	case WRITE:
		Flags = "wb";
		break;
	case READ | WRITE:
		Flags = "r+b";
		break;

	default:
		ETERNAL_ASSERT(false);
		return;
	}
	errno_t Err = fopen_s(&_File, _FileName.c_str(), Flags);
	ETERNAL_ASSERT(!Err);
	ETERNAL_ASSERT(_File);
}

void CFile::Close()
{
	fclose(_File);
}

void CFile::Read(uint8_t* Block, uint64_t Size)
{
	fread(Block, Size, 1, _File);
}

void CFile::Write(uint8_t* Block, uint64_t Size)
{
	fwrite(Block, Size, 1, _File);
}

void CFile::Seek(uint64_t Offset, const Origin& OriginObj)
{
	ETERNAL_ASSERT(Offset < 0xFFFFFFFFull);
	fseek(_File, (int)Offset, (int)OriginObj);
}

uint64_t CFile::Tell() const
{
	return ftell(_File);
}

uint64_t CFile::GetFileSize()
{
	uint64_t Current = Tell();
	Seek(0, END);
	uint64_t Size = Tell();
	Seek(Current, SET);
	return Size;
}