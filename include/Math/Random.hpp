#pragma once

namespace Eternal
{
	namespace Math
	{
		using namespace Eternal::Types;

		class Random
		{
		public:

			Random();
			Random(_In_ uint64_t InSeed);

			float GetNormalized01();
			Vector3 GetNormalizedVector301();

		private:

			uint32_t _Next(_In_ bool InIncrement = true);

			uint64_t _Seed = 0ull;

		};
	}
}
