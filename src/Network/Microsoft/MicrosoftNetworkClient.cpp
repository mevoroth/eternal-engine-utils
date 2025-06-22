#include "Network/Microsoft/MicrosoftNetworkClient.hpp"

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
				ETERNAL_ASSERT(Result != SOCKET_ERROR);

				NewNetworkConnection = new MicrosoftNetworkConnectionTCP(8192u, 16384u);
				static_cast<MicrosoftNetworkConnectionTCP*>(NewNetworkConnection)->SetupMicrosoftNetworkConnectionTCP(_Socket);
			} break;
			case NetworkTransportLayer::TRANSPORT_UDP:
			{

			} break;
			}

			ETERNAL_ASSERT(NewNetworkConnection);
			return MicrosoftNetworkClientServerConnectionScope(NewNetworkConnection);
		}
	}
}
