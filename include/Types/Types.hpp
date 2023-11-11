#pragma once

#include "UtilsSettings.hpp"
#if ETERNAL_USE_DXMATH_TYPES
#include "DXMathTypes.hpp"
#else
#include "NonVectorizedMathTypes.hpp"
#endif

namespace Eternal
{
	namespace Types
	{
		typedef unsigned long long	u64;
		typedef long long			s64;
		typedef unsigned int		u32;
		typedef int					s32;
		typedef char				s8;
		typedef unsigned char		u8;

		struct Color8888
		{
			uint8_t R = 0;
			uint8_t G = 0;
			uint8_t B = 0;
			uint8_t A = 0;
		};
		union PackedColor
		{
			Color8888 Color;
			uint32_t Packed;

			PackedColor(_In_ uint32_t InPacked)
				: Packed(InPacked)
			{
			}
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

		extern Matrix4x4 operator*(_In_ const Matrix4x4& A, _In_ const Matrix4x4& B);
		extern Matrix4x4& operator*=(_In_ Matrix4x4& A, _In_ const Matrix4x4& B);
		extern Quaternion& operator*=(_In_ Quaternion& A, _In_ const Quaternion& B);
		extern Vector4 operator+(_In_ const Vector4& A, _In_ const Vector4& B);
		extern Vector3 operator+(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector2 operator+(_In_ const Vector2& A, _In_ const Vector2& B);
		extern Vector4 operator-(_In_ const Vector4& A, _In_ const Vector4& B);
		extern Vector3 operator-(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector2 operator-(_In_ const Vector2& A, _In_ const Vector2& B);
		extern Vector4 operator*(_In_ const Vector4& A, _In_ const Vector4& B);
		extern Vector3 operator*(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector2 operator*(_In_ const Vector2& A, _In_ const Vector2& B);
		extern Vector4 operator*(_In_ const Matrix4x4& A, _In_ const Vector4& B);
		extern Vector4& operator+=(_Inout_ Vector4& A, _In_ const Vector4& B);
		extern Vector3& operator+=(_Inout_ Vector3& A, _In_ const Vector3& B);
		extern Vector2& operator+=(_Inout_ Vector2& A, _In_ const Vector2& B);
		extern Vector4& operator-=(_Inout_ Vector4& A, _In_ const Vector4& B);
		extern Vector3& operator-=(_Inout_ Vector3& A, _In_ const Vector3& B);
		extern Vector2& operator-=(_Inout_ Vector2& A, _In_ const Vector2& B);
		extern Vector4& operator*=(_Inout_ Vector4& A, _In_ const Vector4& B);
		extern Vector3& operator*=(_Inout_ Vector3& A, _In_ const Vector3& B);
		extern Vector2& operator*=(_Inout_ Vector2& A, _In_ const Vector2& B);
		extern Vector4& operator/=(_Inout_ Vector4& A, _In_ float B);
		extern Vector3& operator/=(_Inout_ Vector3& A, _In_ float B);
		extern Vector2& operator/=(_Inout_ Vector2& A, _In_ float B);
		extern Vector4 operator*(_In_ const Vector4& A, _In_ float B);
		extern Vector4 operator*(_In_ float A, _In_ const Vector4& B);
		extern Vector3 operator*(_In_ const Vector3& A, _In_ float B);
		extern Vector3 operator*(_In_ float A, _In_ const Vector3& B);
		extern Vector2 operator*(_In_ const Vector2& A, _In_ float B);
		extern Vector2 operator*(_In_ float A, _In_ const Vector2& B);
		extern Vector4 operator/(_In_ const Vector4& A, _In_ float B);
		extern Vector3 operator/(_In_ const Vector3& A, _In_ float B);
		extern Vector2 operator/(_In_ const Vector2& A, _In_ float B);
		extern Vector4 operator-(_In_ const Vector4& InVector);
		extern Vector3 operator-(_In_ const Vector3& InVector);
		extern Vector2 operator-(_In_ const Vector2& InVector);

		extern bool operator==(_In_ const Vector4& A, _In_ const Vector4& B);
		extern bool operator==(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector3 operator<(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector3 operator>(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector3 operator<=(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector3 operator>=(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector2 operator<(_In_ const Vector2& A, _In_ const Vector2& B);
		extern Vector2 operator>=(_In_ const Vector2& A, _In_ const Vector2& B);

		extern Vector4 Min(_In_ const Vector4& A, _In_ const Vector4& B);
		extern Vector3 Min(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector2 Min(_In_ const Vector2& A, _In_ const Vector2& B);
		extern Vector4 Max(_In_ const Vector4& A, _In_ const Vector4& B);
		extern Vector3 Max(_In_ const Vector3& A, _In_ const Vector3& B);
		extern Vector2 Max(_In_ const Vector2& A, _In_ const Vector2& B);
		extern Vector4 Abs(_In_ const Vector4& A);
		extern Vector3 Abs(_In_ const Vector3& A);
		extern Vector2 Abs(_In_ const Vector2& A);

		// MATH STUFF: MOVE THIS
		extern void Transpose(_Inout_ Matrix4x4& A);
		extern Matrix4x4 Inverse(_In_ const Matrix4x4& A);
		extern float SquareLength(_In_ const Vector4& V);
		extern float SquareLength(_In_ const Vector3& V);
		extern float Length(_In_ const Vector4& V);
		extern float Length(_In_ const Vector3& V);
		extern Vector4 Normalize(_In_ const Vector4& V);
		extern Vector3 Normalize(_In_ const Vector3& V);
		extern float Dot(_In_ const Vector3& A, _In_ const Vector3& B);
		extern float Dot(_In_ const Vector4& A, _In_ const Vector4& B);
		extern Vector3 Cross(_In_ const Vector3& A, const Vector3& B);
	}
}
