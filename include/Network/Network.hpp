#pragma once

namespace Eternal
{
	namespace NetworkSystem
	{
		using namespace std;

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

		enum class NetworkStrategy
		{
			NETWORKSTRATEGY_IMMEDIATE,
			NETWORKSTRATEGY_BATCH,
			NETWORKSTRATEGY_DEFERRED
		};

		class NetworkConnection
		{
		public:

			static constexpr NetworkStrategy ServerStrategy = NetworkStrategy::NETWORKSTRATEGY_IMMEDIATE;

			void Send(_In_ const uint8_t* InBlock, _In_ uint64_t InSize);
			uint32_t Receive(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize);

			virtual void Poll() = 0;
			virtual void Commit() = 0;

		protected:

			NetworkConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

			vector<uint8_t> _SendBuffer;
			vector<uint8_t> _ReceiveBuffer;

		};

		void InitializeNetworkSystem();
	}
}
