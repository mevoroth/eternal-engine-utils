#if ETERNAL_USE_DXMATH_TYPES

#include "Types/Types.hpp"

#include <cmath>

namespace Eternal
{
	namespace Types
	{
		Matrix4x4 operator*(_In_ const Matrix4x4& A, _In_ const Matrix4x4& B)
		{
			Matrix4x4 ReturnMatrix;
			XMStoreFloat4x4A(&ReturnMatrix, XMMatrixMultiply(XMLoadFloat4x4A(&A), XMLoadFloat4x4A(&B)));
			return ReturnMatrix;
		}

		Matrix4x4& operator*=(_In_ Matrix4x4& A, _In_ const _In_ Matrix4x4& B)
		{
			XMStoreFloat4x4(&A, XMMatrixMultiply(XMLoadFloat4x4(&A), XMLoadFloat4x4(&B)));
			return A;
		}

		Quaternion& operator*=(_In_ Quaternion& A, _In_ const Quaternion& B)
		{
			XMStoreFloat4(&A, XMQuaternionMultiply(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return A;
		}

		Vector4 operator+(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorAdd(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return Result;
		}
		Vector3 operator+(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorAdd(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector2 operator+(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorAdd(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}
		Vector4 operator-(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorSubtract(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return Result;
		}
		Vector3 operator-(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorSubtract(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector2 operator-(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorSubtract(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}
		Vector4 operator*(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorMultiply(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return Result;
		}
		Vector3 operator*(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorMultiply(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector2 operator*(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorMultiply(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}
		Vector4& operator+=(_Inout_ Vector4& A, _In_ const Vector4& B)
		{
			XMStoreFloat4A(&A, XMVectorAdd(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return A;
		}
		Vector3& operator+=(_Inout_ Vector3& A, _In_ const Vector3& B)
		{
			XMStoreFloat3(&A, XMVectorAdd(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return A;
		}
		Vector2& operator+=(_Inout_ Vector2& A, _In_ const Vector2& B)
		{
			XMStoreFloat2(&A, XMVectorAdd(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return A;
		}
		Vector4& operator-=(_Inout_ Vector4& A, _In_ const Vector4& B)
		{
			XMStoreFloat4A(&A, XMVectorSubtract(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return A;
		}
		Vector3& operator-=(_Inout_ Vector3& A, _In_ const Vector3& B)
		{
			XMStoreFloat3(&A, XMVectorSubtract(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return A;
		}
		Vector2& operator-=(_Inout_ Vector2& A, _In_ const Vector2& B)
		{
			XMStoreFloat2(&A, XMVectorSubtract(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return A;
		}
		Vector4& operator*=(_Inout_ Vector4& A, _In_ const Vector4& B)
		{
			XMStoreFloat4A(&A, XMVectorMultiply(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return A;
		}
		Vector3& operator*=(_Inout_ Vector3& A, _In_ const Vector3& B)
		{
			XMStoreFloat3(&A, XMVectorMultiply(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return A;
		}
		Vector2& operator*=(_Inout_ Vector2& A, _In_ const Vector2& B)
		{
			XMStoreFloat2(&A, XMVectorMultiply(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return A;
		}
		Vector4& operator/=(_Inout_ Vector4& A, _In_ float B)
		{
			XMStoreFloat4A(&A, XMVectorDivide(XMLoadFloat4A(&A), XMVectorReplicate(B)));
			return A;
		}
		Vector3& operator/=(_Inout_ Vector3& A, _In_ float B)
		{
			XMStoreFloat3(&A, XMVectorDivide(XMLoadFloat3(&A), XMVectorReplicate(B)));
			return A;
		}
		Vector2& operator/=(_Inout_ Vector2& A, _In_ float B)
		{
			XMStoreFloat2(&A, XMVectorDivide(XMLoadFloat2(&A), XMVectorReplicate(B)));
			return A;
		}
		Vector4 operator*(_In_ const Vector4& A, _In_ float B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorMultiply(XMLoadFloat4A(&A), XMVectorReplicate(B)));
			return Result;
		}
		Vector3 operator*(_In_ const Vector3& A, _In_ float B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorMultiply(XMLoadFloat3(&A), XMVectorReplicate(B)));
			return Result;
		}
		Vector2 operator*(_In_ const Vector2& A, _In_ float B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorMultiply(XMLoadFloat2(&A), XMVectorReplicate(B)));
			return Result;
		}
		Vector2 operator/(_In_ const Vector2& A, _In_ float B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorDivide(XMLoadFloat2(&A), XMVectorReplicate(B)));
			return Result;
		}
		Vector3 operator/(_In_ const Vector3& A, _In_ float B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorDivide(XMLoadFloat3(&A), XMVectorReplicate(B)));
			return Result;
		}
		Vector4 operator/(_In_ const Vector4& A, _In_ float B)
		{
			Vector4 Result;
			XMStoreFloat4(&Result, XMVectorDivide(XMLoadFloat4A(&A), XMVectorReplicate(B)));
			return Result;
		}
		Vector4 operator*(_In_ const Matrix4x4& A, _In_ const Vector4& B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVector4Transform(
				XMLoadFloat4A(&B),
				XMLoadFloat4x4A(&A)
			));
			return Result;
		}

		bool operator==(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			uint32_t ComparisonResult;
			XMVectorEqualR(&ComparisonResult, XMLoadFloat4A(&A), XMLoadFloat4A(&B));
			return XMComparisonAllTrue(ComparisonResult);
		}
		bool operator==(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			uint32_t ComparisonResult;
			XMVectorEqualR(&ComparisonResult, XMLoadFloat3(&A), XMLoadFloat3(&B));
			return XMComparisonAllTrue(ComparisonResult);
		}
		Vector3 operator<(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorLess(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector3 operator>(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorGreater(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector3 operator<=(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorLessOrEqual(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector3 operator>=(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorGreaterOrEqual(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector2 operator<(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorLess(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}
		Vector2 operator>=(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorGreaterOrEqual(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}

		Vector4 operator-(_In_ const Vector4& InVector)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorNegate(XMLoadFloat4A(&InVector)));
			return Result;
		}

		Vector3 operator-(_In_ const Vector3& InVector)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorNegate(XMLoadFloat3(&InVector)));
			return Result;
		}

		Vector2 operator-(_In_ const Vector2& InVector)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorNegate(XMLoadFloat2(&InVector)));
			return Result;
		}

		Vector4 Min(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorMin(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return Result;
		}
		Vector3 Min(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorMin(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector2 Min(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorMin(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}
		Vector4 Max(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorMax(XMLoadFloat4A(&A), XMLoadFloat4A(&B)));
			return Result;
		}
		Vector3 Max(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorMax(XMLoadFloat3(&A), XMLoadFloat3(&B)));
			return Result;
		}
		Vector2 Max(_In_ const Vector2& A, _In_ const Vector2& B)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorMin(XMLoadFloat2(&A), XMLoadFloat2(&B)));
			return Result;
		}
		Vector4 Abs(_In_ const Vector4& A)
		{
			Vector4 Result;
			XMStoreFloat4A(&Result, XMVectorAbs(XMLoadFloat4A(&A)));
			return Result;
		}
		Vector3 Abs(_In_ const Vector3& A)
		{
			Vector3 Result;
			XMStoreFloat3(&Result, XMVectorAbs(XMLoadFloat3(&A)));
			return Result;
		}
		Vector2 Abs(_In_ const Vector2& A)
		{
			Vector2 Result;
			XMStoreFloat2(&Result, XMVectorAbs(XMLoadFloat2(&A)));
			return Result;
		}

		float Dot(_In_ const Vector3& A, _In_ const Vector3& B)
		{
			return A.x*B.x + A.y*B.y + A.z*B.z;
		}
		float Dot(_In_ const Vector4& A, _In_ const Vector4& B)
		{
			return A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
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
			XMStoreFloat4x4(&A, XMMatrixTranspose(XMLoadFloat4x4(&A)));
		}
		Matrix4x4 Inverse(_In_ const Matrix4x4& A)
		{
			Matrix4x4 Result;
			XMStoreFloat4x4(&Result, XMMatrixInverse(nullptr, XMLoadFloat4x4(&A)));
			return Result;
		}

		PerspectiveLHMatrix::PerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: Matrix4x4()
		{
			XMStoreFloat4x4A(this, XMMatrixPerspectiveFovLH(InYFOV, InScreenRatio, InNear, InFar));
		}

		inline XMMATRIX XM_CALLCONV XMMatrixInversePerspectiveFovLH
		(
			_In_ float InNear,
			_In_ float InFar,
			_In_ float InYFOV,
			_In_ float InScreenRatio
		)
		{
			ETERNAL_ASSERT(InNear > 0.f && InFar > 0.f);
			ETERNAL_ASSERT(!XMScalarNearEqual(InYFOV, 0.0f, 0.00001f * 2.0f));
			ETERNAL_ASSERT(!XMScalarNearEqual(InScreenRatio, 0.0f, 0.00001f));
			ETERNAL_ASSERT(!XMScalarNearEqual(InFar, InNear, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)
			float	SinFov;
			float	CosFov;
			XMScalarSinCos(&SinFov, &CosFov, 0.5f * InYFOV);

			float HeightRcp				= SinFov / CosFov;
			float WidthRcp				= HeightRcp * InScreenRatio;
			float InverseNearByRange	= -(InFar - InNear) / (InNear * InFar);
			float InverseNear			= 1.0f / InNear;

			XMMATRIX M(
				WidthRcp,	0.0f,		0.0f,	0.0f,
				0.0f,		HeightRcp,	0.0f,	0.0f,
				0.0f,		0.0f,		0.0f,	InverseNearByRange,
				0.0f,		0.0f,		1.0f,	InverseNear
			);
			return M;
#elif defined(_XM_SSE_INTRINSICS_)
			float	SinFov;
			float	CosFov;
			XMScalarSinCos(&SinFov, &CosFov, 0.5f * InYFOV);

			float HeightRcp				= SinFov / CosFov;
			float InverseNearByRange	= -(InFar - InNear) / (InNear * InFar);
			// On stack
			XMVECTOR rMem = {
				HeightRcp * InScreenRatio,
				HeightRcp,
				InverseNearByRange,
				1.0f / InNear
			};
			// Copy from memory to SSE register
			XMVECTOR vValues = rMem;
			// Copy x only
			XMMATRIX M;

			XMVECTOR vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.r[0] = vTemp;
			
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;
			
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskZ);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(2, 0, 0, 0));
			M.r[2] = vTemp;

			vTemp = _mm_and_ps(rMem, g_XMMaskW);
			vTemp = _mm_or_ps(vTemp, g_XMIdentityR2);
			M.r[3] = vTemp;

			return M;
#endif
		}

		InversePerspectiveLHMatrix::InversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: Matrix4x4()
		{
			XMStoreFloat4x4A(this, XMMatrixInversePerspectiveFovLH(InNear, InFar, InYFOV, InScreenRatio));
		}

		ReverseZPerspectiveLHMatrix::ReverseZPerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: PerspectiveLHMatrix(InFar, InNear, InYFOV, InScreenRatio)
		{
		}

		ReverseZInversePerspectiveLHMatrix::ReverseZInversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: InversePerspectiveLHMatrix(InFar, InNear, InYFOV, InScreenRatio)
		{
		}

		OrthographicLHMatrix::OrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: Matrix4x4()
		{
			XMStoreFloat4x4A(this, XMMatrixOrthographicOffCenterLH(
				0.0f, InWidth,
				0.0f, InHeight,
				InNear, InFar
			));
		}

		static inline XMMATRIX XM_CALLCONV XMMatrixInverseOrthographicOffCenterLH
		(
			float InNear,
			float InFar,
			float InWidth,
			float InHeight
		)
		{
			ETERNAL_ASSERT(InWidth >= 0.00001f);
			ETERNAL_ASSERT(InHeight >= 0.00001f);
			ETERNAL_ASSERT(!XMScalarNearEqual(InFar, InNear, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

			float HalfWidth			= InWidth * 0.5f;
			float HalfHeight		= InHeight * 0.5f;
			float Range				= (InFar - InNear);
			float NegativeHalfRange	= InNear;

			XMMATRIX M(
				HalfWidth,	0.0f,		0.0f,				0.0f,
				0.0f,		HalfHeight,	0.0f,				0.0f,
				0.0f,		0.0f,		Range,				0.0f,
				HalfWidth,	HalfHeight,	NegativeHalfRange,	1.0f
			);
			return M;

#elif defined(_XM_SSE_INTRINSICS_)
			XMMATRIX M;

			// Note: This is recorded on the stack
			XMVECTOR rMem = {
				InWidth * 0.5f,
				InHeight * 0.5f,
				InNear,
				1.0f
			};
			XMVECTOR rMem2 = {
				0.0f,
				0.0f,
				InFar - InNear,
				0.0f
			};

			// Copy from memory to SSE register
			XMVECTOR vValues = rMem;
			XMVECTOR vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			M.r[0] = vTemp;

			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_XMMaskY);
			M.r[1] = vTemp;

			M.r[2] = rMem2;

			M.r[3] = rMem;

			return M;
#endif
		}

		InverseOrthographicLHMatrix::InverseOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: Matrix4x4()
		{
			XMStoreFloat4x4A(this, XMMatrixInverseOrthographicOffCenterLH(
				InNear, InFar,
				InWidth, InHeight
			));
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
			XMStoreFloat4x4A(this, XMMatrixLookToLH(
				XMLoadFloat3(&InPosition),
				XMLoadFloat3(&InForward),
				XMLoadFloat3(&InUp)
			));
		}
	}
}

#endif
