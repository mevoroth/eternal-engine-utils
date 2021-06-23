#pragma once

#include "Math/Math.hpp"
#include "Bit/Bit.hpp"

namespace Eternal
{
	namespace Bit
	{
		using namespace Eternal::Math;

		struct BitFieldPrivateHandle
		{
			using BitFieldToken = uint64_t;

			static constexpr BitFieldToken InvalidToken = BitFieldToken(~0ull);
			static const BitFieldPrivateHandle InvalidHandle;

			BitFieldPrivateHandle(const BitFieldToken& InToken)
				: Token(InToken)
			{}

			bool IsValid() const { return Token != InvalidToken; }
			void Invalidate() { Token = InvalidToken; }
			operator uint64_t() const
			{
				ETERNAL_ASSERT(IsValid());
				return Token;
			}

			BitFieldToken Token = InvalidToken;
		};

		using Handle = BitFieldPrivateHandle;

		template<uint32_t Size = 1024, typename Storage = uint64_t>
		class BitField
		{
		public:
			
			static constexpr Storage FullChunk = Storage(0ull);
			static constexpr Storage EmptyChunk = Storage(~0ull);
			static constexpr size_t ChunkBitCount = sizeof(Storage) * 8;
			static constexpr size_t ChunkCount = DivideRoundUp<size_t>(size_t(Size), ChunkBitCount);

			BitField()
			{
				ETERNAL_STATIC_ASSERT(Size > 0, "Size must be above 0");
				ETERNAL_STATIC_ASSERT(sizeof(Storage) >= 2, "Storage must be at least 16bits");
				memset(_Field, EmptyChunk, sizeof(Storage) * ChunkCount);
			}

			Handle Pop()
			{
				for (uint32_t ChunkIndex = 0; ChunkIndex < ETERNAL_ARRAYSIZE(_Field); ++ChunkIndex)
				{
					if (_Field[ChunkIndex] != FullChunk)
					{
						uint64_t FreeHandle = LeadingZeroCount(_Field[ChunkIndex]);
						uint64_t Offset = ChunkBitCount - FreeHandle - 1ull;
						Storage Key = ~(Storage(1) << Offset);
						_Field[ChunkIndex] &= Key;
						return Handle(ChunkIndex * ChunkBitCount + FreeHandle);
					}
				}

				ETERNAL_BREAK(); // Running out of handle
				return Handle::InvalidHandle;
			}

			void Push(Handle& InOutHandle)
			{
				ETERNAL_ASSERT(InOutHandle.IsValid());

				uint64_t ChunkIndex = InOutHandle.Token / ChunkBitCount;
				uint64_t Offset = ChunkBitCount - InOutHandle.Token % ChunkBitCount - 1ull;
				Storage CurrentHandleState = (_Field[ChunkIndex] & (Storage(1) << Offset));
				ETERNAL_ASSERT(CurrentHandleState == FullChunk); // Ensure handle is already allocated
				_Field[ChunkIndex] |= Storage(1) << Offset;
				InOutHandle.Invalidate(); // Invalidate handle
			}

		private:
			Storage _Field[ChunkCount];
		};
	}
}
