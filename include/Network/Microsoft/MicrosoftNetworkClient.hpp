#pragma once

#include "Network/NetworkClient.hpp"

#if ETERNAL_PLATFORM_WINDOWS

#include "Network/Microsoft/MicrosoftNetwork.hpp"

namespace Eternal
{
	namespace NetworkSystem
	{
		class MicrosoftNetworkClient;

		class MicrosoftNetworkClientServerConnectionScope
		{
		public:

			~MicrosoftNetworkClientServerConnectionScope();

			MicrosoftNetworkConnection* operator->();

		private:

			MicrosoftNetworkClientServerConnectionScope(_In_ MicrosoftNetworkConnection* InServerConnection);

			MicrosoftNetworkConnection* _ServerConnection = nullptr;

			friend class MicrosoftNetworkClient;

		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkClient
			: public NetworkClient
			, public MicrosoftNetworkSocket
		{
		public:

			MicrosoftNetworkClient(_In_ const NetworkClientCreateInformation& InNetworkCreateInformation);

			MicrosoftNetworkClientServerConnectionScope ConnectToServer();

		};
	}
}

#endif
