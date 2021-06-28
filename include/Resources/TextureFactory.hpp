#pragma once

#include <map>
#include <string>
#include <list>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
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
			void Initialize(
				_In_ void* InTextureData,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight,
				_In_ uint32_t InDepthOrArraySize,
				_In_ uint32_t InStride
			);
			void Release();

			void* TextureData			= nullptr;
			uint32_t Width				= 0;
			uint32_t Height				= 0;
			uint32_t DepthOrArraySize	= 0;
			uint32_t Stride				= 0;
		};

		class TextureFactoryLoadTextureCallback
		{
		public:
			virtual bool LoadTexture(_In_ const string& InPath, _Out_ RawTextureData& OutTextureData) = 0;
		};

		class TextureFactoryCreateGpuResourceCallback
		{
		public:
			virtual void BeginBatch() = 0;
			virtual void EndBatch() = 0;
			virtual bool CreateTextureResource(_In_ const string& InName, _Inout_ RawTextureData& TextureData, _Out_ Resource*& OutTexture) = 0;
		};

		struct TextureFactoryCreateInformation
		{
			TextureFactoryLoadTextureCallback& LoadTextureCallback;
			TextureFactoryCreateGpuResourceCallback& CreateGpuResourceCallback;
		};

		struct TextureFactoryRequest
		{
			TextureFactoryRequest(const string& InName, const string& InPath);

			TextureFactoryRequest(const TextureFactoryRequest& InRequest)
				: Name(InRequest.Name)
				, Path(InRequest.Path)
			{
			}

			string Name;
			string Path;
		};

		struct TextureFactoryLoadedTexture : TextureFactoryRequest
		{
			TextureFactoryLoadedTexture(const TextureFactoryRequest& InRequest)
				: TextureFactoryRequest(InRequest)
			{
			}

			RawTextureData TextureData;
		};

		class TextureFactory
		{
		public:
			static TextureFactory* Get();

			TextureFactory(_In_ const TextureFactoryCreateInformation& InTextureFactoryCreateInformation);

			void CreateRequest(const TextureFactoryRequest& InRequest);
			void ProcessRequests();
			bool HasRequests() const;

			Resource* GetTexture(_In_ const string& InName);
			static void RegisterTexturePath(_In_ const string& InPath);

		private:
			static TextureFactory* _Instance;

			TextureFactoryCreateInformation Callbacks;
			struct TextureCache
			{
				Resource* CachedTexture = nullptr;
			};
			map<string, TextureCache> _Textures;
			Mutex* _NewRequestsMutex = nullptr;
			vector<TextureFactoryRequest> _NewRequests;
			list<TextureFactoryRequest> _PendingLoadTextureRequests;
			list<TextureFactoryLoadedTexture> _PendingCreateGpuResourceRequests;

			void _InsertNewTexture(_In_ const string& InName, _In_ Resource* Intexture);

			static vector<string> _IncludePaths;
		};
	}
}
