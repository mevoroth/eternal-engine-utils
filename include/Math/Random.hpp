#pragma once

namespace Eternal
{
	namespace Math
	{
		class Random
		{
		public:

			Random();
			Random(_In_ uint64_t InSeed);

			float GetNormalized01();

		private:

			uint32_t _Next(_In_ bool InIncrement = true);

			uint64_t _Seed = 0ull;

		};
	}
}
