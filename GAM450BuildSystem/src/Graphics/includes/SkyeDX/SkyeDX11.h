/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SkyeDX11.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  SkyeDX11.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <Graphics\includes\SkyeDX\Renderer.h>
#include "Graphics\includes\SkyeDX\ObjectHandle.h"
#include "Viewport.h"
#include "DXTypes.h"
#include "InputData.h"
#include "Area.h"

namespace Skye
{
	struct DX11Data;

	class SkyeDX11 : public Renderer
	{
		public:
			SkyeDX11(void);
			~SkyeDX11(void);

			//Basic Methods
			virtual void Initialize(void) override;
			virtual void ClearBuffer(void) override;
			virtual void SwapBuffers(void) override;

			//Drawing
			virtual void Draw(unsigned vertexCount, unsigned startVertexLocation = 0) override;
			virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation = 0, unsigned baseVertexLocation = 0) override;
			virtual void DrawInstanced(unsigned vertexCount, unsigned instanceCount, unsigned startVertexLocation = 0, unsigned startInstanceLocation = 0) override;
			virtual void DrawIndexedInstanced(unsigned indexCountPerInstance, unsigned instanceCount, unsigned startIndexLocation = 0, 
											  unsigned baseVertexLocation = 0, unsigned startInstanceLocation = 0) override;
      
      virtual void DrawString(const std::string text, float fontSize, float xPos, float yPos, unsigned int color);
      virtual void DrawString(const std::string text, float fontSize, float xPos, float yPos, float4 color);
      virtual void DrawFontWrapperDrawCalls(void);
			
      //Texture Creation
			virtual void CreateTexture(ObjectHandle& texture, const std::string& fileName, bool generateMipChain = true) override;
			virtual void CreateTexture(ObjectHandle& texture, const void* data, const Area& size, const DataFormat format) override;
			virtual void CreateTextureFromMemory(ObjectHandle& texture, const void* data, unsigned size) override;


			virtual void CreateRenderTarget(ObjectHandle& renderTarget, const DataFormat format, const Area& size = Area(0, 0)) override;

			//Buffer Creation
			virtual void CreateVertexBuffer(ObjectHandle& vertexBuffer, const BufferUsage bufferUsage, unsigned size, const void* initialData = nullptr) override;
			virtual void CreateIndexBuffer(ObjectHandle& indexBuffer, const BufferUsage bufferUsage, unsigned size, const void* initialData = nullptr) override;
			virtual void CreateConstantBuffer(ObjectHandle& constantBuffer, unsigned size) override;
			virtual void CreateDepthBuffer(ObjectHandle& depthBuffer, const ObjectHandle& associatedRenderTarget = ObjectHandle::Null(), const Area& size = Area(0, 0)) override;

			//Shader Creation
			virtual void CreateVertexShader(ObjectHandle& vertexShader, const std::string& fileName, const InputLayout& inputLayout,
				bool precompiled, const std::string& entryPoint = "main") override;

			virtual void CreatePixelShader(ObjectHandle& pixelShader, const std::string& fileName, bool precompiled,
				const std::string& entryPoint = "main") override;

			virtual void CreateGeometryShader(ObjectHandle& geometryShader, const std::string& fileName, bool precompiled,
				const std::string& entryPoint = "main") override;

      //Font Wrapper Creation
      virtual void CreateFontWrapper(ObjectHandle& fontWrapper, std::string& fontName);

			//Binding
			virtual void BindTexture(unsigned slot, const ObjectHandle& texture) override;
			virtual void BindTextures(unsigned count, const ObjectHandle textures[]) override;

			virtual void BindRenderTarget(const ObjectHandle& renderTarget) override;
			virtual void BindRenderTargets(unsigned count, const ObjectHandle renderTargets[]) override;

			virtual void BindVertexShader(const ObjectHandle& vertexShader) override;
			virtual void BindPixelShader(const ObjectHandle& pixelShader) override;
			virtual void BindGeometryShader(const ObjectHandle& geometryShader) override;
			virtual void BindComputeShader(const ObjectHandle& computeShader) override;

      virtual void BindNullVertexBuffer() override;
			virtual void BindVertexBuffer(const ObjectHandle& vertexBuffer, unsigned stride) override;

			virtual void BindIndexBuffer(const ObjectHandle& indexBuffer) override;

			virtual void BindConstantBuffer(unsigned slot, const ObjectHandle& constantBuffer, const ObjectType shaderType) override;

			virtual void BindDepthBuffer(const ObjectHandle& depthBuffer) override;

			//Release
			virtual void Release(const ObjectHandle& object) override;

			//Setters

