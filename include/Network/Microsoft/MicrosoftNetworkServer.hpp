#pragma once

#include "Network/NetworkServer.hpp"
#include "Network/Microsoft/MicrosoftNetwork.hpp"
#include "Container/FreeList.hpp"
#include <WinSock2.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		using namespace Eternal::Container;

		class MicrosoftNetworkServer;

		class MicrosoftNetworkServerClientConnectionScope
		{
		public:

			MicrosoftNetworkServerClientConnectionScope() = default;
			~MicrosoftNetworkServerClientConnectionScope();

			MicrosoftNetworkConnection* operator->();
			operator bool() const;

		private:

			MicrosoftNetworkServerClientConnectionScope(_In_ MicrosoftNetworkServer* InServer, _In_ MicrosoftNetworkConnection* InClientConnection);

			bool IsValid() const;

			MicrosoftNetworkServer*		_Server				= nullptr;
			MicrosoftNetworkConnection*	_ClientConnection	= nullptr;

			friend class MicrosoftNetworkServer;

		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkServer
			: public NetworkServer
			, public MicrosoftNetworkSocket
		{
		public:

			static constexpr uint32_t ServerClientConnectionInitialPoolCount = 16u;

			MicrosoftNetworkServer(_In_ const NetworkServerCreateInformation& InNetworkCreateInformation);

			void BindSocket();
			MicrosoftNetworkServerClientConnectionScope AcceptClientConnection();

		private:

			void ReleaseClientConnection(_In_ MicrosoftNetworkConnection* InClientConnection);

			FreeList<MicrosoftNetworkConnection*>	_Connections;

			friend class MicrosoftNetworkServerClientConnectionScope;

		};
	}
}
