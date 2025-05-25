#pragma once

#if ETERNAL_USE_DXMATH_TYPES

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

			Float3(_In_ const Float2& InFloat2, _In_ float InZ)
				: XMFLOAT3(InFloat2.x, InFloat2.y, InZ)
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

		struct Matrix3x4 : public XMFLOAT3X4A
		{
			static Matrix3x4 Identity;

			using XMFLOAT3X4A::XMFLOAT3X4A;
		};

		struct Matrix4x4 : public XMFLOAT4X4A
		{
			static Matrix4x4 Identity;

			using XMFLOAT4X4A::XMFLOAT4X4A;
		};
	}
}

#endif
