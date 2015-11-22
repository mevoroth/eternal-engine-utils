#ifndef _TEXTURE_FACTORY_HPP_
#define _TEXTURE_FACTORY_HPP_

#include <map>
#include <string>
#include <cstdint>

using namespace std;

namespace Eternal
{
	namespace Resources
	{
		class TextureFactory
		{
		public:
			static TextureFactory* Get();

			TextureFactory();
			uint8_t* GetTexture(_In_ const string& NameSrc, _Out_ uint32_t& Height, _Out_ uint32_t& Width);

		private:
			static TextureFactory* _Inst;

			struct TextureCache
			{
				uint8_t* TextureData = nullptr;
				uint32_t Width = 0;
				uint32_t Height = 0;
			};
			map<string, TextureCache> _Textures;
		};
	}
}

#endif
