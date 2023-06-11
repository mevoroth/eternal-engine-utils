#pragma once

#include "Types/Types.hpp"

namespace Eternal
{
	namespace Container
	{
		using namespace std;
		using namespace Eternal::Types;

		template<typename TypeT>
		class RingBuffer
		{
		public:
			RingBuffer(_In_ u32 Size)
				: _Size(Size)
			{
				ETERNAL_ASSERT(Size > 0);
				_Elements.resize(Size);
			}

			void Push(_In_ const TypeT& Element)
			{
				ETERNAL_ASSERT(_Count < _Size);
				_Elements[(_First + _Count) % _Size] = Element;
				++_Count;
			}

			inline TypeT& Head()
			{
				ETERNAL_ASSERT(!Empty());
				return _Elements[_First];
			}

			inline TypeT& operator[](_In_ const u32 Index)
			{
				ETERNAL_ASSERT(Index < _Count);
				return _Elements[(_First + Index) % _Size];
			}

			inline TypeT& Tail()
			{
				ETERNAL_ASSERT(!Empty());
				return _Elements[(_First - 1u + _Count) % _Size];
			}

			void PopHead()
			{
				ETERNAL_ASSERT(!Empty());
				++_First;
				--_Count;
			}

			void PopTail()
			{
				ETERNAL_ASSERT(!Empty());
				--_Count;
			}

			bool Full() const
			{
				return _Count == _Size;
			}

			bool Empty() const
			{
				return _Count == 0;
			}

			inline u32 Size() const
			{
				return _Size;
			}

			inline int Count() const
			{
				return _Count;
			}

		private:
			vector<TypeT> _Elements;
			u32 _Size = 0u;
			u32 _Count = 0u;
			u32 _First = 0u;
		};
	}
}
