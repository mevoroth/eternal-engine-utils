#include "Types/AxisAlignedRectangle.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace Types
	{
		AxisAlignedRectangle::AxisAlignedRectangle(_In_ const OriginAndExtent& InOriginAndExtent)
			: _MinBounds(InOriginAndExtent.Origin - InOriginAndExtent.Extent * 0.5f)
			, _MaxBounds(InOriginAndExtent.Origin + InOriginAndExtent.Extent * 0.5f)
		{
		}

		AxisAlignedRectangle::AxisAlignedRectangle(_In_ const MinAndMaxBounds& InMinAndMaxBounds)
			: _MinBounds(InMinAndMaxBounds.MinBounds)
			, _MaxBounds(InMinAndMaxBounds.MaxBounds)
		{
		}

		float AxisAlignedRectangle::GetArea() const
		{
			Vector2 Area = _MaxBounds - _MinBounds;
			return Math::Max(Area.x * Area.y, 0.0f);
		}

		Vector2 AxisAlignedRectangle::GetSize() const
		{
			return _MaxBounds - _MinBounds;
		}
	}
}
