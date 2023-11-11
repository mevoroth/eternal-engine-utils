#if !ETERNAL_USE_DXMATH_TYPES

#include "Types/Types.hpp"
#include "Math/Math.hpp"
#include <cmath>

namespace Eternal
{
	namespace Types
	{
		using namespace Eternal::Math;

		namespace TypesPrivate
		{
			union UIntFloat
			{
				float FloatValue;
				uint32_t UIntValue;

				UIntFloat(_In_ uint32_t InUIntValue)
					: UIntValue(InUIntValue)
				{
				}

				UIntFloat(_In_ float InFloatValue)
					: FloatValue(InFloatValue)
				{
				}
			};
		}

		static const float Float0xFFFFFFFF = TypesPrivate::UIntFloat(0xFFFFFFFFu).FloatValue;

		Matrix4x4 operator*(_In_ const Matrix4x4& A, _In_ const Matrix4x4& B)
		{
			ETERNAL_BREAK();
			Matrix4x4 ReturnMatrix = Matrix4x4::Identity;
			//XMStoreFloat4x4A(&ReturnMatrix, XMMatrixMultiply(XMLoadFloat4x4A(&A), XMLoadFloat4x4A(&B)));
			return ReturnMatrix;
		}

		Matrix4x4& operator*=(_In_ Matrix4x4& A, _In_ const _In_ Matrix4x4& B)
		{
			ETERNAL_BREAK();
			//XMStoreFloat4x4(&A, XMMatrixMultiply(XMLoadFloat4x4(&A), XMLoadFloat4x4(&B)));
			return A;
		}

		Quaternion& operator*=(_In_ Quaternion& A, _In_ const Quaternion& B)
		{
			ETERNAL_BREAK();
			//XMStoreFloat4(&A, XMQuaternionMultiply(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return A;
		}

