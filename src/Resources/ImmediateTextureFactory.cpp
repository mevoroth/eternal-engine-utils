#include "Resources/ImmediateTextureFactory.hpp"

#include "File/FileFactory.hpp"
#include "Import/tga/ImportTga.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/CommandQueue.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/CommandList.hpp"
#include <string>

namespace Eternal
{
	namespace Resources
	{
		bool ImmediateTextureFactoryLoadTextureCallback::LoadTexture(_In_ const string& Path, _Out_ RawTextureData& OutTextureData)
		{
			using namespace Eternal::File;
			using namespace Eternal::Import;

			if (FileExists(Path))
			{
				uint32_t Height	= 0;
				uint32_t Width	= 0;
				uint8_t* ImageData = ImportTga::Get()->Import(Path, Height, Width);

				OutTextureData.Initialize(ImageData, Width, Height, 1, 4);
				return true;
			}

			return false;
		}

		ImmediateTextureFactoryCreateGpuResourceCallback::ImmediateTextureFactoryCreateGpuResourceCallback(_In_ GraphicsContext& InContext)
			: _Context(InContext)
		{
		}

		void ImmediateTextureFactoryCreateGpuResourceCallback::BeginBatch()
		{
			ETERNAL_ASSERT(!_CommandList);
			_CommandList = _Context.CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHIC);
			_CommandList->Begin(_Context);

		}

		void ImmediateTextureFactoryCreateGpuResourceCallback::EndBatch()
		{
			ETERNAL_ASSERT(_CommandList);
			_CommandList->End();
			_Context.GetGraphicsQueue().SubmitCommandLists(
				&_CommandList, 1
			);
			_CommandList = nullptr;
		}

		bool ImmediateTextureFactoryCreateGpuResourceCallback::CreateTextureResource(_In_ const string& InName, _In_ const RawTextureData& InTextureData, _Out_ Resource*& OutTexture)
		{
			using namespace Eternal::Graphics;

			//////////////////////////////////////////////////////////////////////////
			// CPU Buffer
			std::string UploadTextureName = "AnonymousTextureBuffer";
			const uint32_t UploadBufferSize = InTextureData.Width * InTextureData.Height * InTextureData.DepthOrArraySize * InTextureData.Stride;
			//TextureResourceCreateInformation UploadBufferTextureInformation(
			BufferResourceCreateInformation UploadBufferTextureInformation(
				_Context.GetDevice(),
				UploadTextureName,
				BufferCreateInformation(
					Format::FORMAT_BGRA8888_UNORM,
					BufferResourceUsage::BUFFER_RESOURCE_USAGE_COPY_READ,
					UploadBufferSize
				),
				ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
			);

			Resource* UploadTexture = CreateBuffer(UploadBufferTextureInformation); // TODO: Release LEAKING HERE

			//////////////////////////////////////////////////////////////////////////
			// Map
			MapRange UploadTextureMapRange(UploadBufferSize);
			void* UploadTextureDataPtr = UploadTexture->Map(UploadTextureMapRange);
			memcpy(UploadTextureDataPtr, InTextureData.TextureData, UploadBufferSize);
			UploadTexture->Unmap(UploadTextureMapRange);

			//////////////////////////////////////////////////////////////////////////
			// GPU Texture
			TextureResourceCreateInformation GPUTextureCreateInformation(
				_Context.GetDevice(),
				InName,
				TextureCreateInformation(
					ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D,
					Format::FORMAT_BGRA8888_UNORM,
					TextureResourceUsage::TEXTURE_RESOURCE_USAGE_SHADER_RESOURCE | TextureResourceUsage::TEXTURE_RESOURCE_USAGE_COPY_WRITE,
					InTextureData.Width,
					InTextureData.Height,
					InTextureData.DepthOrArraySize,
					1
				),
				ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY,
				TransitionState::TRANSITION_COPY_WRITE
			);
			OutTexture = CreateTexture(GPUTextureCreateInformation);

			//////////////////////////////////////////////////////////////////////////
			// Upload
			_CommandList->CopyResource(
				*OutTexture,
				*UploadTexture,
				CopyRegion(
					TextureFromBufferRegion(
						Extent3D(InTextureData.Width, InTextureData.Height, InTextureData.DepthOrArraySize),
						UploadBufferSize
					)
				)
			);

			//////////////////////////////////////////////////////////////////////////
			// Transition
			ResourceTransition TextureCopyWriteToShaderResource(
				OutTexture,
				TransitionState::TRANSITION_PIXEL_SHADER_READ
			);
			_CommandList->Transition(
				&TextureCopyWriteToShaderResource, 1
			);

			return true;
		}
	}
}
