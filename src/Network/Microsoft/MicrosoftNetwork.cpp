#include "Network/Microsoft/MicrosoftNetwork.hpp"
#include <WinSock2.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		void InitializeNetworkSystem()
		{
			WSADATA WSAData = {};

			int Result = WSAStartup(MAKEWORD(2, 2), &WSAData);
			ETERNAL_ASSERT(Result == 0);
		}
	}
}
