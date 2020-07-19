#ifndef _GAME_DATA_COLLECTION_HPP_
#define _GAME_DATA_COLLECTION_HPP_

#include "SaveSystem/GameData.hpp"

namespace Eternal
{
	namespace SaveSystem
	{
		using namespace std;

		class SaveData;

		class GameDataCollection : public GameData
		{
		public:
			void RegisterGameData(_In_ GameData* GameDataObj);

			virtual void Prepare(_Inout_ void* Parent) override;
			virtual void GetData(_Inout_ uint8_t* Data) const override;
			virtual size_t GetSize() const override;
			virtual bool CanLoad(_In_ const void* SerializedData) const override;
			virtual void* Load(_In_ const void* SerializedData) override;

		private:
			vector<GameData*> _GameData;
		};
	}
}

#endif
