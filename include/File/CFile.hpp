#ifndef _CFILE_HPP_
#define _CFILE_HPP_

#include <string>
#include "File.hpp"
#include <cstdio>

namespace Eternal
{
	namespace File
	{
		class CFile : public File
		{
		public:
			static bool Exists(const std::string& FileName);

			CFile(const std::string& FileName);
			virtual void Open(const OpenMode& OpenModeObj) override;
			virtual void Close() override;
			virtual void Read(uint8_t* Block, uint64_t Size) override;
			virtual void Write(uint8_t* Block, uint64_t Size) override;
			virtual void Seek(uint64_t Offset, const Origin& OriginObj) override;
			virtual uint64_t Tell() const override;
			virtual uint64_t GetFileSize() override;

		private:
			std::string _FileName;
			std::FILE* _File;

		};
	}
}

#endif