			virtual void SetViewport(const Viewport& viewport) override;
			virtual void SetPrimitiveTopology(const PrimitiveTopology topology) override;
			virtual void SetBlendMode(const BlendMode mode) override;
			virtual void SetFillMode(const FillMode mode) override;
			virtual void SetCullMode(const CullMode mode) override;
			virtual void SetAnisotropy(const Anisotropy anisotropy) override;
			virtual void SetDepthTesting(bool depthTesting) override;
			virtual void SetDepthWriting(bool depthWriting) override;
			virtual void SetScissorRect(unsigned rectCount, SkyeRect* rect) override;

			//Getters
			virtual void* GetDevice(void) const override;
			virtual void* GetDeviceContext(void) const override;
			virtual const std::string& GetGPUInfo(void) override;
			virtual const Viewport& GetViewport(const ObjectHandle& renderTarget = ObjectHandle::Null()) const override;
      virtual const Area& GetTextureSize(const ObjectHandle& texture) override;


			//Utility methods
			virtual void CopyData(const ObjectHandle& object, const void* data, unsigned size) override;
			virtual void CopyResource(const ObjectHandle& dest, const ObjectHandle & source) override;

			virtual void* Map(const ObjectHandle& buffer) override;
			virtual void* Map(const ObjectHandle& texture, unsigned& rowPitch) override;
			virtual void Unmap(const ObjectHandle& buffer) override;

			virtual void ClearRenderTarget(const ObjectHandle& renderTarget, const float4& color) override;
			virtual void ClearRenderTargets(unsigned count, const ObjectHandle renderTargets[], const float4& color) override;
			virtual void ClearDepthBuffer(const ObjectHandle& depthBuffer) override;

			virtual void ResizeDepthBuffer(const ObjectHandle& depthBuffer, const Area& size = Area(0, 0)) override;

			virtual void OnResize(WPARAM wParam, LPARAM lParam) override;
			virtual void OnActivate(WPARAM wParam, LPARAM lParam) override;

		private:
			//Internal Resource Creation

			//Texture Creation
			void InternalCreateTexture(ObjectHandle& handle, const std::string& fileName, bool generateMipChain);
			void InternalCreateTexture(ObjectHandle& handle, const void * data, const Area& size, const DataFormat format);
			void InternalCreateTextureFromMemory(ObjectHandle& handle, const void * data, unsigned size);

			void InternalCreateRenderTarget(ObjectHandle& handle, const DataFormat format, const Area& size);

			//Shader Creation
			void InternalCreateVertexShader(ObjectHandle& handle, const std::string& sourceFileName, const InputLayout& inputLayout, bool precompiled, const std::string& entryPoint);
			void InternalCreatePixelShader(ObjectHandle& handle, const std::string& sourceFileName, bool precompiled, const std::string& entryPoint);
			void InternalCreateGeometryShader(ObjectHandle& handle, const std::string& sourceFileName, bool precompiled, const std::string& entryPoint);

			//Buffer Creation
			void InternalCreateVertexBuffer(ObjectHandle& handle, const BufferUsage usage, unsigned size, const void* initialData);
			void InternalCreateIndexBuffer(ObjectHandle& handle, const BufferUsage usage, unsigned size, const void* initialData);
			void InternalCreateConstantBuffer(ObjectHandle& handle, unsigned size);
			void InternalCreateDepthBuffer(ObjectHandle& handle, const ObjectHandle& toResizeWith, const Area& size);

      //Font Factory / Wrapper Creation
      void InternalCreateFontWrapper(ObjectHandle& handle, const std::string& fontName);

			//Resource Release
			void ReleaseTexture(const ObjectHandle& texture);
			void ReleaseRenderTarget(const ObjectHandle& renderTarget);
			void ReleaseVertexShader(const ObjectHandle& vertexShader);
			void ReleasePixelShader(const ObjectHandle& pixelShader);
			void ReleaseGeometryShader(const ObjectHandle& geometryShader);
			void ReleaseComputeShader(const ObjectHandle& computeShader);
			void ReleaseVertexBuffer(const ObjectHandle& vertexBuffer);
			void ReleaseIndexBuffer(const ObjectHandle& indexBuffer);
			void ReleaseConstantBuffer(const ObjectHandle& constantBuffer);
			void ReleaseDepthBuffer(const ObjectHandle& depthBuffer);
      void ReleaseFontWrapper(const ObjectHandle& fontWrapper);

			//Initialization 
			void InitializeDXGI(void);
			void InitializeDevice(void);
			void InitializeSwapChain(void);
			void InitializeBackBuffer(void);
			void InitializeDepthStencilStates(void);
			void InitializeRasterizerStates(void);
			void InitializeSamplerStates(void);
			void InitializeBlendStates(void);
      void InitializeFontWrapperFactory(void);
			
      void ResizeBuffers(unsigned width, unsigned height);

			//DirectX11 Data
			DX11Data* m_Data;
	};
}