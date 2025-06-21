#pragma once

#include "Network/Network.hpp"

namespace Eternal
{
	namespace NetworkSystem
	{
		using namespace std;

		enum class NetworkStrategy
		{
			NETWORKSTRATEGY_IMMEDIATE,
			NETWORKSTRATEGY_BATCH,
			NETWORKSTRATEGY_DEFERRED
		};

		class NetworkServerClientConnection
		{
		public:

			static constexpr NetworkStrategy ServerStrategy = NetworkStrategy::NETWORKSTRATEGY_IMMEDIATE;

			void Send(_In_ const uint8_t* InBlock, _In_ uint64_t InSize);
			uint32_t Receive(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize);

			virtual void Poll() = 0;
			virtual void Commit() = 0;

		protected:

			NetworkServerClientConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

			vector<uint8_t> _SendBuffer;
			vector<uint8_t> _ReceiveBuffer;

		};

		//////////////////////////////////////////////////////////////////////////

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
