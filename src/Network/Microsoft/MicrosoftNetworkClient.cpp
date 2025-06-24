#include "Network/Microsoft/MicrosoftNetworkClient.hpp"
#include <WS2tcpip.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		MicrosoftNetworkClientServerConnectionScope::MicrosoftNetworkClientServerConnectionScope(_In_ MicrosoftNetworkConnection* InServerConnection)
			: _ServerConnection(InServerConnection)
		{
		}

		MicrosoftNetworkClientServerConnectionScope::~MicrosoftNetworkClientServerConnectionScope()
		{
			delete _ServerConnection;
			_ServerConnection = nullptr;
		}

		MicrosoftNetworkConnection* MicrosoftNetworkClientServerConnectionScope::operator->()
		{
			return _ServerConnection;
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftNetworkClient::MicrosoftNetworkClient(_In_ const NetworkClientCreateInformation& InNetworkCreateInformation)
			: NetworkClient(InNetworkCreateInformation)
		{
		}

		MicrosoftNetworkClientServerConnectionScope MicrosoftNetworkClient::ConnectToServer()
		{
			CreateSocket(_NetworkCreateInformation, _NetworkCreateInformation.Host.c_str());

			MicrosoftNetworkConnection* NewNetworkConnection = nullptr;

			switch (_NetworkCreateInformation.TransportLayer)
			{
			case NetworkTransportLayer::TRANSPORT_TCP:
			{
				int Result = connect(_Socket, _Address->ai_addr, _Address->ai_addrlen);
				while (Result == SOCKET_ERROR && _Address)
				{
					_Address = _Address->ai_next;
					_Socket = socket(_Address->ai_family, _Address->ai_socktype, _Address->ai_protocol);
					if (_Socket == INVALID_SOCKET)
					{
						break;
					}
					Result = connect(_Socket, _Address->ai_addr, _Address->ai_addrlen);
				}

				ETERNAL_ASSERT(Result != SOCKET_ERROR);
				ETERNAL_ASSERT(_Socket != INVALID_SOCKET);

				NewNetworkConnection = new MicrosoftNetworkConnectionTCP(8192u, 16384u);
				static_cast<MicrosoftNetworkConnectionTCP*>(NewNetworkConnection)->SetupMicrosoftNetworkConnectionTCP(_Socket);
			} break;
			case NetworkTransportLayer::TRANSPORT_UDP:
			{
				sockaddr_in	HostAddress			= {};
				int			HostAddressLength	= sizeof(sockaddr_in);

				switch (_NetworkCreateInformation.InternetLayer)
				{
				case NetworkInternetLayer::INTERNET_V4: HostAddress.sin_family = AF_INET;		break;
				case NetworkInternetLayer::INTERNET_V6: HostAddress.sin_family = AF_INET6;	break;
				default: break;
				}

				HostAddress.sin_port			= htons(_NetworkCreateInformation.Port);
				int Result = inet_pton(HostAddress.sin_family, _NetworkCreateInformation.Host.c_str(), &HostAddress.sin_addr);
				ETERNAL_ASSERT(Result > 0);

				char MagicKeyBuffer[64] = "ETERNAL";
				Result = sendto(
					_Socket,
					MagicKeyBuffer,
					ETERNAL_ARRAYSIZE(MagicKeyBuffer),
					0,
					reinterpret_cast<sockaddr*>(&HostAddress),
					HostAddressLength
				);

				NewNetworkConnection = new MicrosoftNetworkConnectionUDP(8192u, 16384u);
				static_cast<MicrosoftNetworkConnectionUDP*>(NewNetworkConnection)->SetupMicrosoftNetworkConnectionUDP(_Socket, std::move(HostAddress));
			} break;
			}

			ETERNAL_ASSERT(NewNetworkConnection);
			return MicrosoftNetworkClientServerConnectionScope(NewNetworkConnection);
		}
	}
}
