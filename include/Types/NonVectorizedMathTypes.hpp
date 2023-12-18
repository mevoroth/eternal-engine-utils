#pragma once

namespace Eternal
{
	namespace Types
	{
		struct Vector3;
		class AxisAlignedBoundingBox;

		struct Float2
		{
			Float2()
				: Float2(0.0f)
			{
			}
			Float2(_In_ float InScalar)
				: Float2(InScalar, InScalar)
			{
			}
			Float2(_In_ float InX, _In_ float InY)
				: x(InX)
				, y(InY)
			{
			}

			float x = 0.0f;
			float y = 0.0f;
		};

		struct Float3
		{
			Float3()
				: Float3(0.0f)
			{
			}
			Float3(_In_ float InScalar)
				: Float3(InScalar, InScalar, InScalar)
			{
			}
			Float3(_In_ float InX, _In_ float InY, _In_ float InZ)
				: x(InX)
				, y(InY)
				, z(InZ)
			{
			}

			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		};

		struct Float4
		{
			Float4()
				: Float4(0.0f)
			{
			}
			Float4(_In_ float Scalar)
				: Float4(Scalar, Scalar, Scalar, Scalar)
			{
			}
			Float4(_In_ const Float3& InPosition, float InW)
				: Float4(InPosition.x, InPosition.y, InPosition.z, InW)
			{
			}
			Float4(_In_ float InX, _In_ float InY, _In_ float InZ, _In_ float InW)
				: x(InX)
				, y(InY)
				, z(InZ)
				, w(InW)
			{
			}
			Float4(_In_ const float InArray[4])
				: x(InArray[0])
				, y(InArray[1])
				, z(InArray[2])
				, w(InArray[3])
			{
			}

			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float w = 0.0f;
		};

		struct Uint4
		{
			Uint4()
				: Uint4(0u)
			{
			}

			Uint4(_In_ uint32_t InScalar)
				: Uint4(InScalar, InScalar, InScalar, InScalar)
			{
			}

			Uint4(_In_ uint32_t InX, _In_ uint32_t InY, _In_ uint32_t InZ, _In_ uint32_t InW)
				: x(InX)
				, y(InY)
				, z(InZ)
				, w(InW)

			{
			}

			uint32_t x = 0u;
			uint32_t y = 0u;
			uint32_t z = 0u;
			uint32_t w = 0u;
		};

		struct Matrix3x4
		{
			static Matrix3x4 Identity;

			constexpr Matrix3x4(
				_In_ float InM00, _In_ float InM01, _In_ float InM02, _In_ float InM03,
				_In_ float InM10, _In_ float InM11, _In_ float InM12, _In_ float InM13,
				_In_ float InM20, _In_ float InM21, _In_ float InM22, _In_ float InM23
			) noexcept
				: _11(InM00), _12(InM01), _13(InM02), _14(InM03)
				, _21(InM10), _22(InM11), _23(InM12), _24(InM13)
				, _31(InM20), _32(InM21), _33(InM22), _34(InM23)
			{
			}

			constexpr Matrix3x4(
				_In_ float InArray[12]
			)
				: _11(InArray[0]), _12(InArray[1]), _13(InArray[2]), _14(InArray[3])
				, _21(InArray[4]), _22(InArray[5]), _23(InArray[6]), _24(InArray[7])
				, _31(InArray[8]), _32(InArray[9]), _33(InArray[10]), _34(InArray[11])
			{
			}

			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
				};
				float m[3][4];
				float f[12];
			};
		};

		struct Matrix4x4
		{
			static Matrix4x4 Identity;

			constexpr Matrix4x4()
				: _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
				, _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
				, _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
				, _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
			{
			}
			constexpr Matrix4x4(
				_In_ float InM00, _In_ float InM01, _In_ float InM02, _In_ float InM03,
				_In_ float InM10, _In_ float InM11, _In_ float InM12, _In_ float InM13,
				_In_ float InM20, _In_ float InM21, _In_ float InM22, _In_ float InM23,
				_In_ float InM30, _In_ float InM31, _In_ float InM32, _In_ float InM33
			) noexcept
				: _11(InM00), _12(InM01), _13(InM02), _14(InM03)
				, _21(InM10), _22(InM11), _23(InM12), _24(InM13)
				, _31(InM20), _32(InM21), _33(InM22), _34(InM23)
				, _41(InM30), _42(InM31), _43(InM32), _44(InM33)
			{
			}

			constexpr Matrix4x4(_In_ const Matrix4x4& InM)
				: Matrix4x4(
					InM._11, InM._12, InM._13, InM._14,
					InM._21, InM._22, InM._23, InM._24,
					InM._31, InM._32, InM._33, InM._34,
					InM._41, InM._42, InM._43, InM._44
				)
			{
			}

			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};
		};
	}
}