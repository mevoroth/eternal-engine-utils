#pragma once

#include <DirectXMath.h>

namespace Eternal
{
	namespace Types
	{
		using namespace DirectX;

		struct Vector3;
		class AxisAlignedBoundingBox;

		struct Float2 : XMFLOAT2
		{
			using XMFLOAT2::XMFLOAT2;

			Float2()
				: Float2(0.0f)
			{
			}
			Float2(_In_ float InScalar)
				: XMFLOAT2(InScalar, InScalar)
			{
			}
		};

		struct Float3 : XMFLOAT3
		{
			using XMFLOAT3::XMFLOAT3;

			Float3()
				: Float3(0.0f)
			{
			}
			Float3(_In_ float InScalar)
				: XMFLOAT3(InScalar, InScalar, InScalar)
			{
			}
		};

		struct Float4 : XMFLOAT4A
		{
			using XMFLOAT4A::XMFLOAT4A;

			Float4()
				: Float4(0.0f)
			{
			}
			Float4(_In_ float Scalar)
				: XMFLOAT4A(Scalar, Scalar, Scalar, Scalar)
			{
			}
			Float4(_In_ const Float3& InPosition, float InW)
				: XMFLOAT4A(InPosition.x, InPosition.y, InPosition.z, InW)
			{
			}
		};

		struct Uint4 : XMUINT4
		{
			using XMUINT4::XMUINT4;

			Uint4()
				: Uint4(0u)
			{
			}

			Uint4(_In_ uint32_t InScalar)
				: XMUINT4(InScalar, InScalar, InScalar, InScalar)
			{
			}
		};

		struct Matrix4x4 : public XMFLOAT4X4A
		{
			static Matrix4x4 Identity;

			using XMFLOAT4X4A::XMFLOAT4X4A;
		};

		struct PerspectiveLHMatrix : public Matrix4x4
		{
			PerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio);
		};

		struct InversePerspectiveLHMatrix : public Matrix4x4
		{
			InversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio);
		};

		struct ReverseZPerspectiveLHMatrix : public PerspectiveLHMatrix
		{
			ReverseZPerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio);
		};

		struct ReverseZInversePerspectiveLHMatrix : public InversePerspectiveLHMatrix
		{
			ReverseZInversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio);
		};

		struct OrthographicLHMatrix : public Matrix4x4
		{
			OrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight);
		};

		struct InverseOrthographicLHMatrix : public Matrix4x4
		{
			InverseOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight);
		};

		struct ReverseZOrthographicLHMatrix : public OrthographicLHMatrix
		{
			ReverseZOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight);
		};

		struct ReverseZInverseOrthographicLHMatrix : public InverseOrthographicLHMatrix
		{
			ReverseZInverseOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight);
		};

		struct LookToLHMatrix : public Matrix4x4
		{
			LookToLHMatrix(_In_ const Vector3& InPosition, _In_ const Vector3& InForward, _In_ const Vector3& InUp);
		};

		struct Vector2 : public Float2
		{
			static Vector2 One;
			static Vector2 Zero;

			using Float2::Float2;
		};

		struct Vector3 : public Float3
		{
			static Vector3 One;
			static Vector3 Zero;
			static Vector3 Right;
			static Vector3 Up;
			static Vector3 Forward;
			static Vector3 Left;
			static Vector3 Down;
			static Vector3 Backward;
			static Vector3 NegativeInfinity;
			static Vector3 PositiveInfinity;

			using Float3::Float3;
		};

		struct Vector4 : public Float4
		{
			static Vector4 One;
			static Vector4 Zero;
			static Vector4 ZeroPosition;
			static Vector4 RightPosition;
			static Vector4 UpPosition;
			static Vector4 ForwardPosition;
			static Vector4 LeftPosition;
			static Vector4 DownPosition;
			static Vector4 BackwardPosition;

			using Float4::Float4;
		};

		struct Plane : public Vector4
		{
			using Vector4::Vector4;

			Plane(_In_ const Vector4& InVector4);
			Plane()
				: Vector4(Vector3::Up, 0.0f)
			{
			}
			float PointProject(_In_ const Vector4& InPoint) const;
		};

		struct Quaternion : public Vector4
		{
			static Quaternion QuaternionIdentity;

			using Vector4::Vector4;
		};

		struct Frustum
		{
			static constexpr uint32_t FrustumPlanesCount = 6;
			static constexpr uint32_t FrustumVerticesCount = 8;

			Plane FrustumPlanes[FrustumPlanesCount];
			Vector4 FrustumVertices[FrustumVerticesCount];

			bool Intersect(_In_ const AxisAlignedBoundingBox& InBoundingBox) const;
		};
	}
}
