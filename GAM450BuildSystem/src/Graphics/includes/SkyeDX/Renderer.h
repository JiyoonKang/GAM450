/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Renderer.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Renderer.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once


#include <Windows.h> //HWND
#include <Base\util\math\VectorMath.h>

#include <Graphics\includes\SkyeDX\DXTypes.h>
#include <Graphics\includes\SkyeDX\ObjectHandle.h>
#include <Graphics\includes\SkyeDX\InputData.h>
#include <Graphics\includes\SkyeDX\Area.h>
#include <Graphics\includes\SkyeDX\Viewport.h>

namespace Skye
{

	class Renderer
	{
		public:
			//Constructors/Destructors
			Renderer(void);
			virtual ~Renderer(void);

			//Basic Public Methods
			virtual void Initialize(void) = 0;
			virtual void ClearBuffer(void) = 0;
			virtual void SwapBuffers(void) = 0;
			static Renderer* Instance(void);

			//Drawing
			virtual void Draw(unsigned vertexCount, unsigned startVertexLocation = 0) = 0;
			virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation = 0, unsigned baseVertexLocation = 0) = 0;
			virtual void DrawInstanced(unsigned vertexCount, unsigned instanceCount, unsigned startVertexLocation = 0, unsigned startInstanceLocation = 0) = 0;
			virtual void DrawIndexedInstanced(unsigned indexCountPerInstance, unsigned instanceCount, unsigned startIndexLocation = 0, 
											  unsigned baseVertexLocation = 0, unsigned startInstanceLocation = 0) = 0;
      virtual void DrawString(const std::string text, float fontSize, float xPos, float yPos, unsigned int color) = 0;
      virtual void DrawString(const std::string text, float fontSize, float xPos, float yPos, float4 color) = 0;

			//Texture Creation
			virtual void CreateTexture(ObjectHandle& texture, const std::string& fileName, bool generateMipChain = true) = 0;
			virtual void CreateTexture(ObjectHandle& texture, const void* data, const Area& size, const DataFormat format) = 0;
			virtual void CreateTextureFromMemory(ObjectHandle& texture, const void* data, unsigned size) = 0;

			
			virtual void CreateRenderTarget(ObjectHandle& renderTarget, const DataFormat format, const Area& size = Area(0, 0)) = 0;

			//Buffer Creation
			virtual void CreateVertexBuffer(ObjectHandle& vertexBuffer, const BufferUsage bufferUsage, unsigned size, const void* initialData = nullptr) = 0;
			virtual void CreateIndexBuffer(ObjectHandle& indexBuffer, const BufferUsage bufferUsage, unsigned size, const void* initialData = nullptr) = 0;
			virtual void CreateConstantBuffer(ObjectHandle& constantBuffer, unsigned size) = 0;
			virtual void CreateDepthBuffer(ObjectHandle& depthBuffer, const ObjectHandle& associatedRenderTarget = ObjectHandle::Null(), const Area& size = Area(0, 0)) = 0;

			//Shader Creation
			virtual void CreateVertexShader(ObjectHandle& vertexShader, const std::string& fileName, const InputLayout& inputLayout, 
											bool precompiled, const std::string& entryPoint = "main") = 0;

			virtual void CreatePixelShader(ObjectHandle& pixelShader, const std::string& fileName, bool precompiled, 
										   const std::string& entryPoint = "main") = 0;

			virtual void CreateGeometryShader(ObjectHandle& geometryShader, const std::string& fileName, bool precompiled, 
											  const std::string& entryPoint = "main") = 0;
      
      //Font Wrapper Creation
      virtual void CreateFontWrapper(ObjectHandle& fontWrapper, std::string& fontName) = 0;
      virtual void DrawFontWrapperDrawCalls(void) = 0;
      
      //Binding
			virtual void BindTexture(unsigned slot, const ObjectHandle& texture) = 0;
			virtual void BindTextures(unsigned count, const ObjectHandle textures[]) = 0;
			
