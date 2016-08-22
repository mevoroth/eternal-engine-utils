#ifndef _GAME_DATA_HPP_
#define _GAME_DATA_HPP_

#include <cstdint>

namespace Eternal
{
	namespace SaveSystem
	{
		class GameData
		{
		public:
			virtual void Prepare(_Inout_ void* Parent = nullptr) = 0;
			virtual void GetData(_Out_ uint8_t* Data) const = 0;
			virtual size_t GetSize() const = 0;
		};
	}
}

#endif
