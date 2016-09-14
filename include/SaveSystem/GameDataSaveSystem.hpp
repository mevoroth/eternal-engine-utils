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
			virtual bool CanLoad(_In_ const void* SerializedData) const override;
			virtual void* Load(_In_ const void* SerializedData) override;

		private:
			GameDataSaveSystemCache* _SaveSystemHierarchy = nullptr;
		};
	}
}

#endif
