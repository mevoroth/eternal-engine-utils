#pragma once

namespace Eternal
{
	namespace Container
	{
		using namespace std;

		template<typename ContainerType>
		class StableVector
		{
		public:

			class Iterator
			{
			public:

				Iterator(_In_ StableVector<ContainerType>& InStableContainer, _In_ uint32_t InCurrentIndex = 0)
					: StableContainer(InStableContainer)
					, CurrentIndex(InCurrentIndex)
				{
				}

				ContainerType& operator*()
				{
					ETERNAL_ASSERT(CurrentIndex < StableContainer._UsedIndices.size());
					return StableContainer[StableContainer._UsedIndices[CurrentIndex]];
				}

				Iterator& operator++()
				{
					++CurrentIndex;
					return *this;
				}

				Iterator operator++(_In_ int)
				{
					return Iterator(StableContainer, CurrentIndex++);
				}

				operator bool() const
				{
					return CurrentIndex < StableContainer._UsedIndices.size();
				}

				bool operator==(_In_ const Iterator& InOtherIterator) const
				{
					return CurrentIndex == InOtherIterator.CurrentIndex;
				}

				uint32_t GetIndex() const
				{
					return StableContainer._UsedIndices[CurrentIndex];
				}

			private:

				StableVector<ContainerType>&	StableContainer;
				uint32_t						CurrentIndex = 0;
			};

			void Resize(_In_ uint32_t InCount)
			{
				_Container.resize(InCount);
				_FreeIndices.reserve(InCount);
				_UsedIndices.resize(InCount);
				for (uint32_t Index = 0; Index < InCount; ++Index)
					_UsedIndices[Index] = InCount - Index - 1u;
			}

			void Reserve(_In_ uint32_t InCount)
			{
				_Container.resize(InCount);
				_FreeIndices.resize(InCount);
				_UsedIndices.reserve(InCount);
				for (uint32_t Index = 0; Index < InCount; ++Index)
					_FreeIndices[Index] = InCount - Index - 1u;
			}

			Iterator Begin()
			{
				return Iterator(*this);
			}

			Iterator End()
			{
				return Iterator(*this, static_cast<uint32_t>(_UsedIndices.size()));
			}

			uint32_t PushBack(_In_ ContainerType&& InElement)
			{
				if (_FreeIndices.size() == 0)
				{
					_FreeIndices.push_back(static_cast<uint32_t>(_Container.size()));
					_Container.emplace_back(ContainerType());
				}

				uint32_t FreeIndex = _FreeIndices.back();
				_FreeIndices.pop_back();
				_UsedIndices.push_back(FreeIndex);
				_Container[FreeIndex] = move(InElement);
				return FreeIndex;
			}

			void Erase(_In_ uint32_t InIndex)
			{
				ETERNAL_ASSERT(InIndex < _Container.size());
				ETERNAL_ASSERT(find(_FreeIndices.begin(), _FreeIndices.end(), InIndex) == _FreeIndices.end());
				auto UsedIndicesIterator = find(_UsedIndices.begin(), _UsedIndices.end(), InIndex);
				if (UsedIndicesIterator != _UsedIndices.end())
				{
					_UsedIndices.erase(UsedIndicesIterator);
					_FreeIndices.push_back(InIndex);
				}
			}

			ContainerType& operator[](_In_ uint32_t InIndex)
			{
				ETERNAL_ASSERT(InIndex < _Container.size());
				ETERNAL_ASSERT(find(_FreeIndices.begin(), _FreeIndices.end(), InIndex) == _FreeIndices.end());
				return _Container[InIndex];
			}

			const ContainerType& operator[](_In_ uint32_t InIndex) const
			{
				ETERNAL_ASSERT(InIndex < _Container.size());
				ETERNAL_ASSERT(find(_FreeIndices.cbegin(), _FreeIndices.cend(), InIndex) == _FreeIndices.end());
				return _Container[InIndex];
			}

		private:

			vector<ContainerType> _Container;
			vector<uint32_t> _FreeIndices;
			vector<uint32_t> _UsedIndices;

			friend class Iterator;

		};
	}
}
