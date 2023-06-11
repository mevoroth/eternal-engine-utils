#pragma once

#include "Types/Types.hpp"

namespace Eternal
{
	namespace Container
	{
		using namespace Eternal::Types;

		template<class Element>
		class QuadTree
		{
		public:
			virtual u64 GetContainerSize() const = 0;
			virtual u64 GetKeyFromPosition(_In_ const Vector2& Position, _In_ u32 LOD) const = 0;
			virtual bool IsValidKey(_In_ u64 Key) const = 0;
			virtual bool IsValidPosition(_In_ const Vector2& Position) const = 0;
			virtual bool IsValidLOD(_In_ u32 LOD) = 0;

			Element& GetNode(_In_ const Vector2& Position, _In_ u32 LOD)
			{
				ETERNAL_ASSERT(IsValidPosition(Position));
				ETERNAL_ASSERT(IsValidLOD(LOD));
				return _Data[GetKeyFromPosition(Position, LOD)];
			}

		protected:
			std::vector<Element> _Data;
		};
	}
}
