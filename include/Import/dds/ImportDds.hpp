#pragma once

#include <string>

namespace Eternal
{
	namespace Graphics
	{
		enum class Format;
		enum class ResourceDimension;
	}

	namespace Import
	{
		using namespace std;
		using namespace Eternal::Graphics;

		class ImportDds
		{
		public:
			uint8_t* Import(_In_ const string& InPath, _Out_ Format& OutFormat, _Out_ ResourceDimension& OutResourceDimension, _Out_ uint32_t& OutWidth, _Out_ uint32_t& OutHeight, _Out_ uint32_t& OutDepth);
		};
	}
}
