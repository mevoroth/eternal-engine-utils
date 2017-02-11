#ifndef _BREADTH_FIRST_QUAD_TREE_HPP_
#define _BREADTH_FIRST_QUAD_TREE_HPP_

#include "Types/Types.hpp"
#include "Container/QuadTree.hpp"

namespace Eternal
{
	namespace Container
	{
		using namespace Eternal::Types;
		template<class Element, u32 LODCount = 8>
		class BreadthFirstQuadTree : public QuadTree<Element>
		{
		public:
			BreadthFirstQuadTree(_In_ const Vector2& Min = Vector2(-1024.0f, -1024.0f), _In_ const Vector2& Max = Vector2(1024.0f, 1024.0f))
				: _Min(Min)
				, _Max(Max)
			{
				ETERNAL_STATIC_ASSERT(LODCount > 0, "[BreadthFirstQuadTree::BreadthFirstQuadTree]LODCount == 0");
				_Data.resize(GetContainerSize());
			}

			virtual constexpr u64 GetContainerSize() const override
			{
				return GetOffset(LODCount);
			}
			virtual u64 GetKeyFromPosition(const Vector2& Position, u32 LOD) const override
			{
				u64 Offset = GetOffset(LOD);
				u64 Key = 0ull;
				u32 SubdivisionCount = 1 << LOD;

				Vector2 CurrentMin = _Min;
				Vector2 CurrentMax = _Max;

				for (;LOD > 0; --LOD)
				{
					Vector2 Center = (CurrentMin + CurrentMax) / 2.0f;
					u64 Block = 0ull;

					if (Position.y >= Center.y)
					{
						Block |= 0x2;
						CurrentMin.y = Center.y;
					}
					else
					{
						CurrentMax.y = Center.y;
					}

					if (Position.x >= Center.x)
					{
						Block |= 0x1;
						CurrentMin.x = Center.x;
					}
					else
					{
						CurrentMax.x = Center.x;
					}

					Key = (Key << 2) | Block;
				}

				return Offset + Key;
			}

			virtual bool IsValidKey(u64 Key) const override
			{
				return Key < GetContainerSize();
			}

			virtual bool IsValidPosition(const Vector2& Position) const override
			{
				return Position >= _Min && Position < _Max;
			}

			virtual bool IsValidLOD(_In_ u32 LOD)
			{
				return LOD < LODCount;
			}

		private:
			struct BreadthFirstQuadTreeInternal
			{
				u32 Random;
			};

			u64 GetOffset(u32 Level) const
			{
				u64 ContainerOffset = 0ull;
				for (int LODIndex = 0; LODIndex < Level; ++LODIndex)
				{
					ContainerOffset += 1 << (2 * LODIndex);
				}
				return ContainerOffset;
			}

			Vector2 _Min = Vector2(-1024.0f, -1024.0f);
			Vector2 _Max = Vector2(1024.0f, 1024.0f);
		};
	}
}

#endif
