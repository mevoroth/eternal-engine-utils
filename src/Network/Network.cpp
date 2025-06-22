#include "Network/Network.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace NetworkSystem
	{
		NetworkConnection::NetworkConnection(_In_ uint32_t InSendBufferSize, _In_ uint32_t InReceiveBufferSize)
		{
			_SendBuffer.reserve(InSendBufferSize);
			_ReceiveBuffer.reserve(InReceiveBufferSize);
		}

		void NetworkConnection::Send(_In_ const uint8_t* InBlock, _In_ uint64_t InSize)
		{
			if constexpr (ServerStrategy == NetworkStrategy::NETWORKSTRATEGY_IMMEDIATE)
			{
				uint32_t PacketCount = static_cast<uint32_t>(Math::DivideRoundUp(InSize, _SendBuffer.capacity()));

				uint32_t PacketIndex = 0;
				uint32_t PacketCountMinus1 = PacketCount - 1;
				uint32_t CurrentBlockBegin = 0;

				while (PacketIndex < PacketCountMinus1)
				{
					uint32_t CurrentBlockEnd = CurrentBlockBegin + _SendBuffer.capacity();

					_SendBuffer.insert(
						_SendBuffer.end(),
						&InBlock[CurrentBlockBegin],
						&InBlock[CurrentBlockEnd]
					);

					Commit();

					CurrentBlockBegin = CurrentBlockEnd;

					++PacketIndex;
				}

				uint32_t LastBlockEnd = Math::Min(CurrentBlockBegin + _SendBuffer.capacity(), InSize);
				if (CurrentBlockBegin < LastBlockEnd)
				{
					_SendBuffer.insert(
						_SendBuffer.end(),
						&InBlock[CurrentBlockBegin],
						&InBlock[LastBlockEnd]
					);

					Commit();
				}
			}

			if constexpr (ServerStrategy == NetworkStrategy::NETWORKSTRATEGY_BATCH)
			{
			}

			if constexpr (ServerStrategy == NetworkStrategy::NETWORKSTRATEGY_DEFERRED)
			{
			}
		}

		uint32_t NetworkConnection::Receive(_Out_ uint8_t* OutBlock, _In_ uint64_t InSize)
		{
			if (InSize <= 0)
				return 0u;

			uint32_t ReceivedBytes = 0u;

			if constexpr (ServerStrategy == NetworkStrategy::NETWORKSTRATEGY_IMMEDIATE)
			{
				Poll();
				ReceivedBytes = static_cast<uint32_t>(Math::Min(InSize, _ReceiveBuffer.size()));
				memcpy(OutBlock, _ReceiveBuffer.data(), ReceivedBytes);
				_ReceiveBuffer.erase(_ReceiveBuffer.begin(), _ReceiveBuffer.begin() + ReceivedBytes);
			}

			if constexpr (ServerStrategy == NetworkStrategy::NETWORKSTRATEGY_BATCH)
			{
			}

			if constexpr (ServerStrategy == NetworkStrategy::NETWORKSTRATEGY_DEFERRED)
			{
			}

			return ReceivedBytes;
		}

	}
}
