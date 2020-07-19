#include "SaveSystem/GameDataSaveSystem.hpp"

#include "include/json/json.h"
#include "SaveSystem/GameDataCollection.hpp"

using namespace Eternal::SaveSystem;

namespace Eternal
{
	namespace SaveSystem
	{
		class GameDataSaveSystemCache
		{
		public:
			Json::Value Value;
			GameDataCollection Hierarchy;
		};
	}
}

GameDataSaveSystem::GameDataSaveSystem()
{
	_SaveSystemHierarchy = new GameDataSaveSystemCache();
}

void GameDataSaveSystem::RegisterGameData(_In_ GameData* GameDataObj)
{
	ETERNAL_ASSERT(GameDataObj);
	_SaveSystemHierarchy->Hierarchy.RegisterGameData(GameDataObj);
}

void GameDataSaveSystem::Prepare(_Inout_ void* Parent)
{
	ETERNAL_ASSERT(!Parent);
	_SaveSystemHierarchy->Hierarchy.Prepare(&_SaveSystemHierarchy->Value);
}

void GameDataSaveSystem::GetData(_Out_ uint8_t* Data) const
{
	Json::FastWriter JsonWriter;
	string JsonString = JsonWriter.write(_SaveSystemHierarchy->Value);
	JsonString._Copy_s((char*)Data, JsonString.size(), JsonString.size());
	//_SaveSystemHierarchy->Hierarchy.GetData(Data);
}

size_t GameDataSaveSystem::GetSize() const
{
	Json::FastWriter JsonWriter;
	return JsonWriter.write(_SaveSystemHierarchy->Value).size();
	//return _SaveSystemHierarchy->Hierarchy.GetSize();
}

bool GameDataSaveSystem::CanLoad(_In_ const void* SerializedData) const
{
	return true;
}

void* GameDataSaveSystem::Load(_In_ const void* SerializedData)
{
	Json::Reader JsonReader;
	Json::Value RootNode;
	if (JsonReader.parse((const char*)SerializedData, RootNode))
	{
		return _SaveSystemHierarchy->Hierarchy.Load(&RootNode);
	}
	ETERNAL_ASSERT(false);
	return nullptr;
}
