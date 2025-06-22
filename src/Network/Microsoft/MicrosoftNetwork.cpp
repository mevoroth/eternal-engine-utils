#include "Network/Microsoft/MicrosoftNetwork.hpp"
#include "Network/Network.hpp"
#include <WS2tcpip.h>

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

		//////////////////////////////////////////////////////////////////////////

		void MicrosoftNetworkSocket::CreateSocket(_In_ const NetworkCreateInformation& InCreateInformation, _In_ const char* InHost)
		{
			char PortString[6];
			sprintf_s(PortString, "%d", InCreateInformation.Port);

			addrinfo Hints = {};

			switch (InCreateInformation.InternetLayer)
			{
			case NetworkInternetLayer::INTERNET_V4:	Hints.ai_family = AF_INET;	break;
			case NetworkInternetLayer::INTERNET_V6:	Hints.ai_family = AF_INET6;	break;
			default: break;
			}

			switch (InCreateInformation.TransportLayer)
			{
			case NetworkTransportLayer::TRANSPORT_TCP:
				Hints.ai_socktype = SOCK_STREAM;
				Hints.ai_protocol = IPPROTO_TCP;
				break;

			case NetworkTransportLayer::TRANSPORT_UDP:
				Hints.ai_socktype = SOCK_DGRAM;
				Hints.ai_protocol = IPPROTO_UDP;
				break;

			default:
				break;
			}

			Hints.ai_flags = 0;

			if (InCreateInformation.TransportLayer == NetworkTransportLayer::TRANSPORT_TCP)
			{
				int Result = getaddrinfo(InHost, PortString, &Hints, &_Address);
				ETERNAL_ASSERT(Result == 0);
				ETERNAL_ASSERT(Hints.ai_family == _Address->ai_family);
				ETERNAL_ASSERT(Hints.ai_socktype == _Address->ai_socktype);
				ETERNAL_ASSERT(Hints.ai_protocol == _Address->ai_protocol);
			}

			_Socket = socket(Hints.ai_family, Hints.ai_socktype, Hints.ai_protocol);
			ETERNAL_ASSERT(_Socket != INVALID_SOCKET);
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkConnection::MicrosoftNetworkConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
			: NetworkConnection(InSendBufferSize, InReceiveBufferSize)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkConnectionTCP::MicrosoftNetworkConnectionTCP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
			: MicrosoftNetworkConnection(InSendBufferSize, InReceiveBufferSize)
		{
		}

		void MicrosoftNetworkConnectionTCP::Poll()
		{
			_ReceiveBuffer.resize(_ReceiveBuffer.capacity());
			int ReceivedBytes = recv(ConnectionSocket, reinterpret_cast<char*>(_ReceiveBuffer.data()), static_cast<int>(_ReceiveBuffer.capacity()), 0);
			ETERNAL_ASSERT(ReceivedBytes != SOCKET_ERROR);
			_ReceiveBuffer.resize(ReceivedBytes);
		}

		void MicrosoftNetworkConnectionTCP::Commit()
		{
			int Result = send(ConnectionSocket, reinterpret_cast<const char*>(_SendBuffer.data()), static_cast<int>(_SendBuffer.size()), 0);
			ETERNAL_ASSERT(Result != SOCKET_ERROR);
			_SendBuffer.clear();
		}

		void MicrosoftNetworkConnectionTCP::SetupMicrosoftNetworkConnectionTCP(_In_ SOCKET InConnectionSocket)
		{
			ConnectionSocket = InConnectionSocket;
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkConnectionUDP::MicrosoftNetworkConnectionUDP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
			: MicrosoftNetworkConnection(InSendBufferSize, InReceiveBufferSize)
		{
		}

		void MicrosoftNetworkConnectionUDP::Poll()
		{
			ETERNAL_BREAK();
		}

		void MicrosoftNetworkConnectionUDP::Commit()
		{
			ETERNAL_BREAK();
		}

		void MicrosoftNetworkConnectionUDP::SetupMicrosoftNetworkConnectionUDP(_In_ sockaddr_in&& InClientAddress)
		{
			ClientAddress = std::move(InClientAddress);
		}

	}
}
