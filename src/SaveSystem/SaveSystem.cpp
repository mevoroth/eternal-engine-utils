#include "SaveSystem/SaveSystem.hpp"

#include "File/File.hpp"
#include "File/FileFactory.hpp"
#include "File/FilePath.hpp"
#include "SaveSystem/GameDataSaveSystem.hpp"

using namespace Eternal::SaveSystem;

SaveSystem* SaveSystem::_Inst = nullptr;
//vector<string> SaveSystem::_IncludePaths;

SaveSystem::SaveSystem()
{
	ETERNAL_ASSERT(!_Inst);

	_SaveSystem = new GameDataSaveSystem();

	_Inst = this;
}

SaveSystem* SaveSystem::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

//void SaveSystem::RegisterSavePath(_In_ const string& SavePath)
//{
//	_IncludePaths.push_back(SavePath);
//}

void* SaveSystem::Load(_In_ const string& SaveFile)
{
	//string FilePath = FilePath::Find(SaveFile, FileType::SAVES);

	//Eternal::File::File* Save = CreateFileHandle(FilePath);
	//Save->Open(Eternal::File::File::READ);

	//uint64_t SaveFileSize = Save->GetFileSize();
	//uint8_t* SaveContent = new uint8_t[SaveFileSize];
	//memset(SaveContent, 0x0, SaveFileSize);

	//Save->Read(SaveContent, SaveFileSize);
	//Save->Close();
	//DestroyFileHandle(Save);

	//return _SaveSystem->Load(SaveContent);
	return nullptr;
}

void SaveSystem::SetGameDataLoader(_In_ GameDataLoader* Loader)
{
	_Loader = Loader;
}

GameDataLoader* SaveSystem::GetGameDataLoader()
{
	return _Loader;
}
