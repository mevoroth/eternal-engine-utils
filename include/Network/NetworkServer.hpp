#pragma once

#include "Network/Network.hpp"

namespace Eternal
{
	namespace NetworkSystem
	{
		struct NetworkServerCreateInformation : public NetworkCreateInformation
		{
		};

		class NetworkServer
		{
		public:

			NetworkServer(_In_ const NetworkServerCreateInformation& InNetworkCreateInformation);

		protected:

			NetworkServerCreateInformation _NetworkCreateInformation;

		};
	}
}
