#ifndef _TEXTURE_FACTORY_HPP_
#define _TEXTURE_FACTORY_HPP_

#include <map>
#include <string>
#include <cstdint>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Texture;
	}
	namespace Resources
	{
		using namespace Graphics;

		class TextureFactory
		{
		public:
			static TextureFactory* Get();

			TextureFactory();
			Texture* GetTexture(_In_ const string& NameSrc);

		private:
			static TextureFactory* _Inst;

			struct TextureCache
			{
				Texture* TextureObj = nullptr;
			};
			map<string, TextureCache> _Textures;
		};
	}
}

#endif
