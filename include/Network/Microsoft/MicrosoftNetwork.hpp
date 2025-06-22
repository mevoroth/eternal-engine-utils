#pragma once

#include "Network/Network.hpp"
#include <WinSock2.h>

namespace Eternal
{
	namespace NetworkSystem
	{
		struct NetworkCreateInformation;

		class MicrosoftNetworkSocket
		{
		protected:

			void CreateSocket(_In_ const NetworkCreateInformation& InCreateInformation, _In_ const char* InHost = nullptr);

			addrinfo*	_Address	= nullptr;
			SOCKET		_Socket		= INVALID_SOCKET;

		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkConnection : public NetworkConnection
		{
		protected:

			MicrosoftNetworkConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkConnectionTCP : public MicrosoftNetworkConnection
		{
		public:

			MicrosoftNetworkConnectionTCP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

			virtual void Poll() override final;
			virtual void Commit() override final;

			void SetupMicrosoftNetworkConnectionTCP(_In_ SOCKET InConnectionSocket);

		private:

			SOCKET ConnectionSocket = INVALID_SOCKET;
		};

		//////////////////////////////////////////////////////////////////////////

		class MicrosoftNetworkConnectionUDP : public MicrosoftNetworkConnection
		{
		public:

			MicrosoftNetworkConnectionUDP(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize);

			virtual void Poll() override final;
			virtual void Commit() override final;

			void SetupMicrosoftNetworkConnectionUDP(_In_ sockaddr_in&& InClientAddress);

		private:

			sockaddr_in ClientAddress = {};

			friend class MicrosoftNetworkServer;

		};
	}
}
