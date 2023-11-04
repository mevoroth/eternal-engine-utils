#pragma once

#include <vector>
#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		class File
		{
		public:
			enum class OpenMode
			{
				NOT_OPENED	= 0x0,
				READ		= 0x1,
				WRITE		= 0x2,
				READ_WRITE	= READ | WRITE
			};
			enum class Origin
			{
				SET = 0,
				CUR = 1,
				END = 2
			};
			virtual ~File() {}
			virtual void Open(_In_ const OpenMode& InOpenMode);
			virtual void Close() = 0;
			virtual void Read(_In_ uint8_t* InBlock, _In_ uint64_t InSize) = 0;
			virtual void Write(_In_ uint8_t* InBlock, _In_ uint64_t InSize) = 0;
			virtual void Serialize(_Inout_ uint8_t* InOutBlock, _In_ uint64_t InSize) = 0;
			virtual void Seek(_In_ uint64_t InOffset, _In_ const Origin& InOrigin) = 0;
			virtual uint64_t Tell() const = 0;
			virtual uint64_t GetFileSize() = 0;

			template<typename Type>
			void Serialize(_Inout_ Type& InOutValue)
			{
				Serialize(reinterpret_cast<uint8_t*>(&InOutValue), sizeof(Type));
			}

			template<typename Type>
			void Serialize(_Inout_ std::vector<Type>& InOutValue)
			{
				uint32_t Count = static_cast<uint32_t>(InOutValue.size());
				Serialize(Count);
				InOutValue.resize(Count);
				Serialize(reinterpret_cast<uint8_t*>(InOutValue.data()), sizeof(Type) * Count);
			}
			template<typename Type, typename PerElementConstructorFunction, typename PerElementSerializeFunction>
			void Serialize(_Inout_ std::vector<Type*>& InOutValue, _In_ const PerElementConstructorFunction& InPerElementConstructorFunctor, _In_ const PerElementSerializeFunction& InPerElementSerializeFunctor)
			{
				uint32_t Count = static_cast<uint32_t>(InOutValue.size());
				Serialize(Count);
				InOutValue.resize(Count);
				for (uint32_t ElementIndex = 0; ElementIndex < Count; ++ElementIndex)
				{
					if (IsOpenedForReadExclusive())
						InOutValue[ElementIndex] = InPerElementConstructorFunctor();

					InPerElementSerializeFunctor(InOutValue[ElementIndex]);
				}
			}
			void Serialize(_Inout_ std::string& InOutValue)
			{
				uint32_t Length = static_cast<uint32_t>(InOutValue.length());
				Serialize(Length);
				InOutValue.resize(Length);
				Serialize(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(InOutValue.data())), Length);
			}

			bool IsOpenedForReadExclusive() const { return _OpenMode == OpenMode::READ; }
			bool IsOpenedForWriteExclusive() const { return _OpenMode == OpenMode::WRITE; }
			bool IsOpenedForRead() const { return (static_cast<uint32_t>(_OpenMode) & static_cast<uint32_t>(OpenMode::READ)) != static_cast<uint32_t>(OpenMode::NOT_OPENED); }
			bool IsOpenedForWrite() const { return (static_cast<uint32_t>(_OpenMode) & static_cast<uint32_t>(OpenMode::WRITE)) != static_cast<uint32_t>(OpenMode::NOT_OPENED); }

		private:
			OpenMode _OpenMode = OpenMode::NOT_OPENED;
		};
	}
}
