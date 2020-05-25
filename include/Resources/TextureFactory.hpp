#ifndef _RESOURCES_TEXTURE_FACTORY_HPP_
#define _RESOURCES_TEXTURE_FACTORY_HPP_

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#include <list>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Texture;
	}
	namespace Parallel
	{
		class Mutex;
	}
	namespace Resources
	{
		using namespace Graphics;
		using namespace Parallel;

		struct RawTextureData
		{
			~RawTextureData();

			void* TextureData = nullptr;
			uint32_t Width = 0;
			uint32_t Height = 0;
			uint32_t Depth = 0;
		};

		class TextureFactoryLoadTextureCallback
		{
		public:
			virtual bool LoadTexture(_In_ const string& Path, _Out_ RawTextureData& OutTextureData) = 0;
		};

		class TextureFactoryCreateGpuResourceCallback
		{
		public:
			virtual bool CreateTexture(_In_ const RawTextureData& TextureData, _Out_ Texture*& OutTexture) = 0;
		};

		struct TextureFactoryCreateInformation
		{
			TextureFactoryLoadTextureCallback& LoadTextureCallback;
			TextureFactoryCreateGpuResourceCallback& CreateGpuResourceCallback;
		};

		struct TextureFactoryRequest
		{
			TextureFactoryRequest(const string& InName, const string& InPath);

			TextureFactoryRequest(const TextureFactoryRequest& Request)
				: Name(Request.Name)
				, Path(Request.Path)
			{
			}

			string Name;
			string Path;
		};

		struct TextureFactoryLoadedTexture : TextureFactoryRequest
		{
			TextureFactoryLoadedTexture(const TextureFactoryRequest& Request)
				: TextureFactoryRequest(Request)
			{
			}

			RawTextureData TextureData;
		};

		class TextureFactory
		{
		public:
			static TextureFactory* Get();

			TextureFactory(_In_ const TextureFactoryCreateInformation& CreateInformation);

			void CreateRequest(const TextureFactoryRequest& Request);
			void ProcessRequests();


			Texture* GetTexture(_In_ const string& NameSrc);
			static void RegisterTexturePath(_In_ const string& Path);

		private:
			static TextureFactory* _Inst;

			TextureFactoryCreateInformation Callbacks;
			struct TextureCache
			{
				Texture* TextureObj = nullptr;
			};
			map<string, TextureCache> _Textures;
			Mutex* _NewRequestsMutex = nullptr;
			vector<TextureFactoryRequest> _NewRequests;
			list<TextureFactoryRequest> _PendingLoadTextureRequests;
			list<TextureFactoryLoadedTexture> _PendingCreateGpuResourceRequests;


			void _InsertNewTexture(_In_ const string& NameSrc, _In_ Texture* TextureObj);

			static vector<string> _IncludePaths;
		};
	}
}

#endif
