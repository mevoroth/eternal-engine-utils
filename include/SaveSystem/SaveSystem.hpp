#ifndef _SAVE_SYSTEM_HPP_
#define _SAVE_SYSTEM_HPP_

#include <vector>
#include <string>

namespace Eternal
{
	namespace SaveSystem
	{
		using namespace std;

		class GameDataLoader;
		class GameDataSaveSystem;

		class SaveSystem
		{
		public:
			static SaveSystem* Get();

			SaveSystem();
			static void RegisterSavePath(_In_ const string& SavePath);
			void* Load(_In_ const string& SaveFile);
			void SetGameDataLoader(_In_ GameDataLoader* Loader);
			GameDataLoader* GetGameDataLoader();

		private:
			static SaveSystem* _Inst;
			static vector<string> _IncludePaths;

			GameDataSaveSystem* _SaveSystem = nullptr;
			GameDataLoader* _Loader = nullptr;
		};
	}
}

#endif
