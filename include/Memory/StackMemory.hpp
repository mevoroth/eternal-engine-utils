#pragma once

namespace Eternal
{
	namespace Memory
	{
		template<typename AllocationType>
		class StackAllocation
		{
		public:
			StackAllocation(_In_ void* InMemory, _In_ size_t InElementSize, _In_ uint32_t InElementsCount)
				: _Memory(reinterpret_cast<uint8_t*>(InMemory))
				, _ElementSize(InElementSize)
				, _ElementsCount(InElementsCount)
			{
			}

			~StackAllocation()
			{
				for (uint32_t ElementIndex = 0; ElementIndex < _SubAllocationCount; ++ElementIndex)
					reinterpret_cast<AllocationType*>(& _Memory[_ElementSize * ElementIndex])->~AllocationType();
			}

			AllocationType* SubAllocate()
			{
				ETERNAL_ASSERT(_SubAllocationCount < _ElementsCount);
				return reinterpret_cast<AllocationType*>(&_Memory[_ElementSize * _SubAllocationCount++]);
			}

		private:
			uint8_t* _Memory				= nullptr;
			size_t _ElementSize				= 0u;
			uint32_t _ElementsCount			= 0u;
			uint32_t _SubAllocationCount	= 0u;
		};
	}
}
