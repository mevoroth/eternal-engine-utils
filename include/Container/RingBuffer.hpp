#pragma once

#include <array>

namespace Eternal
{
	namespace Container
	{
		using namespace std;
		
		template<typename TypeT, uint32_t Size>
		class RingBuffer
		{
		public:

			static constexpr uint32_t RingBufferSize = Size;

			ETERNAL_STATIC_ASSERT(Size > 0, "Size must be > 0");

			RingBuffer()
			{
			}

			void Push(_In_ const TypeT& InElement)
			{
				ETERNAL_ASSERT(_Count < Size());
				_Elements[(_First + _Count) % Size()] = InElement;
				++_Count;
			}

			inline TypeT& Head()
			{
				ETERNAL_ASSERT(!Empty());
				return _Elements[_First];
			}

			inline TypeT& operator[](_In_ const uint32_t InIndex)
			{
				ETERNAL_ASSERT(InIndex < _Count);
				return _Elements[(_First + InIndex) % Size()];
			}

			inline TypeT& Tail()
			{
				ETERNAL_ASSERT(!Empty());
				return _Elements[(_First - 1u + _Count) % Size()];
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
				return _Count == Size();
			}

			bool Empty() const
			{
				return _Count == 0;
			}

			inline constexpr uint32_t Size() const
			{
				return RingBufferSize;
			}

			inline uint32_t Count() const
			{
				return _Count;
			}

		private:
			array<TypeT, RingBufferSize> _Elements;
			uint32_t _Count = 0u;
			uint32_t _First = 0u;
		};
	}
}
