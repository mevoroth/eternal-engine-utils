#pragma once

namespace Eternal
{
	namespace NetworkSystem
	{
		enum class NetworkInternetLayer
		{
			INTERNET_V4,
			INTERNET_V6
		};

		enum class NetworkTransportLayer
		{
			TRANSPORT_TCP,
			TRANSPORT_UDP
		};

		struct NetworkCreateInformation
		{
			NetworkInternetLayer	InternetLayer	= NetworkInternetLayer::INTERNET_V4;
			NetworkTransportLayer	TransportLayer	= NetworkTransportLayer::TRANSPORT_TCP;
			uint16_t				Port			= 0;
		};

		void InitializeNetworkSystem();
	}
}
