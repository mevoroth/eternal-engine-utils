#pragma once

#include "Network/Network.hpp"
#include <string>

namespace Eternal
{
	namespace NetworkSystem
	{
		struct NetworkClientCreateInformation : public NetworkCreateInformation
		{
			std::string Host;
		};

		class NetworkClient
		{
		public:

			NetworkClient(_In_ const NetworkClientCreateInformation& InNetworkCreateInformation);

		protected:

			NetworkClientCreateInformation _NetworkCreateInformation;

		};
	}
}
