#ifndef _GAME_DATA_SAVE_SYSTEM_HPP_
#define _GAME_DATA_SAVE_SYSTEM_HPP_

#include "SaveSystem/GameData.hpp"

namespace Eternal
{
	namespace SaveSystem
	{
		class GameDataSaveSystemCache;

		class GameDataSaveSystem : public GameData
		{
		public:
			GameDataSaveSystem();

			void RegisterGameData(_In_ GameData* GameDataObj);
			virtual void Prepare(_Inout_ void* Parent = nullptr) override;
			virtual void GetData(_Out_ uint8_t* Data) const override;
			virtual size_t GetSize() const override;

		private:
			GameDataSaveSystemCache* _SaveSystemHierarchy;
		};
	}
}

#endif
