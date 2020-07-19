#ifndef _POOL_HPP_
#define _POOL_HPP_

#include "Bit/Bit.hpp"
#include "Parallel/StdAtomicU64.hpp"
#include "Parallel/Sleep.hpp"

namespace Eternal
{
	namespace Resources
	{
		using namespace Eternal::Bit;
		using namespace Eternal::Parallel;

		typedef uint32_t PoolHandle;
		constexpr PoolHandle InvalidHandle = 0xFFFFFFFF;

		template<class Resource, size_t Size = 1024>
		class Pool
		{
		public:
			Pool()
			{
				ETERNAL_STATIC_ASSERT(Size > 0, "Pool size cannot be equal to 0");

				memset(_ResourcesCluster, 0x0, ETERNAL_ARRAYSIZE(_ResourcesCluster) * sizeof(ResourceCluster));

				StdAtomicU64* ResourcesCluster = new StdAtomicU64[ETERNAL_ARRAYSIZE(_ResourcesCluster)];
				for (uint32_t ResourceClusterIndex = 0; ResourceClusterIndex < ETERNAL_ARRAYSIZE(_ResourcesCluster); ++ResourceClusterIndex)
				{
					_ResourcesCluster[ResourceClusterIndex] = &ResourcesCluster[ResourceClusterIndex];
				}
			}

			~Pool()
			{
				delete[](StdAtomicU64*)_ResourcesCluster[0];
			}

			Resource* Initialize(_Out_ PoolHandle& Handle)
			{
				for (uint32_t ResourceClusterIndex = 0; ResourceClusterIndex < ETERNAL_ARRAYSIZE(_ResourcesCluster); ++ResourceClusterIndex)
				{
					for (;;)
					{
						uint64_t Cluster = _ResourcesCluster[ResourceClusterIndex]->Load();
						if (Cluster == FullCluster)
							break;
						
						{
							uint32_t ClusterKey = ResourceClusterIndex * ResourceClusterBitCount;
							uint32_t Key = (uint32_t)LeadingZeroCount(~Cluster);
							ETERNAL_ASSERT((ClusterKey + Key) < Size);
							uint64_t NewCluster = Cluster | (1ull << (ResourceClusterBitCount - Key - 1));

							if (_ResourcesCluster[ResourceClusterIndex]->CompareAndSwap(Cluster, NewCluster))
							{
								Handle = ClusterKey + Key;
								return Get(Handle);
							}
						}
					}
				}

				Handle = InvalidHandle;
				ETERNAL_BREAK();

				return nullptr;
			}

			void Release(_In_ const PoolHandle& Handle)
			{
				uint32_t ClusterKey = Handle / ResourceClusterBitCount;
				uint32_t Key = Handle % ResourceClusterBitCount;
				uint64_t InvFlag = ~(1ull << (ResourceClusterBitCount - Key - 1));

				for (;;)
				{
					uint64_t Cluster = _ResourcesCluster[ClusterKey]->Load();
					if (_ResourcesCluster[ClusterKey]->CompareAndSwap(Cluster, Cluster & InvFlag))
						break;

					Sleep(1);
				}
			}

			bool IsValid(_In_ const PoolHandle& Handle)
			{
				uint32_t ClusterKey = Handle / ResourceClusterBitCount;
				uint32_t Key = Handle % ResourceClusterBitCount;
				return Handle < Size
					&& (_ResourcesCluster[ClusterKey]->Load() & (1ull << (ResourceClusterBitCount - Key - 1))) != 0;
			}

			Resource* Get(_In_ const PoolHandle& Handle)
			{
				ETERNAL_ASSERT(IsValid(Handle));
				return &_Resources[Handle];
			}

			constexpr size_t GetSize() const { return Size; }

		private:
			typedef AtomicU64* ResourceCluster;
			static constexpr uint64_t FullCluster = 0xFFFFFFFFFFFFFFFF;
			static constexpr size_t ResourceClusterBitCount = sizeof(uint64_t) * 8;

			Resource _Resources[Size];
			ResourceCluster _ResourcesCluster[Size / ResourceClusterBitCount + 1];
		};
	}
}

#endif
