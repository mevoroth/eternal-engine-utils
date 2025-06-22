#include "Network/NetworkServer.hpp"

namespace Eternal
{
	namespace NetworkSystem
	{
		NetworkServer::NetworkServer(_In_ const NetworkServerCreateInformation& InNetworkCreateInformation)
			: _NetworkCreateInformation(InNetworkCreateInformation)
		{
		}
	}
}
