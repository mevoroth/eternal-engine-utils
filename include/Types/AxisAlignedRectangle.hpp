#pragma once

namespace Eternal
{
	namespace Types
	{
		class AxisAlignedRectangle
		{
		public:

			struct OriginAndExtent
			{
				Vector2 Origin;
				Vector2 Extent;
			};

			struct MinAndMaxBounds
			{
				Vector2 MinBounds;
				Vector2 MaxBounds;
			};

			AxisAlignedRectangle() = default;
			AxisAlignedRectangle(_In_ const OriginAndExtent& InOriginAndExtent);
			AxisAlignedRectangle(_In_ const MinAndMaxBounds& InMinAndMaxBounds);

			float GetArea() const;

			const Vector2& GetMinBounds() const { return _MinBounds; }
			Vector2& GetMinBounds() { return _MinBounds; }
			const Vector2& GetMaxBounds() const { return _MaxBounds; }
			Vector2& GetMaxBounds() { return _MaxBounds; }
			Vector2 GetSize() const;

		private:

			Vector2 _MinBounds = Vector2::PositiveInfinity;
			Vector2 _MaxBounds = Vector2::NegativeInfinity;

		};
	}
}
