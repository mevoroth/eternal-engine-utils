#pragma once

#include <vector>

namespace Eternal
{
	namespace Event
	{
		using namespace std;

		template<typename EventReceiverType, typename EventReceiverFunctionType>
		struct EventSender
		{
			EventSender(_In_ EventReceiverFunctionType InEventFunctor)
				: EventFunctor(InEventFunctor)
			{
			}

			template<typename... EventReceiverFunctionArgumentsType>
			void Notify(_In_ EventReceiverFunctionArgumentsType&&... InArguments)
			{
				for (uint32_t ReceiverIndex = 0; ReceiverIndex < Receivers.size(); ++ReceiverIndex)
					(Receivers[ReceiverIndex]->*EventFunctor)(forward<EventReceiverFunctionArgumentsType>(InArguments)...);
			}

			vector<EventReceiverType*> Receivers;
			EventReceiverFunctionType EventFunctor;
		};
	}
}
