#include "Network/NetworkClient.hpp"

namespace Eternal
{
	namespace NetworkSystem
	{
		NetworkClient::NetworkClient(_In_ const NetworkClientCreateInformation& InNetworkCreateInformation)
			: _NetworkCreateInformation(InNetworkCreateInformation)
		{
		}
	}
}
