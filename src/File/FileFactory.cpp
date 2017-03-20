#include "File/FileFactory.hpp"

#include "UtilsSettings.hpp"
#ifdef ETERNAL_USE_CFILE
#include "File/CFile.hpp"
#endif

namespace Eternal
{
	namespace File
	{
		File* CreateFile(const std::string& FileName)
		{
#ifdef ETERNAL_USE_CFILE
			return new CFile(FileName);
#else
			return nullptr;
#endif
		}

		void DestroyFile(File*& FileObj)
		{
			delete FileObj;
			FileObj = nullptr;
		}

		bool FileExists(const std::string& FileName)
		{
#ifdef ETERNAL_USE_CFILE
			return CFile::Exists(FileName);
#else
			return false;
#endif
		}
	}
}
