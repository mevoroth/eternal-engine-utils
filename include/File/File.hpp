#pragma once

#include <vector>
#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		enum class FileOpenMode
		{
			FILE_OPEN_MODE_CLOSED		= 0x0,
			FILE_OPEN_MODE_READ			= 0x1,
			FILE_OPEN_MODE_WRITE		= 0x2,
			FILE_OPEN_MODE_READ_WRITE	= FILE_OPEN_MODE_READ | FILE_OPEN_MODE_WRITE
		};

		enum class FileCursor
		{
			FILE_CURSOR_SET = 0,
			FILE_CURSOR_CUR = 1,
			FILE_CURSOR_END = 2
		};

		class File
		{
		public:

			virtual ~File() {}
			virtual void Open(_In_ const FileOpenMode& InOpenMode);
			virtual void Close();
			virtual void Read(_In_ uint8_t* InBlock, _In_ uint64_t InSize);
			virtual void Write(_In_ const uint8_t* InBlock, _In_ uint64_t InSize);
			virtual void Seek(_In_ uint64_t InOffset, _In_ const FileCursor& InCursor) = 0;
			virtual uint64_t Tell() const = 0;
			virtual uint64_t GetFileSize() = 0;

			void Serialize(_Inout_ uint8_t* InOutBlock, _In_ uint64_t InSize);

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
			template<typename Type, typename ElementConstructorFunction, typename ElementSerializeFunction>
			void Serialize(_Inout_ Type*& InOutValue, _In_ const ElementConstructorFunction& InElementConstructorFunctor, _In_ const ElementSerializeFunction& InElementSerializeFunctor)
			{
				if (IsOpenedForReadExclusive())
					InOutValue = InElementConstructorFunctor();
				InElementSerializeFunctor(InOutValue);
			}
			template<typename Type, typename ElementConstructorFunction, typename ElementSerializeFunction>
			void Serialize(_Inout_ std::vector<Type*>& InOutValue, _In_ const ElementConstructorFunction& InElementConstructorFunctor, _In_ const ElementSerializeFunction& InElementSerializeFunctor)
			{
				uint32_t Count = static_cast<uint32_t>(InOutValue.size());
				Serialize(Count);
				InOutValue.resize(Count);
				for (uint32_t ElementIndex = 0; ElementIndex < Count; ++ElementIndex)
					Serialize(InOutValue[ElementIndex], InElementConstructorFunctor, InElementSerializeFunctor);
			}
			void Serialize(_Inout_ std::string& InOutValue)
			{
				uint32_t Length = static_cast<uint32_t>(InOutValue.length());
				Serialize(Length);
				InOutValue.resize(Length);
				Serialize(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(InOutValue.data())), Length);
			}
			void Serialize(_Inout_ std::vector<std::string>& InOutValue)
			{
				uint32_t Count = static_cast<uint32_t>(InOutValue.size());
				Serialize(Count);
				InOutValue.resize(Count);
				for (uint32_t ElementIndex = 0; ElementIndex < Count; ++ElementIndex)
					Serialize(InOutValue[ElementIndex]);
			}

			bool IsOpenedForReadExclusive() const { return _OpenMode == FileOpenMode::FILE_OPEN_MODE_READ; }
			bool IsOpenedForWriteExclusive() const { return _OpenMode == FileOpenMode::FILE_OPEN_MODE_WRITE; }
			bool IsOpenedForRead() const;
			bool IsOpenedForWrite() const;

		private:
			FileOpenMode _OpenMode = FileOpenMode::FILE_OPEN_MODE_CLOSED;
		};
	}
}
