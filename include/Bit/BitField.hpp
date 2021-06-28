#pragma once

#include "Math/Math.hpp"
#include "Bit/Bit.hpp"
#include <vector>
#include <algorithm>
#include <cstdlib>

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

			BitFieldPrivateHandle(const BitFieldToken& InToken = InvalidToken)
				: Token(InToken)
			{}

			bool IsValid() const { return Token != InvalidToken; }
			void Invalidate() { Token = InvalidToken; }
			bool IsContigous(const BitFieldPrivateHandle& InHandle) const
			{
				return abs(static_cast<int64_t>(*this) - static_cast<int64_t>(InHandle)) == 1;
			}
			operator uint64_t() const
			{
				ETERNAL_ASSERT(IsValid());
				return Token;
			}

			BitFieldToken Token = InvalidToken;
		};

		using Handle = BitFieldPrivateHandle;

		template<typename FieldType, typename StorageType = uint64_t>
		class BitFieldRoutine
		{
		public:

			void Init(FieldType& InOutField)
			{
				InOutField.Reset();
			}

			Handle Pop(FieldType& InOutField)
			{
				for (uint32_t ChunkIndex = 0; ChunkIndex < InOutField.Size(); ++ChunkIndex)
				{
					if (InOutField[ChunkIndex] != InOutField.GetFullChunk())
					{
						uint64_t FreeHandle = LeadingZeroCount(InOutField[ChunkIndex]);
						uint64_t Offset = InOutField.GetChunkBitCount() - FreeHandle - 1ull;
						StorageType Key = ~(StorageType(1) << Offset);
						InOutField[ChunkIndex] &= Key;
						return Handle(ChunkIndex * InOutField.GetChunkBitCount() + FreeHandle);
					}
				}

				ETERNAL_BREAK(); // Running out of handle
				return Handle::InvalidHandle;
			}

			void Push(Handle& InOutHandle, FieldType& InOutField)
			{
				ETERNAL_ASSERT(InOutHandle.IsValid());

				uint64_t ChunkIndex = InOutHandle.Token / InOutField.GetChunkBitCount();
				uint64_t Offset = InOutField.GetChunkBitCount() - InOutHandle.Token % InOutField.GetChunkBitCount() - 1ull;
				StorageType CurrentHandleState = (InOutField[ChunkIndex] & (StorageType(1) << Offset));
				ETERNAL_ASSERT(CurrentHandleState == InOutField.GetFullChunk()); // Ensure handle is already allocated
				InOutField[ChunkIndex] |= StorageType(1) << Offset;
				InOutHandle.Invalidate(); // Invalidate handle
			}
		};

		template<uint32_t Size = 1024, typename StorageType = uint64_t>
		class BitFieldArray
		{
		public:

			static constexpr StorageType FullChunk = StorageType(0ull);
			static constexpr StorageType EmptyChunk = StorageType(~0ull);
			static constexpr size_t ChunkBitCount = sizeof(StorageType) * 8;
			static constexpr size_t ChunkCount = DivideRoundUp<size_t>(size_t(Size), ChunkBitCount);

			BitFieldArray()
			{
				ETERNAL_STATIC_ASSERT(ChunkCount > 0, "Size must be above 0");
				ETERNAL_STATIC_ASSERT(sizeof(StorageType) >= 2, "Storage must be at least 16bits");
			}

			constexpr StorageType GetFullChunk()
			{
				return FullChunk;
			}

			constexpr size_t GetChunkBitCount()
			{
				return ChunkBitCount;
			}

			constexpr uint32_t Size()
			{
				return ETERNAL_ARRAYSIZE(_Field);
			}

			void Reset()
			{
				std::fill(_Field, _Field + ChunkCount, EmptyChunk);
			}

			StorageType& operator[](std::size_t Index)
			{
				ETERNAL_ASSERT(Index < ChunkCount);
				return _Field[Index];
			}

		private:
			StorageType _Field[ChunkCount];
		};

		template<typename StorageType = uint64_t>
		class BitFieldVector
		{
		public:

			static constexpr StorageType FullChunk = StorageType(0ull);
			static constexpr StorageType EmptyChunk = StorageType(~0ull);

			static constexpr size_t ChunkBitCount = sizeof(StorageType) * 8;

			BitFieldVector(uint32_t Size)
			{
				ETERNAL_ASSERT(Size > 0);
				ETERNAL_STATIC_ASSERT(sizeof(StorageType) >= 2, "Storage must be at least 16bits");

				_Field.resize(DivideRoundUp<size_t>(size_t(Size), ChunkBitCount));
			}

			constexpr StorageType GetFullChunk()
			{
				return FullChunk;
			}

			constexpr size_t GetChunkBitCount()
			{
				return ChunkBitCount;
			}

			uint32_t Size() const
			{
				return static_cast<uint32_t>(_Field.size());
			}

			void Reset()
			{
				std::fill(_Field.begin(), _Field.end(), EmptyChunk);
			}

			StorageType& operator[](std::size_t Index)
			{
				ETERNAL_ASSERT(Index < _Field.size());
				return _Field[Index];
			}

		private:
			std::vector<StorageType> _Field;
		};

		template<const uint32_t Size = 1024, typename StorageType = uint64_t>
		class StaticBitField
			: public BitFieldRoutine<BitFieldArray<Size, StorageType>, StorageType>
		{
		public:

			using Routine = BitFieldRoutine<BitFieldArray<Size, StorageType>, StorageType>;

			StaticBitField()
				: Routine()
			{
				ETERNAL_STATIC_ASSERT(Size > 0, "Size must be above 0");
				Init(_Storage);
			}

			Handle Pop()
			{
				return Routine::Pop(_Storage);
			}

			void Push(Handle& InOutHandle)
			{
				Routine::Push(InOutHandle, _Storage);
			}

		private:
			BitFieldArray<Size, StorageType> _Storage;
		};

		template<typename StorageType = uint64_t>
		class DynamicBitField
			: public BitFieldRoutine<BitFieldVector<StorageType>, StorageType>
			, public BitFieldVector<StorageType>
		{
		public:

			using Routine = BitFieldRoutine<BitFieldVector<StorageType>, StorageType>;

			DynamicBitField(uint32_t Size)
				: BitFieldRoutine<BitFieldVector<StorageType>, StorageType>()
				, BitFieldVector<StorageType>(Size)
			{
				Init(*this);
			}

			Handle Pop()
			{
				return Routine::Pop(*this);
			}

			void Push(Handle& InOutHandle)
			{
				Routine::Push(InOutHandle, *this);
			}
		};
	}
}
