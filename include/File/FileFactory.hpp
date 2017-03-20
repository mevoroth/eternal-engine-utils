#ifndef _FILE_FACTORY_HPP_
#define _FILE_FACTORY_HPP_

#include <string>

namespace Eternal
{
	namespace File
	{
		class File;

		File* CreateFile(const std::string& FileName);
		void DestroyFile(File*& FileObj);
		bool FileExists(const std::string& FileName);
	}
}

#endif
