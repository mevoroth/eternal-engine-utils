#include "Parallel/ConditionVariableFactory.hpp"
#include "Parallel/StdConditionVariable.hpp"

namespace Eternal
{
	namespace Parallel
	{
		ConditionVariable* CreateConditionVariable()
		{
			return new StdConditionVariable();
		}

		void DestroyConditionVariable(_Inout_ ConditionVariable*& InOutConditionVariable)
		{
			delete InOutConditionVariable;
			InOutConditionVariable = nullptr;
		}
	}
}
