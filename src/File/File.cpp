#include "File/File.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		void File::Open(_In_ const OpenMode& InOpenMode)
		{
			_OpenMode = InOpenMode;
		}
	}
}
