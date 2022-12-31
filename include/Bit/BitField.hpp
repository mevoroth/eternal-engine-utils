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

			BitFieldPrivateHandle(_In_ const BitFieldToken& InToken = InvalidToken)
				: Token(InToken)
			{}

			bool IsValid() const { return Token != InvalidToken; }
			void Invalidate() { Token = InvalidToken; }
			bool IsContigous(_In_ const BitFieldPrivateHandle& InHandle) const
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
		class PoolRoutine
		{
		public:

			void Init(_Inout_ FieldType& InOutField)
			{
				InOutField.Reset();
			}

			Handle Pop(_Inout_ FieldType& InOutField)
			{
				for (uint32_t ChunkIndex = 0; ChunkIndex < InOutField.GetSize(); ++ChunkIndex)
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

			void Push(_Inout_ Handle& InOutHandle, _Inout_ FieldType& InOutField)
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

		template<typename FieldType, typename StorageType = uint64_t>
		class BitFieldRoutine
		{
		public:

			void Init(_Inout_ FieldType& InOutField)
			{
				InOutField.Reset();
			}

			void Set(_In_ uint32_t InIndex, _Inout_ FieldType& InOutField)
			{
				ETERNAL_ASSERT(!IsSet(InIndex, InOutField)); // Double set
				InOutField[InIndex / InOutField.GetChunkBitCount()] &= ~(StorageType(1) << (InIndex % InOutField.GetChunkBitCount()));
			}

			void Unset(_In_ uint32_t InIndex, _Inout_ FieldType& InOutField)
			{
				ETERNAL_ASSERT(IsSet(InIndex, InOutField)); // Double unset
				InOutField[InIndex / InOutField.GetChunkBitCount()] |= (StorageType(1) << (InIndex % InOutField.GetChunkBitCount()));
			}

			bool IsSet(_In_ uint32_t InIndex, _In_ const FieldType& InOutField) const
			{
				ETERNAL_ASSERT(InIndex < InOutField.GetBitCount());
				size_t Entry = InIndex / InOutField.GetChunkBitCount();
				uint32_t FlagIndex = InIndex % InOutField.GetChunkBitCount();
				return (InOutField[Entry] & (StorageType(1) << FlagIndex)) == StorageType(0);
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
				Reset();
			}

			constexpr StorageType GetFullChunk() const
			{
				return FullChunk;
			}

			constexpr size_t GetChunkBitCount() const
			{
				return ChunkBitCount;
			}

			constexpr uint32_t GetSize() const
			{
				return ETERNAL_ARRAYSIZE(_Field);
			}

			constexpr uint32_t GetBitCount() const
			{
				return Size;
			}

			void Reset()
			{
				std::fill(_Field, _Field + ChunkCount, EmptyChunk);
			}

			StorageType& operator[](_In_ std::size_t Index)
			{
				ETERNAL_ASSERT(Index < ChunkCount);
				return _Field[Index];
			}

			const StorageType& operator[](_In_ std::size_t Index) const
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
			static constexpr StorageType EmptyChunk = static_cast<StorageType>(~0ull);

			static constexpr size_t ChunkBitCount = sizeof(StorageType) * 8;

			BitFieldVector(_In_ uint32_t InSize = 0)
				: Size(InSize)
			{
				ETERNAL_STATIC_ASSERT(sizeof(StorageType) >= 2, "Storage must be at least 16bits");
				if (InSize > 0)
					Resize(InSize);
			}

			constexpr StorageType GetFullChunk() const
			{
				return FullChunk;
			}

			constexpr size_t GetChunkBitCount() const
			{
				return ChunkBitCount;
			}

			uint32_t GetSize() const
			{
				return static_cast<uint32_t>(_Field.size());
			}

			void Resize(_In_ uint32_t InSize)
			{
				ETERNAL_ASSERT(_Field.size() == 0);
				if (InSize > 0)
				{
					_Field.resize(DivideRoundUp<size_t>(size_t(InSize), ChunkBitCount));
					Size = InSize;
					Reset();
				}
			}

			void Reset()
			{
				std::fill(_Field.begin(), _Field.end(), EmptyChunk);
			}

			uint32_t GetBitCount() const
			{
				return Size;
			}

			StorageType& operator[](_In_ std::size_t Index)
			{
				ETERNAL_ASSERT(Index < _Field.size());
				return _Field[Index];
			}

			const StorageType& operator[](_In_ std::size_t Index) const
			{
				ETERNAL_ASSERT(Index < _Field.size());
				return _Field[Index];
			}

		private:
			std::vector<StorageType> _Field;
			uint32_t Size = 0;
		};

		template<const uint32_t Size = 1024, typename StorageType = uint64_t>
		class StaticHandlePool
			: public PoolRoutine<BitFieldArray<Size, StorageType>, StorageType>
		{
		public:

			using Routine = PoolRoutine<BitFieldArray<Size, StorageType>, StorageType>;

			StaticHandlePool()
				: Routine()
			{
				ETERNAL_STATIC_ASSERT(Size > 0, "Size must be above 0");
				Routine::Init(_Storage);
			}

			Handle Pop()
			{
				return Routine::Pop(_Storage);
			}

			void Push(_Inout_ Handle& InOutHandle)
			{
				Routine::Push(InOutHandle, _Storage);
			}

		private:
			BitFieldArray<Size, StorageType> _Storage;
		};

		template<typename StorageType = uint64_t>
		class DynamicHandlePool
			: public PoolRoutine<BitFieldVector<StorageType>, StorageType>
			, public BitFieldVector<StorageType>
		{
		public:

			using Routine = PoolRoutine<BitFieldVector<StorageType>, StorageType>;

			DynamicHandlePool(_In_ uint32_t InSize)
				: Routine()
				, BitFieldVector<StorageType>(InSize)
			{
				Routine::Init(*this);
			}

			Handle Pop()
			{
				return Routine::Pop(*this);
			}

			void Push(_Inout_ Handle& InOutHandle)
			{
				Routine::Push(InOutHandle, *this);
			}
		};

		template<typename StorageType = uint64_t>
		class DynamicBitField
			: public BitFieldRoutine<BitFieldVector<StorageType>, StorageType>
			, public BitFieldVector<StorageType>
		{
		public:

			using Routine = BitFieldRoutine<BitFieldVector<StorageType>, StorageType>;
			using Storage = BitFieldVector<StorageType>;

			DynamicBitField(_In_ uint32_t InSize = 0)
				: Routine()
				, BitFieldVector<StorageType>(InSize)
			{
				if (InSize > 0)
					Routine::Init(*this);
			}

			void Resize(_In_ uint32_t InSize)
			{
				Storage::Resize(InSize);
			}

			void Set(_In_ uint32_t InIndex)
			{
				ETERNAL_ASSERT(Storage::GetSize() > 0);
				Routine::Set(InIndex, *this);
			}

			void Unset(_In_ uint32_t InIndex)
			{
				ETERNAL_ASSERT(Storage::GetSize() > 0);
				Routine::Unset(InIndex, *this);
			}

			bool IsSet(_In_ uint32_t InIndex) const
			{
				ETERNAL_ASSERT(Storage::GetSize() > 0);
				return Routine::IsSet(InIndex, *this);
			}

			void Reset()
			{
				Storage::Reset();
			}
		};
	}
}