		Vector4 operator+(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return Vector4(
				A.x + B.x,
				A.y + B.y,
				A.x + B.z,
				A.w + B.w
			);
		}
		Vector3 operator+(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x + B.x,
				A.y + B.y,
				A.x + B.z
			);
		}
		Vector2 operator+(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				A.x + B.x,
				A.y + B.y
			);
		}
		Vector4 operator-(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return Vector4(
				A.x - B.x,
				A.y - B.y,
				A.z - B.z,
				A.w - B.w
			);
		}
		Vector3 operator-(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x - B.x,
				A.y - B.y,
				A.z - B.z
			);
		}
		Vector2 operator-(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				A.x - B.x,
				A.y - B.y
			);
		}
		Vector4 operator*(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return Vector4(
				A.x * B.x,
				A.y * B.y,
				A.z * B.z,
				A.w * B.w
			);
		}
		Vector3 operator*(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x * B.x,
				A.y * B.y,
				A.z * B.z
			);
		}
		Vector2 operator*(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				A.x * B.x,
				A.y * B.y
			);
		}
		Vector4& operator+=(_Inout_ Vector4& A, _In_ const Vector4& B)
		{
			A = A + B;
			return A;
		}
		Vector3& operator+=(_Inout_ Vector3& A, _In_ const Vector3& B)
		{
			A = A + B;
			return A;
		}
		Vector2& operator+=(_Inout_ Vector2& A, _In_ const Vector2& B)
		{
			A = A + B;
			return A;
		}
		Vector4& operator-=(_Inout_ Vector4& A, _In_ const Vector4& B)
		{
			A = A - B;
			return A;
		}
		Vector3& operator-=(_Inout_ Vector3& A, _In_ const Vector3& B)
		{
			A = A - B;
			return A;
		}
		Vector2& operator-=(_Inout_ Vector2& A, _In_ const Vector2& B)
		{
			A = A - B;
			return A;
		}
		Vector4& operator*=(_Inout_ Vector4& A, _In_ const Vector4& B)
		{
			A = A * B;
			return A;
		}
		Vector3& operator*=(_Inout_ Vector3& A, _In_ const Vector3& B)
		{
			A = A * B;
			return A;
		}
		Vector2& operator*=(_Inout_ Vector2& A, _In_ const Vector2& B)
		{
			A = A * B;
			return A;
		}
		Vector4& operator/=(_Inout_ Vector4& A, _In_ float B)
		{
			float BRcp = 1.0f / B;
			A *= BRcp;
			return A;
		}
		Vector3& operator/=(_Inout_ Vector3& A, _In_ float B)
		{
			float BRcp = 1.0f / B;
			A *= BRcp;
			return A;
		}
		Vector2& operator/=(_Inout_ Vector2& A, _In_ float B)
		{
			float BRcp = 1.0f / B;
			A *= BRcp;
			return A;
		}
		Vector4 operator*(_In_ const Vector4& A, _In_ float B)
		{
			return Vector4(
				A.x * B,
				A.y * B,
				A.z * B,
				A.w * B
			);
		}
		Vector3 operator*(_In_ const Vector3& A, _In_ float B)
		{
			return Vector3(
				A.x * B,
				A.y * B,
				A.z * B
			);
		}
		Vector2 operator*(_In_ const Vector2& A, _In_ float B)
		{
			return Vector2(
				A.x * B,
				A.y * B
			);
		}
		Vector2 operator/(_In_ const Vector2& A, _In_ float B)
		{
			float BRcp = 1.0f / B;
			return A * BRcp;
		}
		Vector3 operator/(_In_ const Vector3& A, _In_ float B)
		{
			float BRcp = 1.0f / B;
			return A * BRcp;
		}
		Vector4 operator/(_In_ const Vector4& A, _In_ float B)
		{
			float BRcp = 1.0f / B;
			return A * BRcp;
		}
		Vector4 operator*(_In_ const Matrix4x4& A, _In_ const Vector4& B)
		{
			ETERNAL_BREAK();
			return Vector4(
				Dot(Vector4(A.m[0]), B),
				Dot(Vector4(A.m[1]), B),
				Dot(Vector4(A.m[2]), B),
				Dot(Vector4(A.m[3]), B)
			);
		}

		bool operator==(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return A.x == B.x
				&& A.y == B.y
				&& A.z == B.z
				&& A.w == B.w;
		}
		bool operator==(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return A.x == B.x
				&& A.y == B.y
				&& A.z == B.z;
		}
		Vector3 operator<(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x < B.x ? Float0xFFFFFFFF : 0.0f,
				A.y < B.y ? Float0xFFFFFFFF : 0.0f,
				A.z < B.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector3 operator>(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x > B.x ? Float0xFFFFFFFF : 0.0f,
				A.y > B.y ? Float0xFFFFFFFF : 0.0f,
				A.z > B.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector3 operator<=(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x <= B.x ? Float0xFFFFFFFF : 0.0f,
				A.y <= B.y ? Float0xFFFFFFFF : 0.0f,
				A.z <= B.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector3 operator>=(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.x >= B.x ? Float0xFFFFFFFF : 0.0f,
				A.y >= B.y ? Float0xFFFFFFFF : 0.0f,
				A.z >= B.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector2 operator<(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				A.x < B.x ? Float0xFFFFFFFF : 0.0f,
				A.y < B.y ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector2 operator>=(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				A.x >= B.x ? Float0xFFFFFFFF : 0.0f,
				A.y >= B.y ? Float0xFFFFFFFF : 0.0f
			);
		}

		Vector4 operator-(_In_ const Vector4& InVector)
		{
			return Vector4(
				-InVector.x,
				-InVector.y,
				-InVector.z,
				-InVector.w
			);
		}

		Vector3 operator-(_In_ const Vector3& InVector)
		{
			return Vector3(
				-InVector.x,
				-InVector.y,
				-InVector.z
			);
		}

		Vector2 operator-(_In_ const Vector2& InVector)
		{
			return Vector2(
				-InVector.x,
				-InVector.y
			);
		}

		Vector4 Min(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return Vector4(
				std::min(A.x, B.x),
				std::min(A.y, B.y),
				std::min(A.z, B.z),
				std::min(A.w, B.w)
			);
		}
		Vector3 Min(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				std::min(A.x, B.x),
				std::min(A.y, B.y),
				std::min(A.z, B.z)
			);
		}
		Vector2 Min(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				std::min(A.x, B.x),
				std::min(A.y, B.y)
			);
		}
		Vector4 Max(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return Vector4(
				std::max(A.x, B.x),
				std::max(A.y, B.y),
				std::max(A.z, B.z),
				std::max(A.w, B.w)
			);
		}
		Vector3 Max(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				std::max(A.x, B.x),
				std::max(A.y, B.y),
				std::max(A.z, B.z)
			);
		}
		Vector2 Max(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			return Vector2(
				std::max(A.x, B.x),
				std::max(A.y, B.y)
			);
		}
		Vector4 Abs(_In_ const Vector4& A)
		{
			return Vector4(
				std::abs(A.x),
				std::abs(A.y),
				std::abs(A.z),
				std::abs(A.w)
			);
		}
		Vector3 Abs(_In_ const Vector3& A)
		{
			return Vector3(
				std::abs(A.x),
				std::abs(A.y),
				std::abs(A.z)
			);
		}
		Vector2 Abs(_In_ const Vector2& A)
		{
			return Vector2(
				std::abs(A.x),
				std::abs(A.y)
			);
		}

		float Dot(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z;
		}
		float Dot(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
		}
		Vector3 Cross(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return Vector3(
				A.y * B.z - A.z * B.y,
				A.z * B.x - A.x * B.z,
				A.x * B.y - A.y * B.x
			);
		}

		void Transpose(_Inout_ Matrix4x4& A)
		{
			std::swap(A._12, A._21);
			std::swap(A._13, A._31);
			std::swap(A._14, A._41);
			std::swap(A._23, A._32);
			std::swap(A._24, A._42);
			std::swap(A._34, A._43);
		}
		PerspectiveLHMatrix::PerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: Matrix4x4()
		{
			ETERNAL_BREAK();
			//XMStoreFloat4x4A(this, XMMatrixPerspectiveFovLH(InYFOV, InScreenRatio, InNear, InFar));
		}

		inline bool ScalarNearEqual
		(
			_In_ float InS1,
			_In_ float InS2,
			_In_ float InEpsilon
		) noexcept
		{
			float Delta = InS1 - InS2;
			return (fabsf(Delta) <= InEpsilon);
		}

		inline void ScalarSinCos
		(
			float* OutSin,
			float* OutCos,
			float  Value
		) noexcept
		{
			ETERNAL_ASSERT(OutSin);
			ETERNAL_ASSERT(OutCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float Quotient = PI_2_RCP * Value;
			if (Value >= 0.0f)
			{
				Quotient = static_cast<float>(static_cast<int>(Quotient + 0.5f));
			}
			else
			{
				Quotient = static_cast<float>(static_cast<int>(Quotient - 0.5f));
			}
			float Y = Value - PI_2 * Quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float Sign;
			if (Y > PI_DIV_2)
			{
				Y = PI - Y;
				Sign = -1.0f;
			}
			else if (Y < -PI_DIV_2)
			{
				Y = -PI - Y;
				Sign = -1.0f;
			}
			else
			{
				Sign = +1.0f;
			}

			float Y2 = Y * Y;

			// 11-degree minimax approximation
			*OutSin = (((((-2.3889859e-08f * Y2 + 2.7525562e-06f) * Y2 - 0.00019840874f) * Y2 + 0.0083333310f) * Y2 - 0.16666667f) * Y2 + 1.0f) * Y;

			// 10-degree minimax approximation
			float P = ((((-2.6051615e-07f * Y2 + 2.4760495e-05f) * Y2 - 0.0013888378f) * Y2 + 0.041666638f) * Y2 - 0.5f) * Y2 + 1.0f;
			*OutCos = Sign * P;
		}

		inline Matrix4x4 MatrixInversePerspectiveFovLH
		(
			_In_ float InNear,
			_In_ float InFar,
			_In_ float InYFOV,
			_In_ float InScreenRatio
		)
		{
			ETERNAL_ASSERT(InNear > 0.f && InFar > 0.f);
			ETERNAL_ASSERT(!ScalarNearEqual(InYFOV, 0.0f, 0.00001f * 2.0f));
			ETERNAL_ASSERT(!ScalarNearEqual(InScreenRatio, 0.0f, 0.00001f));
			ETERNAL_ASSERT(!ScalarNearEqual(InFar, InNear, 0.00001f));

			float	SinFov;
			float	CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * InYFOV);

			float HeightRcp				= SinFov / CosFov;
			float WidthRcp				= HeightRcp * InScreenRatio;
			float InverseNearByRange	= -(InFar - InNear) / (InNear * InFar);
			float InverseNear			= 1.0f / InNear;

			return Matrix4x4(
				WidthRcp,	0.0f,		0.0f,	0.0f,
				0.0f,		HeightRcp,	0.0f,	0.0f,
				0.0f,		0.0f,		0.0f,	InverseNearByRange,
				0.0f,		0.0f,		1.0f,	InverseNear
			);
		}

		InversePerspectiveLHMatrix::InversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: Matrix4x4()
		{
			*static_cast<Matrix4x4*>(this) = MatrixInversePerspectiveFovLH(InNear, InFar, InYFOV, InScreenRatio);
		}

		ReverseZPerspectiveLHMatrix::ReverseZPerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: PerspectiveLHMatrix(InFar, InNear, InYFOV, InScreenRatio)
		{
		}

		ReverseZInversePerspectiveLHMatrix::ReverseZInversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: InversePerspectiveLHMatrix(InFar, InNear, InYFOV, InScreenRatio)
		{
		}

		inline Matrix4x4 MatrixOrthographicOffCenterLH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		) noexcept
		{
			ETERNAL_ASSERT(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			ETERNAL_ASSERT(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			ETERNAL_ASSERT(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);

			return Matrix4x4(
				ReciprocalWidth + ReciprocalWidth,			0.0f,										0.0f,				0.0f,
				0.0f,										ReciprocalHeight + ReciprocalHeight,		0.0f,				0.0f,
				0.0f, 0.0f,																				fRange,				0.0f,
				-(ViewLeft + ViewRight) * ReciprocalWidth,	-(ViewTop + ViewBottom) * ReciprocalHeight,	-fRange * NearZ,	1.0f
			);
		}

		OrthographicLHMatrix::OrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: Matrix4x4()
		{
			*static_cast<Matrix4x4*>(this) = MatrixOrthographicOffCenterLH(
				0.0f,	InWidth,
				0.0f,	InHeight,
				InNear,	InFar
			);
		}

		static inline Matrix4x4 MatrixInverseOrthographicOffCenterLH
		(
			float InNear,
			float InFar,
			float InWidth,
			float InHeight
		)
		{
			ETERNAL_ASSERT(InWidth >= 0.00001f);
			ETERNAL_ASSERT(InHeight >= 0.00001f);
			ETERNAL_ASSERT(!ScalarNearEqual(InFar, InNear, 0.00001f));

			float HalfWidth			= InWidth * 0.5f;
			float HalfHeight		= InHeight * 0.5f;
			float Range				= (InFar - InNear);
			float NegativeHalfRange	= InNear;
			
			return Matrix4x4(
				HalfWidth,	0.0f,		0.0f,				0.0f,
				0.0f,		HalfHeight,	0.0f,				0.0f,
				0.0f,		0.0f,		Range,				0.0f,
				HalfWidth,	HalfHeight,	NegativeHalfRange,	1.0f
			);
		}

		InverseOrthographicLHMatrix::InverseOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: Matrix4x4()
		{
			*static_cast<Matrix4x4*>(this) = MatrixInverseOrthographicOffCenterLH(
				InNear,		InFar,
				InWidth,	InHeight
			);
		}

		ReverseZOrthographicLHMatrix::ReverseZOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: OrthographicLHMatrix(InFar, InNear, InWidth, InHeight)
		{
		}

		ReverseZInverseOrthographicLHMatrix::ReverseZInverseOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: InverseOrthographicLHMatrix(InFar, InNear, InWidth, InHeight)
		{
		}

		LookToLHMatrix::LookToLHMatrix(_In_ const Vector3& InPosition, _In_ const Vector3& InForward, _In_ const Vector3& InUp)
			: Matrix4x4()
		{
			ETERNAL_BREAK();
			//XMStoreFloat4x4A(this, XMMatrixLookToLH(
			//	XMLoadFloat3(&InPosition),
			//	XMLoadFloat3(&InForward),
			//	XMLoadFloat3(&InUp)
			//));
		}
	}
}

#endif
