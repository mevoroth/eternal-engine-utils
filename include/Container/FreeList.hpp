#pragma once

#include <functional>

namespace Eternal
{
	namespace Container
	{
		using namespace std;

		template<typename TypeT>
		class FreeList
		{
		public:

			FreeList(_In_ const function<TypeT()>& InFreeListConstructorFunction, _In_ uint32_t Count)
				: _Count(Count)
			{
				ETERNAL_ASSERT(Count > 0);
				_List.resize(Count);
			
				for (uint32_t Index = 0; Index < Count; ++Index)
					_List[Index] = InFreeListConstructorFunction();
			}

			bool Empty() const
			{
				return _List.size() == 0;
			}

			TypeT Pop()
			{
				ETERNAL_ASSERT(!Empty());
				TypeT Object = _List.back();
				_List.pop_back();
				return Object;
			}

			void Push(_In_ TypeT Object)
			{
				ETERNAL_ASSERT(_List.size() < _Count);
				_List.push_back(Object);
			}

		private:
			vector<TypeT>	_List;
			uint32_t		_Count	= 0;
		};
	}
}
