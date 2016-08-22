#include "SaveSystem/GameDataCollection.hpp"

using namespace Eternal::SaveSystem;

void GameDataCollection::RegisterGameData(_In_ GameData* GameDataObj)
{
	_GameData.push_back(GameDataObj);
}

void GameDataCollection::Prepare(_Inout_ void* Parent)
{
	for (int GameDataIndex = 0; GameDataIndex < _GameData.size(); ++GameDataIndex)
	{
		_GameData[GameDataIndex]->Prepare(Parent);
	}
}

void GameDataCollection::GetData(_Out_ uint8_t* Data) const
{
	size_t Offset = 0ull;
	for (int GameDataIndex = 0; GameDataIndex < _GameData.size(); ++GameDataIndex)
	{
		_GameData[GameDataIndex]->GetData(Data + Offset);
		Offset += _GameData[GameDataIndex]->GetSize();
	}
}

size_t GameDataCollection::GetSize() const
{
	size_t TotalDataSize = 0;
	for (int GameDataIndex = 0; GameDataIndex < _GameData.size(); ++GameDataIndex)
	{
		TotalDataSize += _GameData[GameDataIndex]->GetSize();
	}
	return TotalDataSize;
}
