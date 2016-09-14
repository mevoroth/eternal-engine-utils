#ifndef _GAME_DATA_LOADER_HPP_
#define _GAME_DATA_LOADER_HPP_

#include <cstdint>

namespace Eternal
{
	namespace SaveSystem
	{
		class GameData;

		class GameDataLoader
		{
		public:
			virtual GameData* FindLoader(_In_ const void* SerializedData) = 0;
		};
	}
}

#endif
