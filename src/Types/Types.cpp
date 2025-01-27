#include "Types/Types.hpp"
#include "Types/AxisAlignedBoundingBox.hpp"
#include <cmath>

namespace Eternal
{
	namespace Types
	{
		Matrix3x4 Matrix3x4::Identity(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f
		);
		Matrix4x4 Matrix4x4::Identity(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		Vector4 Vector4::ZeroPosition(0.0f, 0.0f, 0.0f, 1.0f);

		Quaternion Quaternion::QuaternionIdentity(0.0f, 0.0f, 0.0f, 1.0f);
		
		Vector2 Vector2::Right(	 1.0f,	 0.0f);
		Vector2 Vector2::Up(	 0.0f,	 1.0f);
		Vector2 Vector2::Left(	-1.0f,	 0.0f);
		Vector2 Vector2::Down(	 0.0f,	-1.0f);

		Vector3 Vector3::Right(				 1.0f,  0.0f,  0.0f);
		Vector3 Vector3::Up(				 0.0f,  1.0f,  0.0f);
		Vector3 Vector3::Forward(			 0.0f,  0.0f,  1.0f);
		Vector3 Vector3::Left(				-1.0f,  0.0f,  0.0f);
		Vector3 Vector3::Down(				 0.0f, -1.0f,  0.0f);
		Vector3 Vector3::Backward(			 0.0f,  0.0f, -1.0f);
		Vector3	Vector3::NegativeInfinity(	-std::numeric_limits<float>::infinity());
		Vector3 Vector3::PositiveInfinity(	 std::numeric_limits<float>::infinity());

		Vector4 Vector4::RightPosition(		 1.0f,  0.0f,  0.0f, 1.0f);
		Vector4 Vector4::UpPosition(		 0.0f,  1.0f,  0.0f, 1.0f);
		Vector4 Vector4::ForwardPosition(	 0.0f,  0.0f,  1.0f, 1.0f);
		Vector4 Vector4::LeftPosition(		-1.0f,  0.0f,  0.0f, 1.0f);
		Vector4 Vector4::DownPosition(		 0.0f, -1.0f,  0.0f, 1.0f);
		Vector4 Vector4::BackwardPosition(	 0.0f,  0.0f, -1.0f, 1.0f);

		Vector4 Vector4::One(1.0f);
		Vector3 Vector3::One(1.0f);
		Vector2 Vector2::One(1.0f);
		Vector4 Vector4::Zero(0.0f);
		Vector3 Vector3::Zero(0.0f);
		Vector2 Vector2::Zero(0.0f);

		//////////////////////////////////////////////////////////////////////////

		Quaternion operator*(_In_ const Quaternion& A, _In_ const Euler& B)
		{
			return A * ToQuaternion(B);
		}
		Vector4 operator*(_In_ float A, _In_ const Vector4& B)
		{
			return B * A;
		}
		Vector3 operator*(_In_ float A, _In_ const Vector3& B)
		{
			return B * A;
		}
		Vector2 operator*(_In_ float A, _In_ const Vector2& B)
		{
			return B * A;
		}
		float SquareLength(_In_ const Vector4& V)
		{
			return Dot(V, V);
		}
		float SquareLength(_In_ const Vector3& V)
		{
			return Dot(V, V);
		}
		float Length(_In_ const Vector4& V)
		{
			return sqrt(SquareLength(V));
		}
		float Length(_In_ const Vector3& V)
		{
			return sqrt(SquareLength(V));
		}
		Vector4 Normalize(_In_ const Vector4& V)
		{
			float VectorLength = Length(V);
			ETERNAL_ASSERT(VectorLength > 0.0f);
			return V / VectorLength;
		}
		Vector3 Normalize(_In_ const Vector3& V)
		{
			float VectorLength = Length(V);
			ETERNAL_ASSERT(VectorLength > 0.0f);
			return V / VectorLength;
		}

		//////////////////////////////////////////////////////////////////////////

		Plane::Plane(_In_ const Vector4& InVector4)
			: Vector4(InVector4)
		{
		}

		////////////////////////////////////////////////////////////////////////////

		float Plane::PointProject(_In_ const Vector4& InPoint) const
		{
			return Dot(InPoint, *this);
		}

		////////////////////////////////////////////////////////////////////////////

		bool Frustum::Intersect(_In_ const AxisAlignedBoundingBox& InBoundingBox) const
		{
			const Vector3& BoxOrigin = InBoundingBox.GetOrigin();
			const Vector3& BoxExtent = InBoundingBox.GetExtent();

			for (uint32_t PlaneIndex = 0; PlaneIndex < FrustumPlanesCount; ++PlaneIndex)
			{
				Vector4 AxisVertex(
					FrustumPlanes[PlaneIndex].x < 0.0f ? -BoxExtent.x : BoxExtent.x,
					FrustumPlanes[PlaneIndex].y < 0.0f ? -BoxExtent.y : BoxExtent.y,
					FrustumPlanes[PlaneIndex].z < 0.0f ? -BoxExtent.z : BoxExtent.z,
					0.0f
				);
				AxisVertex += Vector4(BoxOrigin, 1.0f);
				if (Dot(AxisVertex, FrustumPlanes[PlaneIndex]) < 0.0f)
					return false;
			}

			return true;
		}
	}
}
