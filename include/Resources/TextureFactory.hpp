#ifndef _RESOURCES_TEXTURE_FACTORY_HPP_
#define _RESOURCES_TEXTURE_FACTORY_HPP_

#include <map>
#include <string>
#include <cstdint>
#include <vector>

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
			static void RegisterTexturePath(_In_ const string& Path);

		private:
			static TextureFactory* _Inst;

			struct TextureCache
			{
				Texture* TextureObj = nullptr;
			};
			map<string, TextureCache> _Textures;

			void _InsertNewTexture(_In_ const string& NameSrc, _In_ Texture* TextureObj);

			static vector<string> _IncludePaths;
		};
	}
}

#endif
