#pragma once

#include "Types/Types.hpp"

namespace Eternal
{
	namespace Types
	{
		class AxisAlignedBoundingBox
		{
		public:
			const Vector3& GetMin() const;
			const Vector3& GetMax() const;
			void SetMin(_In_ const Vector3& InMin);
			void SetMax(_In_ const Vector3& InMax);
			void Extend(_In_ const Vector3& InPosition);
			Vector3 GetOrigin() const;
			Vector3 GetExtent() const;
			AxisAlignedBoundingBox TransformBy(_In_ const Matrix4x4& InTransform) const;

		private:
			Vector3 _Min = Vector3::PositiveInfinity;
			Vector3 _Max = Vector3::NegativeInfinity;
		};
	}
}
