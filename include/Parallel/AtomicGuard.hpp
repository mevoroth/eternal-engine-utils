#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class AtomicS32;

		class AtomicGuard
		{
		public:

			AtomicGuard(_In_ AtomicS32* InAtomic);
			~AtomicGuard();

		private:

			AtomicS32* _AtomicGuard = nullptr;
		};
	}
}
