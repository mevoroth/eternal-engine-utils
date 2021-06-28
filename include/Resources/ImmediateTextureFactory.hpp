#pragma once

#include "Resources/TextureFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class CommandList;
	}

	namespace Resources
	{
		using namespace Eternal::Graphics;

		class ImmediateTextureFactoryLoadTextureCallback : public TextureFactoryLoadTextureCallback
		{
		public:
			virtual bool LoadTexture(_In_ const string& Path, _Out_ RawTextureData& OutTextureData) override final;
		};

		class ImmediateTextureFactoryCreateGpuResourceCallback : public TextureFactoryCreateGpuResourceCallback
		{
		public:
			ImmediateTextureFactoryCreateGpuResourceCallback(_In_ GraphicsContext& InContext);

			virtual void BeginBatch() override final;
			virtual void EndBatch() override final;
			virtual bool CreateTextureResource(_In_ const string& InName, _Inout_ RawTextureData& InOutTextureData, _Out_ Resource*& OutTexture) override final;

		private:
			GraphicsContext&	_Context;
			CommandList*		_CommandList = nullptr;
		};
	}
}
