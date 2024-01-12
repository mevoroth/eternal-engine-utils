#include "Parallel/AtomicGuard.hpp"
#include "Parallel/AtomicS32.hpp"

namespace Eternal
{
	namespace Parallel
	{
		AtomicGuard::AtomicGuard(_In_ AtomicS32* InAtomicGuard)
			: _AtomicGuard(InAtomicGuard)
		{
			ETERNAL_ASSERT(_AtomicGuard->Add() == 0);
		}

		AtomicGuard::~AtomicGuard()
		{
			ETERNAL_ASSERT(_AtomicGuard->Sub() == 1);
		}
	}
}
