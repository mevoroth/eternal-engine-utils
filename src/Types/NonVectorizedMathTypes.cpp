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

		Matrix4x4 operator*(_In_ const Matrix4x4& InA, _In_ const Matrix4x4& InB)
		{
			Vector4 Row0(InA._11, InA._12, InA._13, InA._14);
			Vector4 Row1(InA._21, InA._22, InA._23, InA._24);
			Vector4 Row2(InA._31, InA._32, InA._33, InA._34);
			Vector4 Row3(InA._41, InA._42, InA._43, InA._44);

			Vector4 Col0(InB._11, InB._21, InB._31, InB._41);
			Vector4 Col1(InB._12, InB._22, InB._32, InB._42);
			Vector4 Col2(InB._13, InB._23, InB._33, InB._43);
			Vector4 Col3(InB._14, InB._24, InB._34, InB._44);

			return Matrix4x4(
				Dot(Row0, Col0), Dot(Row0, Col1), Dot(Row0, Col2), Dot(Row0, Col3),
				Dot(Row1, Col0), Dot(Row1, Col1), Dot(Row1, Col2), Dot(Row1, Col3),
				Dot(Row2, Col0), Dot(Row2, Col1), Dot(Row2, Col2), Dot(Row2, Col3),
				Dot(Row3, Col0), Dot(Row3, Col1), Dot(Row3, Col2), Dot(Row3, Col3)
			);
		}

		Matrix4x4& operator*=(_In_ Matrix4x4& InA, _In_ const _In_ Matrix4x4& InB)
		{
			InA = InA * InB;
			return InA;
		}

		Quaternion& operator*=(_In_ Quaternion& InA, _In_ const Quaternion& InB)
		{
			InA = InA * InB;
			return InA;
		}

		Quaternion& operator*=(_In_ Quaternion& InA, _In_ const Euler& InB)
		{
			InA *= ToQuaternion(InB);
			return InA;
		}

		Vector4 operator+(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return Vector4(
				InA.x + InB.x,
				InA.y + InB.y,
				InA.z + InB.z,
				InA.w + InB.w
			);
		}
		Vector3 operator+(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x + InB.x,
				InA.y + InB.y,
				InA.z + InB.z
			);
		}
		Vector2 operator+(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				InA.x + InB.x,
				InA.y + InB.y
			);
		}
		Vector4 operator-(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return Vector4(
				InA.x - InB.x,
				InA.y - InB.y,
				InA.z - InB.z,
				InA.w - InB.w
			);
		}
		Vector3 operator-(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x - InB.x,
				InA.y - InB.y,
				InA.z - InB.z
			);
		}
		Vector2 operator-(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				InA.x - InB.x,
				InA.y - InB.y
			);
		}
		Vector4 operator*(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return Vector4(
				InA.x * InB.x,
				InA.y * InB.y,
				InA.z * InB.z,
				InA.w * InB.w
			);
		}
		Vector3 operator*(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x * InB.x,
				InA.y * InB.y,
				InA.z * InB.z
			);
		}
		Vector2 operator*(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				InA.x * InB.x,
				InA.y * InB.y
			);
		}
		Quaternion operator*(_In_ const Quaternion& InA, _In_ const Quaternion& InB)
		{
			return Quaternion(
				(InB.w * InA.x) + (InB.x * InA.w) + (InB.y * InA.z) - (InB.z * InA.y),
				(InB.w * InA.y) - (InB.x * InA.z) + (InB.y * InA.w) + (InB.z * InA.x),
				(InB.w * InA.z) + (InB.x * InA.y) - (InB.y * InA.x) + (InB.z * InA.w),
				(InB.w * InA.w) - (InB.x * InA.x) - (InB.y * InA.y) - (InB.z * InA.z)
			);
		}
		Vector4& operator+=(_Inout_ Vector4& InA, _In_ const Vector4& InB)
		{
			InA = InA + InB;
			return InA;
		}
		Vector3& operator+=(_Inout_ Vector3& InA, _In_ const Vector3& InB)
		{
			InA = InA + InB;
			return InA;
		}
		Vector2& operator+=(_Inout_ Vector2& InA, _In_ const Vector2& InB)
		{
			InA = InA + InB;
			return InA;
		}
		Vector4& operator-=(_Inout_ Vector4& InA, _In_ const Vector4& InB)
		{
			InA = InA - InB;
			return InA;
		}
		Vector3& operator-=(_Inout_ Vector3& InA, _In_ const Vector3& InB)
		{
			InA = InA - InB;
			return InA;
		}
		Vector2& operator-=(_Inout_ Vector2& InA, _In_ const Vector2& InB)
		{
			InA = InA - InB;
			return InA;
		}
		Vector4& operator*=(_Inout_ Vector4& InA, _In_ const Vector4& InB)
		{
			InA = InA * InB;
			return InA;
		}
		Vector3& operator*=(_Inout_ Vector3& InA, _In_ const Vector3& InB)
		{
			InA = InA * InB;
			return InA;
		}
		Vector2& operator*=(_Inout_ Vector2& InA, _In_ const Vector2& InB)
		{
			InA = InA * InB;
			return InA;
		}
		Vector4& operator/=(_Inout_ Vector4& InA, _In_ float InB)
		{
			float BRcp = 1.0f / InB;
			InA *= BRcp;
			return InA;
		}
		Vector3& operator/=(_Inout_ Vector3& InA, _In_ float InB)
		{
			float BRcp = 1.0f / InB;
			InA *= BRcp;
			return InA;
		}
		Vector2& operator/=(_Inout_ Vector2& InA, _In_ float InB)
		{
			float BRcp = 1.0f / InB;
			InA *= BRcp;
			return InA;
		}
		Vector4 operator*(_In_ const Vector4& InA, _In_ float InB)
		{
			return Vector4(
				InA.x * InB,
				InA.y * InB,
				InA.z * InB,
				InA.w * InB
			);
		}
		Vector3 operator*(_In_ const Vector3& InA, _In_ float InB)
		{
			return Vector3(
				InA.x * InB,
				InA.y * InB,
				InA.z * InB
			);
		}
		Vector2 operator*(_In_ const Vector2& InA, _In_ float InB)
		{
			return Vector2(
				InA.x * InB,
				InA.y * InB
			);
		}
		Vector2 operator/(_In_ const Vector2& InA, _In_ float InB)
		{
			float BRcp = 1.0f / InB;
			return InA * BRcp;
		}
		Vector3 operator/(_In_ const Vector3& InA, _In_ float InB)
		{
			float BRcp = 1.0f / InB;
			return InA * BRcp;
		}
		Vector4 operator/(_In_ const Vector4& InA, _In_ float InB)
		{
			float BRcp = 1.0f / InB;
			return InA * BRcp;
		}
		Vector4 operator*(_In_ const Matrix4x4& InA, _In_ const Vector4& InB)
		{
			Vector4 Col0(InA._11, InA._21, InA._31, InA._41);
			Vector4 Col1(InA._12, InA._22, InA._32, InA._42);
			Vector4 Col2(InA._13, InA._23, InA._33, InA._43);
			Vector4 Col3(InA._14, InA._24, InA._34, InA._44);

			return Vector4(
				Dot(Col0, InB),
				Dot(Col1, InB),
				Dot(Col2, InB),
				Dot(Col3, InB)
			);
		}

		bool operator==(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return InA.x == InB.x
				&& InA.y == InB.y
				&& InA.z == InB.z
				&& InA.w == InB.w;
		}
		bool operator==(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return InA.x == InB.x
				&& InA.y == InB.y
				&& InA.z == InB.z;
		}
		bool operator!=(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return !(InA == InB);
		}
		Vector3 operator<(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x < InB.x ? Float0xFFFFFFFF : 0.0f,
				InA.y < InB.y ? Float0xFFFFFFFF : 0.0f,
				InA.z < InB.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector3 operator>(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x > InB.x ? Float0xFFFFFFFF : 0.0f,
				InA.y > InB.y ? Float0xFFFFFFFF : 0.0f,
				InA.z > InB.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector3 operator<=(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x <= InB.x ? Float0xFFFFFFFF : 0.0f,
				InA.y <= InB.y ? Float0xFFFFFFFF : 0.0f,
				InA.z <= InB.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector3 operator>=(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.x >= InB.x ? Float0xFFFFFFFF : 0.0f,
				InA.y >= InB.y ? Float0xFFFFFFFF : 0.0f,
				InA.z >= InB.z ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector2 operator<(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				InA.x < InB.x ? Float0xFFFFFFFF : 0.0f,
				InA.y < InB.y ? Float0xFFFFFFFF : 0.0f
			);
		}
		Vector2 operator>=(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				InA.x >= InB.x ? Float0xFFFFFFFF : 0.0f,
				InA.y >= InB.y ? Float0xFFFFFFFF : 0.0f
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

		Vector4 Min(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return Vector4(
				std::min(InA.x, InB.x),
				std::min(InA.y, InB.y),
				std::min(InA.z, InB.z),
				std::min(InA.w, InB.w)
			);
		}
		Vector3 Min(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				std::min(InA.x, InB.x),
				std::min(InA.y, InB.y),
				std::min(InA.z, InB.z)
			);
		}
		Vector2 Min(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				std::min(InA.x, InB.x),
				std::min(InA.y, InB.y)
			);
		}
		Vector4 Max(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return Vector4(
				std::max(InA.x, InB.x),
				std::max(InA.y, InB.y),
				std::max(InA.z, InB.z),
				std::max(InA.w, InB.w)
			);
		}
		Vector3 Max(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				std::max(InA.x, InB.x),
				std::max(InA.y, InB.y),
				std::max(InA.z, InB.z)
			);
		}
		Vector2 Max(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return Vector2(
				std::max(InA.x, InB.x),
				std::max(InA.y, InB.y)
			);
		}
		Vector4 Abs(_In_ const Vector4& InA)
		{
			return Vector4(
				std::abs(InA.x),
				std::abs(InA.y),
				std::abs(InA.z),
				std::abs(InA.w)
			);
		}
		Vector3 Abs(_In_ const Vector3& InA)
		{
			return Vector3(
				std::abs(InA.x),
				std::abs(InA.y),
				std::abs(InA.z)
			);
		}
		Vector2 Abs(_In_ const Vector2& InA)
		{
			return Vector2(
				std::abs(InA.x),
				std::abs(InA.y)
			);
		}

		static inline bool IsInfinite(float InF)
		{
			return ((*(const uint32_t*)&(InF) & 0x7FFFFFFF) == 0x7F800000);
		}

		bool IsInfinite(_In_ const Vector3& InA)
		{
			return IsInfinite(InA.x)
				|| IsInfinite(InA.y)
				|| IsInfinite(InA.z);
		}

		float Dot(_In_ const Vector2& InA, _In_ const Vector2& InB)
		{
			return InA.x * InB.x + InA.y * InB.y;
		}
		float Dot(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return InA.x * InB.x + InA.y * InB.y + InA.z * InB.z;
		}
		float Dot(_In_ const Vector4& InA, _In_ const Vector4& InB)
		{
			return InA.x * InB.x + InA.y * InB.y + InA.z * InB.z + InA.w * InB.w;
		}
		Vector3 Cross(_In_ const Vector3& InA, _In_ const Vector3& InB)
		{
			return Vector3(
				InA.y * InB.z - InA.z * InB.y,
				InA.z * InB.x - InA.x * InB.z,
				InA.x * InB.y - InA.y * InB.x
			);
		}

		void Transpose(_Inout_ Matrix4x4& InA)
		{
			std::swap(InA._12, InA._21);
			std::swap(InA._13, InA._31);
			std::swap(InA._14, InA._41);
			std::swap(InA._23, InA._32);
			std::swap(InA._24, InA._42);
			std::swap(InA._34, InA._43);
		}

		Quaternion ToQuaternion(_In_ const Euler& InR)
		{
			const float Pitch	= InR.x;
			const float Yaw		= InR.y;
			const float Roll	= InR.z;

			const float HalfPitch = Pitch * 0.5f;
			const float CosHalfPitch = cosf(HalfPitch);
			const float SinHalfPitch = sinf(HalfPitch);

			const float HalfYaw = Yaw * 0.5f;
			const float CosHalfYaw = cosf(HalfYaw);
			const float SinHalfYaw = sinf(HalfYaw);

			const float HalfRoll = Roll * 0.5f;
			const float CosHalfRoll = cosf(HalfRoll);
			const float SinHalfRoll = sinf(HalfRoll);

			return Quaternion(
				CosHalfRoll * SinHalfPitch * CosHalfYaw + SinHalfRoll * CosHalfPitch * SinHalfYaw,
				CosHalfRoll * CosHalfPitch * SinHalfYaw - SinHalfRoll * SinHalfPitch * CosHalfYaw,
				SinHalfRoll * CosHalfPitch * CosHalfYaw - CosHalfRoll * SinHalfPitch * SinHalfYaw,
				CosHalfRoll * CosHalfPitch * CosHalfYaw + SinHalfRoll * SinHalfPitch * SinHalfYaw
			);
		}

		//////////////////////////////////////////////////////////////////////////

		TranslationMatrix4x4::TranslationMatrix4x4(_In_ const Vector3& InTranslation)
			: TranslationMatrix4x4(InTranslation.x, InTranslation.y, InTranslation.z)
		{
		}

		TranslationMatrix4x4::TranslationMatrix4x4(_In_ float InX, _In_ float InY, _In_ float InZ)
			: Matrix4x4(
				1.0f,	0.0f,	0.0f,	0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				InX,	InY,	InZ,	1.0f
			)
		{
		}

		RotationMatrix4x4::RotationMatrix4x4(_In_ const Quaternion& InQuaternion)
			: Matrix4x4()
		{
			float RX = InQuaternion.x;
			float RXSquared = RX * RX;

			float RY = InQuaternion.y;
			float RYSquared = RY * RY;

			float RZ = InQuaternion.z;
			float RZSquared = RZ * RZ;

			float RW = InQuaternion.w;

			m[0][0] = 1.0f - 2.0f * RYSquared	- 2.0f * RZSquared;
			m[0][1] = 2.0f * RX * RY			+ 2.0f * RZ * RW;
			m[0][2] = 2.0f * RX * RZ			- 2.0f * RY * RW;
			m[0][3] = 0.0f;

			m[1][0] = 2.0f * RX * RY			- 2.0f * RZ * RW;
			m[1][1] = 1.0f						- 2.0f * RXSquared	- 2.0f * RZSquared;
			m[1][2] = 2.0f * RY * RZ			+ 2.0f * RX * RW;
			m[1][3] = 0.0f;

			m[2][0] = 2.0f * RX * RZ			+ 2.0f * RY * RW;
			m[2][1] = 2.0f * RY * RZ			- 2.0f * RX * RW;
			m[2][2] = 1.0f						- 2.0f * RXSquared	- 2.0f * RYSquared;
			m[2][3] = 0.0f;

			m[3][0] = 0.0f;
			m[3][1] = 0.0f;
			m[3][2] = 0.0f;
			m[3][3] = 1.0f;
		}

		ScaleMatrix4x4::ScaleMatrix4x4(_In_ float InUniformScale)
			: ScaleMatrix4x4(InUniformScale, InUniformScale, InUniformScale)
		{
		}
		
		ScaleMatrix4x4::ScaleMatrix4x4(_In_ const Vector3& InScale)
			: ScaleMatrix4x4(InScale.x, InScale.y, InScale.z)
		{
		}

		ScaleMatrix4x4::ScaleMatrix4x4(_In_ float InScaleX, _In_ float InScaleY, _In_ float InScaleZ)
			: Matrix4x4(
				InScaleX,	0.0f,		0.0f,		0.0f,
				0.0f,		InScaleY,	0.0f,		0.0f,
				0.0f,		0.0f,		InScaleZ,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			)
		{
		}

		static inline bool ScalarNearEqual
		(
			_In_ float InS1,
			_In_ float InS2,
			_In_ float InEpsilon
		) noexcept
		{
			float Delta = InS1 - InS2;
			return (fabsf(Delta) <= InEpsilon);
		}

		static inline void ScalarSinCos
		(
			_Out_ float* OutSin,
			_Out_ float* OutCos,
			_In_  float  InValue
		) noexcept
		{
			ETERNAL_ASSERT(OutSin);
			ETERNAL_ASSERT(OutCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float Quotient = PI_2_RCP * InValue;
			if (InValue >= 0.0f)
			{
				Quotient = static_cast<float>(static_cast<int>(Quotient + 0.5f));
			}
			else
			{
				Quotient = static_cast<float>(static_cast<int>(Quotient - 0.5f));
			}
			float Y = InValue - PI_2 * Quotient;

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

		static inline Matrix4x4 MatrixPerspectiveFovLH(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
		{
			ETERNAL_ASSERT(InNear > 0.f && InFar > 0.f);
			ETERNAL_ASSERT(!ScalarNearEqual(InYFOV, 0.0f, 0.00001f * 2.0f));
			ETERNAL_ASSERT(!ScalarNearEqual(InScreenRatio, 0.0f, 0.00001f));
			ETERNAL_ASSERT(!ScalarNearEqual(InFar, InNear, 0.00001f));

			float SinFov;
			float CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * InYFOV);

			float Height = CosFov / SinFov;
			float Width = Height / InScreenRatio;
			float RangeRatio = InFar / (InFar - InNear);

			return Matrix4x4(
				Width,	0.0f,	0.0f,					0.0f,
				0.0f,	Height,	0.0f,					0.0f,
				0.0f,	0.0f,	RangeRatio,				1.0f,
				0.0f,	0.0f,	-RangeRatio * InNear,	0.0f
			);
		}

		PerspectiveLHMatrix::PerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: Matrix4x4(MatrixPerspectiveFovLH(InNear, InFar, InYFOV, InScreenRatio))
		{
		}

		static inline Matrix4x4 MatrixInversePerspectiveFovLH
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
			: Matrix4x4(MatrixInversePerspectiveFovLH(InNear, InFar, InYFOV, InScreenRatio))
		{
		}

		ReverseZPerspectiveLHMatrix::ReverseZPerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: PerspectiveLHMatrix(InFar, InNear, InYFOV, InScreenRatio)
		{
		}

		ReverseZInversePerspectiveLHMatrix::ReverseZInversePerspectiveLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InYFOV, _In_ float InScreenRatio)
			: InversePerspectiveLHMatrix(InFar, InNear, InYFOV, InScreenRatio)
		{
		}

		static inline Matrix4x4 MatrixOrthographicOffCenterLH
		(
			_In_ float ViewLeft,
			_In_ float ViewRight,
			_In_ float ViewBottom,
			_In_ float ViewTop,
			_In_ float NearZ,
			_In_ float FarZ
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
			: Matrix4x4(MatrixOrthographicOffCenterLH(
				0.0f,	InWidth,
				0.0f,	InHeight,
				InNear,	InFar
			))
		{
		}

		static inline Matrix4x4 MatrixInverseOrthographicOffCenterLH
		(
			_In_ float InNear,
			_In_ float InFar,
			_In_ float InWidth,
			_In_ float InHeight
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
			: Matrix4x4(MatrixInverseOrthographicOffCenterLH(
				InNear,		InFar,
				InWidth,	InHeight
			))
		{
		}

		ReverseZOrthographicLHMatrix::ReverseZOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: OrthographicLHMatrix(InFar, InNear, InWidth, InHeight)
		{
		}

		ReverseZInverseOrthographicLHMatrix::ReverseZInverseOrthographicLHMatrix(_In_ float InNear, _In_ float InFar, _In_ float InWidth, _In_ float InHeight)
			: InverseOrthographicLHMatrix(InFar, InNear, InWidth, InHeight)
		{
		}

		static inline Matrix4x4 MatrixLookToLH(_In_ const Vector3& InPosition, _In_ const Vector3& InForward, _In_ const Vector3& InUp)
		{
			ETERNAL_ASSERT(InForward != Vector3::Zero);
			ETERNAL_ASSERT(!IsInfinite(InForward));
			ETERNAL_ASSERT(InUp != Vector3::Zero);
			ETERNAL_ASSERT(!IsInfinite(InUp));

			Vector3 NormalizedForward = Normalize(InForward);

			Vector3 NormalizedRight = Cross(InUp, NormalizedForward);
			NormalizedRight = Normalize(NormalizedRight);

			Vector3 NormalizedUp = Cross(NormalizedForward, NormalizedRight);

			Vector3 NegativePosition = -InPosition;

			float RightDotNegativePosition		= Dot(NormalizedRight, NegativePosition);
			float UpDotNegativePosition			= Dot(NormalizedUp, NegativePosition);
			float ForwardDotNegativePosition	= Dot(NormalizedForward, NegativePosition);

			return Matrix4x4(
				NormalizedRight.x,			NormalizedUp.x,				NormalizedForward.x,		0.0f,
				NormalizedRight.y,			NormalizedUp.y,				NormalizedForward.y,		0.0f,
				NormalizedRight.z,			NormalizedUp.z,				NormalizedForward.z,		0.0f,
				RightDotNegativePosition,	UpDotNegativePosition,		ForwardDotNegativePosition,	1.0f
			);
		}

		LookToLHMatrix::LookToLHMatrix(_In_ const Vector3& InPosition, _In_ const Vector3& InForward, _In_ const Vector3& InUp)
			: Matrix4x4(MatrixLookToLH(InPosition, InForward, InUp))
		{
		}
	}
}

#endif