			virtual void BindRenderTarget(const ObjectHandle& renderTarget) = 0;
			virtual void BindRenderTargets(unsigned count, const ObjectHandle renderTargets[]) = 0;

			virtual void BindVertexShader(const ObjectHandle& vertexShader) = 0;
			virtual void BindPixelShader(const ObjectHandle& pixelShader) = 0;
			virtual void BindGeometryShader(const ObjectHandle& geometryShader) = 0;
			virtual void BindComputeShader(const ObjectHandle& computeShader) = 0;

      virtual void BindNullVertexBuffer() = 0;
			virtual void BindVertexBuffer(const ObjectHandle& vertexBuffer, unsigned stride) = 0;
			
			virtual void BindIndexBuffer(const ObjectHandle& indexBuffer) = 0;

			virtual void BindConstantBuffer(unsigned slot, const ObjectHandle& constantBuffer, const ObjectType shaderType) = 0;

			virtual void BindDepthBuffer(const ObjectHandle& depthBuffer) = 0;

			//Release
			virtual void Release(const ObjectHandle& object) = 0;

			//Setters
			virtual void SetWindow(const HWND& hwnd);
			virtual void SetBackColor(const float4& color);
			virtual void SetVsync(bool vSync);

			virtual void SetViewport(const Viewport & viewport) = 0;
			virtual void SetMSAALevel(const MSAALevel level);
			virtual void SetPrimitiveTopology(const PrimitiveTopology topology) = 0;
			virtual void SetBlendMode(const BlendMode mode) = 0;
			virtual void SetFillMode(const FillMode mode) = 0;
			virtual void SetCullMode(const CullMode mode) = 0;
			virtual void SetAnisotropy(const Anisotropy anisotropy) = 0;
			virtual void SetDepthTesting(bool depthTesting) = 0;
			virtual void SetDepthWriting(bool depthWriting) = 0;
			virtual void SetScissorRect(unsigned rectCount, SkyeRect* rect) = 0;

			//Getters
			virtual const ObjectHandle& GetBackBuffer(void) const;
			virtual bool GetVsync(void) const;
			virtual const Viewport& GetViewport(const ObjectHandle& target = ObjectHandle::Null()) const = 0;
			virtual const Area& GetTextureSize(const ObjectHandle& texture) = 0;
			virtual void* GetDevice(void) const = 0;
			virtual void* GetDeviceContext(void) const = 0;
			virtual const std::string& GetGPUInfo(void) = 0;


			//Utility methods
			virtual void CopyData(const ObjectHandle& object, const void* data, unsigned size) = 0;
			virtual void CopyResource(const ObjectHandle& dest, const ObjectHandle & source) = 0;

			virtual void* Map(const ObjectHandle& buffer) = 0;
			virtual void* Map(const ObjectHandle& texture, unsigned& rowPitch) = 0;
			virtual void Unmap(const ObjectHandle& buffer) = 0;

			virtual void ClearRenderTarget(const ObjectHandle& renderTarget, const float4& color) = 0;
			virtual void ClearRenderTargets(unsigned count, const ObjectHandle renderTargets[], const float4& color) = 0;
			virtual void ClearDepthBuffer(const ObjectHandle& depthBuffer) = 0;

			virtual void ResizeDepthBuffer(const ObjectHandle& depthBuffer, const Area& size = Area(0, 0)) = 0;

			virtual void OnResize(WPARAM wParam, LPARAM lParam) = 0;
			virtual void OnActivate(WPARAM wParam, LPARAM lParam) = 0;

		protected:
			ObjectHandle CreateHandle(const ObjectType type, const int handle);
			
		protected:
			HWND m_hwnd;
			float4 m_ScreenColor;
			ObjectHandle m_BackBuffer;
			Viewport m_Viewport;
			MSAALevel m_MSAALevel;
			bool m_Vsync;

		private:
			static Renderer* m_Instance;

			//friend class ObjectHandle;
	};
}