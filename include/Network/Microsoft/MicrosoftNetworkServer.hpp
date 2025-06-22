#pragma once

#include "Network/NetworkServer.hpp"
#include "Container/FreeList.hpp"
#include <WinSock2.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		using namespace Eternal::Container;

		class MicrosoftNetworkServer;

		class MicrosoftNetworkServerClientConnection : public NetworkServerClientConnection
		{
		protected:
		
			MicrosoftNetworkServerClientConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

		};

		class MicrosoftNetworkServerClientConnectionTCP : public MicrosoftNetworkServerClientConnection
		{
		public:

			MicrosoftNetworkServerClientConnectionTCP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

			virtual void Poll() override final;
			virtual void Commit() override final;

			void SetupMicrosoftNetworkServerClientConnectionTCP(_In_ SOCKET InClientSocket);

		private:
			
			SOCKET ClientSocket = INVALID_SOCKET;
		};

		class MicrosoftNetworkServerClientConnectionUDP : public MicrosoftNetworkServerClientConnection
		{
		public:

			MicrosoftNetworkServerClientConnectionUDP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

			virtual void Poll() override final;
			virtual void Commit() override final;

			void SetupMicrosoftNetworkServerClientConnectionUDP(_In_ sockaddr_in&& InClientAddress);

		private:

			sockaddr_in ClientAddress = {};

			friend class MicrosoftNetworkServer;

		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkServerClientConnectionScope
		{
		public:

			~MicrosoftNetworkServerClientConnectionScope();

		private:

			MicrosoftNetworkServerClientConnectionScope(_In_ MicrosoftNetworkServer* InServer, _In_ MicrosoftNetworkServerClientConnection* InClientConnection);

			MicrosoftNetworkServer*					_Server				= nullptr;
			MicrosoftNetworkServerClientConnection*	_ClientConnection	= nullptr;

			friend class MicrosoftNetworkServer;

		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkServer : public NetworkServer
		{
		public:

			static constexpr uint32_t ServerClientConnectionInitialPoolCount = 16u;

			MicrosoftNetworkServer(_In_ const NetworkServerCreateInformation& InNetworkCreateInformation);

			bool BindSocket();
			MicrosoftNetworkServerClientConnectionScope AcceptClientConnection();

		private:

			void ReleaseClientConnection(_In_ MicrosoftNetworkServerClientConnection* InClientConnection);

			addrinfo*											_Address	= nullptr;
			SOCKET												_Socket		= INVALID_SOCKET;
			FreeList<MicrosoftNetworkServerClientConnection*>	_Connections;

			friend class MicrosoftNetworkServerClientConnectionScope;

		};
	}
}
