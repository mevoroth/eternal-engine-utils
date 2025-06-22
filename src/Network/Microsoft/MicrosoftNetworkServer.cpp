#include "Network/Microsoft/MicrosoftNetworkServer.hpp"
#include <WS2tcpip.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		MicrosoftNetworkServerClientConnection::MicrosoftNetworkServerClientConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
			: NetworkServerClientConnection(InSendBufferSize, InReceiveBufferSize)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkServerClientConnectionTCP::MicrosoftNetworkServerClientConnectionTCP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
			: MicrosoftNetworkServerClientConnection(InSendBufferSize, InReceiveBufferSize)
		{
		}

		void MicrosoftNetworkServerClientConnectionTCP::Poll()
		{
			_ReceiveBuffer.resize(_ReceiveBuffer.capacity());
			int ReceivedBytes = recv(ClientSocket, reinterpret_cast<char*>(_ReceiveBuffer.data()), static_cast<int>(_ReceiveBuffer.capacity()), 0);
			ETERNAL_ASSERT(ReceivedBytes != SOCKET_ERROR);
			_ReceiveBuffer.resize(ReceivedBytes);
		}

		void MicrosoftNetworkServerClientConnectionTCP::Commit()
		{
			int Result = send(ClientSocket, reinterpret_cast<const char*>(_SendBuffer.data()), static_cast<int>(_SendBuffer.size()), 0);
			ETERNAL_ASSERT(Result != SOCKET_ERROR);
			_SendBuffer.clear();
		}

		void MicrosoftNetworkServerClientConnectionTCP::SetupMicrosoftNetworkServerClientConnectionTCP(_In_ SOCKET InClientSocket)
		{
			ClientSocket = InClientSocket;
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkServerClientConnectionUDP::MicrosoftNetworkServerClientConnectionUDP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
			: MicrosoftNetworkServerClientConnection(InSendBufferSize, InReceiveBufferSize)
		{
		}

		void MicrosoftNetworkServerClientConnectionUDP::Poll()
		{
			ETERNAL_BREAK();
		}

		void MicrosoftNetworkServerClientConnectionUDP::Commit()
		{
			ETERNAL_BREAK();
		}

		void MicrosoftNetworkServerClientConnectionUDP::SetupMicrosoftNetworkServerClientConnectionUDP(_In_ sockaddr_in&& InClientAddress)
		{
			ClientAddress = std::move(InClientAddress);
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkServerClientConnectionScope::MicrosoftNetworkServerClientConnectionScope(_In_ MicrosoftNetworkServer* InServer, _In_ MicrosoftNetworkServerClientConnection* InClientConnection)
			: _Server(InServer)
			, _ClientConnection(InClientConnection)
		{
		}

		MicrosoftNetworkServerClientConnectionScope::~MicrosoftNetworkServerClientConnectionScope()
		{
			if (_ClientConnection)
				_Server->ReleaseClientConnection(_ClientConnection);
		}

		//////////////////////////////////////////////////////////////////////////

		constexpr uint32_t MicrosoftNetworkServer::ServerClientConnectionInitialPoolCount;

		MicrosoftNetworkServer::MicrosoftNetworkServer(_In_ const NetworkServerCreateInformation& InNetworkCreateInformation)
			: NetworkServer(InNetworkCreateInformation)
			, _Connections(
				[this](void) -> MicrosoftNetworkServerClientConnection*
				{
					MicrosoftNetworkServerClientConnection* NewMicrosoftNetworkServerClientConnection = nullptr;

					if (_NetworkCreateInformation.TransportLayer == NetworkTransportLayer::TRANSPORT_TCP)
						NewMicrosoftNetworkServerClientConnection = new MicrosoftNetworkServerClientConnectionTCP(8192u, 16384u);

					if (_NetworkCreateInformation.TransportLayer == NetworkTransportLayer::TRANSPORT_UDP)
						NewMicrosoftNetworkServerClientConnection = new MicrosoftNetworkServerClientConnectionUDP(8192u, 16384u);

					ETERNAL_ASSERT(NewMicrosoftNetworkServerClientConnection);
					return NewMicrosoftNetworkServerClientConnection;
				},
				ServerClientConnectionInitialPoolCount
			)
		{
		}

		bool MicrosoftNetworkServer::BindSocket()
		{
			char PortString[6];
			sprintf_s(PortString, "%d", _NetworkCreateInformation.Port);
			
			addrinfo Hints = {};
			
			switch (_NetworkCreateInformation.InternetLayer)
			{
			case NetworkInternetLayer::INTERNET_V4:	Hints.ai_family = AF_INET;	break;
			case NetworkInternetLayer::INTERNET_V6:	Hints.ai_family = AF_INET6;	break;
			default: break;
			}

			switch (_NetworkCreateInformation.TransportLayer)
			{
			case NetworkTransportLayer::TRANSPORT_TCP:
				Hints.ai_socktype	= SOCK_STREAM;
				Hints.ai_protocol	= IPPROTO_TCP;
				break;

			case NetworkTransportLayer::TRANSPORT_UDP:
				Hints.ai_socktype	= SOCK_DGRAM;
				Hints.ai_protocol	= IPPROTO_UDP;
				break;

			default:
				break;
			}

			Hints.ai_flags = 0;

			int Result = getaddrinfo(nullptr, PortString, &Hints, &_Address);
			ETERNAL_ASSERT(Result == 0);

			_Socket = socket(_Address->ai_family, _Address->ai_socktype, _Address->ai_protocol);
			ETERNAL_ASSERT(_Socket != INVALID_SOCKET);

			Result = ::bind(_Socket, _Address->ai_addr, static_cast<int>(_Address->ai_addrlen));
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
			MicrosoftNetworkServerClientConnection* NewServerClientConnection = nullptr;

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
				static_cast<MicrosoftNetworkServerClientConnectionTCP*>(NewServerClientConnection)->SetupMicrosoftNetworkServerClientConnectionTCP(ClientSocket);
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
				static_cast<MicrosoftNetworkServerClientConnectionUDP*>(NewServerClientConnection)->SetupMicrosoftNetworkServerClientConnectionUDP(std::move(ClientAddress));
			} break;
			default:
				break;
			}

			return MicrosoftNetworkServerClientConnectionScope(this, NewServerClientConnection);
		}
	}
}
