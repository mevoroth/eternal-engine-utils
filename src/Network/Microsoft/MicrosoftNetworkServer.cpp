#include "Network/Microsoft/MicrosoftNetworkServer.hpp"
#include <WS2tcpip.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkServerClientConnectionScope::MicrosoftNetworkServerClientConnectionScope(_In_ MicrosoftNetworkServer* InServer, _In_ MicrosoftNetworkConnection* InClientConnection)
			: _Server(InServer)
			, _ClientConnection(InClientConnection)
		{
		}

		MicrosoftNetworkServerClientConnectionScope::~MicrosoftNetworkServerClientConnectionScope()
		{
			if (IsValid())
				_Server->ReleaseClientConnection(_ClientConnection);
		}

		MicrosoftNetworkConnection* MicrosoftNetworkServerClientConnectionScope::operator->()
		{
			return _ClientConnection;
		}

		MicrosoftNetworkServerClientConnectionScope::operator bool() const
		{
			return IsValid();
		}

		bool MicrosoftNetworkServerClientConnectionScope::IsValid() const
		{
			return _ClientConnection;
		}

		//////////////////////////////////////////////////////////////////////////

		constexpr uint32_t MicrosoftNetworkServer::ServerClientConnectionInitialPoolCount;

		MicrosoftNetworkServer::MicrosoftNetworkServer(_In_ const NetworkServerCreateInformation& InNetworkCreateInformation)
			: NetworkServer(InNetworkCreateInformation)
			, _Connections(
				[this](void) -> MicrosoftNetworkConnection*
				{
					MicrosoftNetworkConnection* NewMicrosoftNetworkServerClientConnection = nullptr;

					if (_NetworkCreateInformation.TransportLayer == NetworkTransportLayer::TRANSPORT_TCP)
						NewMicrosoftNetworkServerClientConnection = new MicrosoftNetworkConnectionTCP(8192u, 16384u);

					if (_NetworkCreateInformation.TransportLayer == NetworkTransportLayer::TRANSPORT_UDP)
						NewMicrosoftNetworkServerClientConnection = new MicrosoftNetworkConnectionUDP(8192u, 16384u);

					ETERNAL_ASSERT(NewMicrosoftNetworkServerClientConnection);
					return NewMicrosoftNetworkServerClientConnection;
				},
				ServerClientConnectionInitialPoolCount
			)
		{
		}

		bool MicrosoftNetworkServer::BindSocket()
		{
			CreateSocket(_NetworkCreateInformation);

			int Result = ::bind(_Socket, _Address->ai_addr, static_cast<int>(_Address->ai_addrlen));
			ETERNAL_ASSERT(Result != SOCKET_ERROR);

			if (_NetworkCreateInformation.TransportLayer == NetworkTransportLayer::TRANSPORT_TCP)
			{
				Result = listen(_Socket, SOMAXCONN);
				ETERNAL_ASSERT(Result != SOCKET_ERROR);
			}

			return true;
		}

		MicrosoftNetworkServerClientConnectionScope MicrosoftNetworkServer::AcceptClientConnection()
		{
			MicrosoftNetworkConnection* NewServerClientConnection = nullptr;

			switch (_NetworkCreateInformation.TransportLayer)
			{
			case NetworkTransportLayer::TRANSPORT_TCP:
			{
				SOCKET ClientSocket = accept(_Socket, nullptr, nullptr);
				if (ClientSocket == INVALID_SOCKET)
				{
					ETERNAL_BREAK();
					break;
				}

				NewServerClientConnection = _Connections.Pop();
				static_cast<MicrosoftNetworkConnectionTCP*>(NewServerClientConnection)->SetupMicrosoftNetworkConnectionTCP(ClientSocket);
			} break;
			case NetworkTransportLayer::TRANSPORT_UDP:
			{
				NewServerClientConnection = _Connections.Pop();

				char MagicKeyBuffer[1024] = {};
				sockaddr_in ClientAddress = {};
				int ClientAddressLength = 0;
				int ReceivedBytes = recvfrom(_Socket, MagicKeyBuffer, ETERNAL_ARRAYSIZE(MagicKeyBuffer), 0,
					(sockaddr*)&ClientAddress, &ClientAddressLength);
				ETERNAL_ASSERT(ReceivedBytes != SOCKET_ERROR);
				static_cast<MicrosoftNetworkConnectionUDP*>(NewServerClientConnection)->SetupMicrosoftNetworkConnectionUDP(std::move(ClientAddress));
			} break;
			default:
				break;
			}

			return MicrosoftNetworkServerClientConnectionScope(this, NewServerClientConnection);
		}

		void MicrosoftNetworkServer::ReleaseClientConnection(_In_ MicrosoftNetworkConnection* InClientConnection)
		{
			_Connections.Push(InClientConnection);
		}
	}
}
