#include "SaveSystem/GameDataCollection.hpp"

#include "Macros/Macros.hpp"
#include "include/json/json.h"
#include "SaveSystem/SaveSystem.hpp"
#include "SaveSystem/GameDataLoader.hpp"

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

bool GameDataCollection::CanLoad(_In_ const void* SerializedData) const
{
	Json::Value& Collection = *(Json::Value*)SerializedData;
	return Collection.isArray();
}

void* GameDataCollection::Load(_In_ const void* SerializedData)
{
	Json::Value& Collection = *(Json::Value*)SerializedData;
	ETERNAL_ASSERT(Collection.isArray());

	vector<void*>* Scene = new vector<void*>();
	for (uint32_t GameObjectIndex = 0; GameObjectIndex < Collection.size(); ++GameObjectIndex)
	{
		GameData* Loader = SaveSystem::Get()->GetGameDataLoader()->FindLoader(&Collection[GameObjectIndex]);
		Scene->push_back(Loader->Load(&Collection[GameObjectIndex]));
	}

	return Scene;
}
