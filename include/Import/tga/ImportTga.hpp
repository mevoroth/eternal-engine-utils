#pragma once

#include <string>

namespace Eternal
{
	namespace Import
	{
		using namespace std;

		class ImportTga
		{
		public:
			uint8_t* Import(_In_ const string& InPath, _Out_ uint32_t& OutWidth, _Out_ uint32_t& OutHeight);
		};
	}
}
