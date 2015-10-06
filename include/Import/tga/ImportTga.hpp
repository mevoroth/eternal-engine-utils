#ifndef _IMPORT_TGA_HPP_
#define _IMPORT_TGA_HPP_

#include <string>
#include <cstdint>

using namespace std;

namespace Eternal
{
	namespace Import
	{
		class ImportTga
		{
		public:
			ImportTga();
			static ImportTga* Get();
			uint8_t* Import(_In_ const string& Path, _Out_ uint32_t& Height, _Out_ uint32_t& Width);

		private:
			static ImportTga* _Inst;
		};
	}
}

#endif
