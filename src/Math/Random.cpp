#include "Math/Random.hpp"
#include <limits>

namespace Eternal
{
	namespace Math
	{
		Random::Random()
		{
		}

		Random::Random(_In_ uint64_t InSeed)
			: _Seed(InSeed)
		{
		}

		float Random::GetNormalized01()
		{
			return static_cast<float>(_Next()) / static_cast<float>(std::numeric_limits<uint32_t>::max());
		}

		Vector2 Random::GetNormalizedVector201()
		{
			return Vector2(
				GetNormalized01(),
				GetNormalized01()
			);
		}

		Vector3 Random::GetNormalizedVector301()
		{
			return Vector3(
				GetNormalized01(),
				GetNormalized01(),
				GetNormalized01()
			);
		}

		uint32_t Random::_Next(_In_ bool InIncrement)
		{
			uint64_t PreviousState = _Seed;
			_Seed = _Seed * 6364136223846793005ULL + (InIncrement ? 1 : 0);
			uint32_t XorShifted = static_cast<uint32_t>(((PreviousState >> 18u) ^ PreviousState) >> 27u);
			uint32_t Rotated = static_cast<uint32_t>(PreviousState >> 59u);
			return (XorShifted >> Rotated) | (XorShifted << ((-Rotated) & 31));
		}
	}
}
