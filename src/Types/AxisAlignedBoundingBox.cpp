#include "Types/AxisAlignedBoundingBox.hpp"

namespace Eternal
{
	namespace Types
	{
		const Vector3& AxisAlignedBoundingBox::GetMin() const
		{
			return _Min;
		}

		const Vector3& AxisAlignedBoundingBox::GetMax() const
		{
			return _Max;
		}

		void AxisAlignedBoundingBox::SetMin(_In_ const Vector3& InMin)
		{
			_Min = InMin;
		}

		void AxisAlignedBoundingBox::SetMax(_In_ const Vector3& InMax)
		{
			_Max = InMax;
		}

		void AxisAlignedBoundingBox::Extend(_In_ const Vector3& InPosition)
		{
			_Min = Min(_Min, InPosition);
			_Max = Max(_Max, InPosition);
		}

		Vector3 AxisAlignedBoundingBox::GetOrigin() const
		{
			return (_Max + _Min) * 0.5f;
		}

		Vector3 AxisAlignedBoundingBox::GetExtent() const
		{
			return (_Max - _Min) * 0.5f;
		}

		AxisAlignedBoundingBox AxisAlignedBoundingBox::TransformBy(_In_ const Matrix4x4& InTransform) const
		{
			Vector3 Extent = GetExtent();
			Vector3 Origin = GetOrigin();

			Vector4 TransformedOrigin4	= Vector4(Origin.x) * Vector4(InTransform.m[0])
										+ Vector4(Origin.y) * Vector4(InTransform.m[1])
										+ Vector4(Origin.z) * Vector4(InTransform.m[2])
										+ Vector4(InTransform.m[3]);

			Vector4 TransformedExtent4	= Abs(Vector4(Extent.x) * Vector4(InTransform.m[0]))
										+ Abs(Vector4(Extent.y) * Vector4(InTransform.m[1]))
										+ Abs(Vector4(Extent.z) * Vector4(InTransform.m[2]));

			Vector3 TransformedOrigin(TransformedOrigin4.x, TransformedOrigin4.y, TransformedOrigin4.z);
			Vector3 TransformedExtent(TransformedExtent4.x, TransformedExtent4.y, TransformedExtent4.z);

			AxisAlignedBoundingBox BoundingBox;
			BoundingBox.SetMin(TransformedOrigin - TransformedExtent);
			BoundingBox.SetMax(TransformedOrigin + TransformedExtent);
			return BoundingBox;
		}
	}
}
