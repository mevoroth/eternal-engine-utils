#include "File/FileFactory.hpp"

#include "UtilsSettings.hpp"
#ifdef ETERNAL_USE_CFILE
#include "File/CFile.hpp"
#endif

namespace Eternal
{
	namespace FileSystem
	{
		File* CreateFileHandle(const std::string& InFileName)
		{
#ifdef ETERNAL_USE_CFILE
			return new CFile(InFileName);
#else
			return nullptr;
#endif
		}

		void DestroyFileHandle(File*& InFile)
		{
			delete InFile;
			InFile = nullptr;
		}

		bool FileExists(const std::string& InFileName)
		{
#ifdef ETERNAL_USE_CFILE
			return CFile::Exists(InFileName);
#else
			return false;
#endif
		}
	}
}
