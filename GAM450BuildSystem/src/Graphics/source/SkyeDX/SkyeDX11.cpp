/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SkyeDX11.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  SkyeDX11.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch/precompiled.h"
#include <includes\SkyeDX\SkyeDX11.h>
#include <includes\SkyeDX\DirectXObject.h>
#include <Core\systems\window\Window.h>
#include <Base\util\debug\DebugAssist.h>

#include <includes\SkyeDX\DXIncludes.h>
#include "includes/GraphicsSystem/Vertex.h"
#include <fstream>
//#include "..\..\Microsoft DirectX SDK (June 2010)\Include\dxgi.h"
#include <includes/SkyeDX/FW1FontWrapper.h>

#define MAX_BIND_SLOTS 8

namespace Skye
{
	template<typename Container>
	int NextAvailableIndex(const Container& container)
	{
		//Iterate through container
		for (unsigned i = 0; i < container.size(); ++i)
		{
			//If the element is a free and already deleted handle,
			//return it's index
			if (container[i].deleted)
				return i;
		}

		//No free spaces
		return -1;
	}

	//Struct containing all the necessary data for DirectX to run
	struct DX11Data
	{
		bool initialized;
		bool fullscreen;
		bool vsync;

		//Version compatibility
		FeatureLevel lowestFeatureLevel;
		MSAALevel desiredMSAA;

		//DXGI Objects
		SwapChainPtr swapChain;
		FactoryPtr factory;
		AdapterPtr adapter;
		OutputPtr output;

		//DirectX Device and Context
		DevicePtr device;
		DeviceContextPtr deviceContext;

		//Depth testing
		DepthStencilStatePtr depthStencilTestOnly, depthStencilWriteOnly, depthStencilBothEnabled, depthStencilBothDisabled;

		bool depthTesting, depthWriting;

		//Rasterizer state data
		RasterizerStatePtr rsNone, rsBack, rsFront, rsScissor, rsImgui;
		FillMode fillMode;

		//Blend mode: None, Alpha, Additive
		BlendStatePtr blendState[3];

		//Texture filter mode: Point, Bilinear, Trilinear, Anisotropic
		SamplerStatePtr samplerStateClamp[4];
		SamplerStatePtr samplerStateWrap[4];

		Anisotropy anisotropy;

		//Display mode container
		std::vector<DXGI_MODE_DESC> displayModes;
		unsigned displayModeIndex;

		DXGI_SAMPLE_DESC msaaLevel;

		//Resource containers
		std::vector<D3DObject::Texture> textures;
		std::vector<D3DObject::RenderTarget> renderTargets;
		std::vector<D3DObject::VertexShader> vertexShaders;
		std::vector<D3DObject::PixelShader> pixelShaders;
		std::vector<D3DObject::GeometryShader> geometryShaders;
		std::vector<D3DObject::ComputeShader> computeShaders;
		std::vector<D3DObject::Buffer> vertexBuffers;
		std::vector<D3DObject::Buffer> indexBuffers;
		std::vector<D3DObject::Buffer> constantBuffers;
		std::vector<D3DObject::DepthBuffer> depthBuffers;

		ID3D11DepthStencilView* currentDepthBuffer;

		//Render targets
		D3DObject::RenderTarget backBuffer;
		ID3D11RenderTargetView* renderTargetList[8];
		unsigned boundRenderTargets;

		static const DXGI_FORMAT RENDER_TARGET_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
		std::string GPUInfo;

		ObjectHandle lastVertexShader, lastPixelShader;
		BlendMode blendMode;

    //Font factory & wrappers
    FW1FontFactoryPtr fontFactory;
    std::vector<D3DObject::FontWrapper> fontWrappers;
    std::vector<D3DObject::FontWrapperDrawCall> fontWrapperDrawCalls;

	};

	SkyeDX11::SkyeDX11(void) : Renderer(), m_Data(new DX11Data)
	{
		//Initializing the data structure
		m_Data->initialized = false;
		m_Data->fullscreen = WINDOWSSYSTEM->IsFullScreen();
		m_Data->vsync = true;

		//Default structure values
		m_Data->anisotropy = Anisotropy::ANISOTROPY_1X;
		m_Data->desiredMSAA = MSAALevel::MSAA_LEVEL_NONE;
		m_Data->lowestFeatureLevel = FeatureLevel::FEATURE_LEVEL_DX11_0;

		//Default rendering
		m_Data->fillMode = FillMode::FILL_SOLID;
		m_Data->currentDepthBuffer = nullptr;
		m_Data->blendMode = BlendMode::BLEND_MODE_NONE;
		m_Data->depthTesting = false;
		m_Data->depthWriting = false;

		//Zeroing out memory for render targets
		ZeroMemory(&m_Data->renderTargetList, sizeof(ID3D11RenderTargetView*) * 8);
		m_Data->boundRenderTargets = 0;
	}

	SkyeDX11::~SkyeDX11(void)
	{
		if (m_Data->initialized)
		{
			m_Data->swapChain->SetFullscreenState(false, nullptr);
		}

		delete m_Data;
		PrintOut("Skye Graphics has been uninitialized");
	}

	void SkyeDX11::Initialize(void)
	{
		//Initialize DirectX items
		InitializeDXGI();
		InitializeDevice();
		InitializeSwapChain();
		InitializeBackBuffer();
		InitializeDepthStencilStates();
		InitializeRasterizerStates();
		InitializeSamplerStates();
		InitializeBlendStates();

		//DirectX has been initialized
		m_Data->initialized = true;


		m_Data->swapChain->SetFullscreenState(m_Data->fullscreen, nullptr);

		//Bind render target
		ID3D11RenderTargetView* rtv = m_Data->backBuffer.rtv;
		m_Data->deviceContext->OMSetRenderTargets(1, &rtv, nullptr);

    //Initialize font factory
    InitializeFontWrapperFactory();
	}

	void SkyeDX11::ClearBuffer(void)
	{
		ID3D11RenderTargetView* rtv = m_Data->backBuffer.rtv;
		m_Data->deviceContext->ClearRenderTargetView(rtv, m_ScreenColor.v);
	}

	void SkyeDX11::SwapBuffers(void)
	{
		int vsync = m_Data->vsync ? 1 : 0;

		HRESULT hr = m_Data->swapChain->Present(vsync, 0);
	}

	void SkyeDX11::Draw(unsigned vertexCount, unsigned startVertexLocation)
	{
		m_Data->deviceContext->Draw(vertexCount, startVertexLocation);
	}

	void SkyeDX11::DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned baseVertexLocation)
	{
		m_Data->deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}

	void SkyeDX11::DrawInstanced(unsigned vertexCount, unsigned instanceCount, unsigned startVertexLocation, unsigned startInstanceLocation)
	{
		m_Data->deviceContext->DrawInstanced(vertexCount, instanceCount, startVertexLocation, startInstanceLocation);
	}

	void SkyeDX11::DrawIndexedInstanced(unsigned indexCountPerInstance, unsigned instanceCount, unsigned startIndexLocation, unsigned baseVertexLocation, unsigned startInstanceLocation)
	{
		m_Data->deviceContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
	}

  void SkyeDX11::DrawString(const std::string text, float fontSize, float xPos, float yPos, unsigned int color)
  {
    std::wstring temp;
    temp.assign(text.begin(), text.end());

    D3DObject::FontWrapperDrawCall fwDrCll;
    fwDrCll.text = temp;
    fwDrCll.fontSize = fontSize;
    fwDrCll.xPos = xPos;
    fwDrCll.yPos = yPos;
    fwDrCll.color = color;
    m_Data->fontWrapperDrawCalls.push_back(fwDrCll);
  }

  void SkyeDX11::DrawString(const std::string text, float fontSize, float xPos, float yPos, float4 color)
  {
    DrawString(text, fontSize, xPos, yPos, 0xff0099ff);
  }

  void SkyeDX11::DrawFontWrapperDrawCalls(void)
  {
    for (unsigned int i = 0; i < m_Data->fontWrapperDrawCalls.size(); ++i)
    {
      m_Data->fontWrappers[0].fontWrapper->DrawString(m_Data->deviceContext, m_Data->fontWrapperDrawCalls[i].text.c_str(), 
        m_Data->fontWrapperDrawCalls[i].fontSize, m_Data->fontWrapperDrawCalls[i].xPos, m_Data->fontWrapperDrawCalls[i].yPos, 
        m_Data->fontWrapperDrawCalls[i].color, FW1_RESTORESTATE | FW1_VCENTER | FW1_CENTER);
    }

    m_Data->fontWrapperDrawCalls.clear();
  }

  void SkyeDX11::CreateTexture(ObjectHandle& texture, const std::string& fileName, bool generateMipChain)
	{
		InternalCreateTexture(texture, fileName, generateMipChain);
	}

	void SkyeDX11::CreateTexture(ObjectHandle& texture, const void* data, const Area& size, const DataFormat format)
	{
		InternalCreateTexture(texture, data, size, format);
	}

	void SkyeDX11::CreateTextureFromMemory(ObjectHandle& texture, const void* data, unsigned size)
	{
		InternalCreateTextureFromMemory(texture, data, size);
	}

	void SkyeDX11::CreateRenderTarget(ObjectHandle& renderTarget, const DataFormat format, const Area& size)
	{
		InternalCreateRenderTarget(renderTarget, format, size);
	}

	void SkyeDX11::CreateVertexBuffer(ObjectHandle& vertexBuffer, const BufferUsage bufferUsage, unsigned size, const void* initialData)
	{
		InternalCreateVertexBuffer(vertexBuffer, bufferUsage, size, initialData);
	}

	void SkyeDX11::CreateIndexBuffer(ObjectHandle& indexBuffer, const BufferUsage bufferUsage, unsigned size, const void* initialData)
	{
		InternalCreateIndexBuffer(indexBuffer, bufferUsage, size, initialData);
	}

	void SkyeDX11::CreateConstantBuffer(ObjectHandle& constantBuffer, unsigned size)
	{
		InternalCreateConstantBuffer(constantBuffer, size);
	}

	void SkyeDX11::CreateDepthBuffer(ObjectHandle& depthBuffer, const ObjectHandle& associatedRenderTarget, const Area& size)
	{
		InternalCreateDepthBuffer(depthBuffer, associatedRenderTarget, size);
	}

	void SkyeDX11::CreateVertexShader(ObjectHandle& vertexShader, const std::string& fileName, const InputLayout& inputLayout, bool precompiled, const std::string& entryPoint)
	{
		InternalCreateVertexShader(vertexShader, fileName, inputLayout, precompiled, entryPoint);
	}

	void SkyeDX11::CreatePixelShader(ObjectHandle& pixelShader, const std::string& fileName, bool precompiled, const std::string& entryPoint)
	{
		InternalCreatePixelShader(pixelShader, fileName, precompiled, entryPoint);
	}

  void SkyeDX11::CreateGeometryShader(ObjectHandle& geometryShader, const std::string& fileName, bool precompiled, const std::string& entryPoint)
  {
    InternalCreateGeometryShader(geometryShader, fileName, precompiled, entryPoint);
  }

  void SkyeDX11::CreateFontWrapper(ObjectHandle& fontWrapper, std::string& fontName)
  {
    InternalCreateFontWrapper(fontWrapper, fontName);
  }

  void SkyeDX11::BindTexture(unsigned slot, const ObjectHandle& texture)
	{
		//Make sure it is possible to bind a texture
		if (slot >= MAX_BIND_SLOTS)
		{
			Warning("Warning: Unable to bind texture. Maximum slots already occupied.");
			return;
		}

		ID3D11ShaderResourceView* srv(nullptr);

		//If the handle is empty
		if (!texture)
		{
			//Bind an empty srv
			m_Data->deviceContext->PSSetShaderResources(slot, 1, &srv);
			return;
		}

		ObjectType type = texture.GetType();

		//If the object is a texture, bind from the texture container
		if (type == ObjectType::TEXTURE)
		{
			srv = m_Data->textures[*texture].srv;
		}

		//If the object is a render target, bind form the render target container
		else if (type == ObjectType::RENDER_TARGET)
		{
			srv = m_Data->renderTargets[*texture].srv;
		}

		//If the object is a depth buffer, bind form the depth buffer container
		else if (type == ObjectType::DEPTH_BUFFER)
		{
			srv = m_Data->depthBuffers[*texture].srv;
		}

		//Bind srv to the shader
		m_Data->deviceContext->PSSetShaderResources(slot, 1, &srv);
	}

	void SkyeDX11::BindTextures(unsigned count, const ObjectHandle textures[])
	{
		//Ensure at least 
		if (count < 1)
		{
			Warning("Warning: At least one texture is needed. Invalid texture count of: %i textures", count);
			return;
		}

		//Allocate SRV array depending on how many textures we want to bind
		ID3D11ShaderResourceView ** srvArray = reinterpret_cast<ID3D11ShaderResourceView**>(_malloca(sizeof(ID3D11ShaderResourceView*) * count));

		for (unsigned i = 0; i < count; ++i)
		{
			//Get the handle for the current texture to be bound
			const ObjectHandle& texture = textures[i];

			//If the current handle is null, assign an empty srv
			if (!texture)
			{
				srvArray[i] = nullptr;
			}

			else
			{
				ObjectType type = texture.GetType();

				//If the object is a texture, bind from the texture container
				if (type == ObjectType::TEXTURE)
				{
					srvArray[i] = m_Data->textures[*texture].srv;
				}

				//If the object is a render target, bind form the render target container
				else if (type == ObjectType::RENDER_TARGET)
				{
					srvArray[i] = m_Data->renderTargets[*texture].srv;
				}

				//If the object is a depth buffer, bind form the depth buffer container
				else if (type == ObjectType::DEPTH_BUFFER)
				{
					srvArray[i] = m_Data->depthBuffers[*texture].srv;
				}
			}
		}

		//Once all textures have been assigned to the SRV array, bind the array
		m_Data->deviceContext->PSSetShaderResources(0, count, srvArray);

	}

	void SkyeDX11::BindRenderTarget(const ObjectHandle& renderTarget)
	{
		//If handle is empty, assign an empty render target
		if (!renderTarget)
		{
			ID3D11RenderTargetView* rtv = nullptr;
			m_Data->deviceContext->OMSetRenderTargets(0, &rtv, nullptr);
			return;
		}

		//Get the current depth buffer
		ID3D11DepthStencilView* dsv = m_Data->currentDepthBuffer;

		//Setting the depth testing attributes
		if (m_Data->depthTesting && m_Data->depthWriting)
		{
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilBothEnabled, 1);
		}

		else if (m_Data->depthTesting)
		{
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilTestOnly, 1);
		}

		else if (m_Data->depthWriting)
		{
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilWriteOnly, 1);
		}

		else
		{
			//No depth testing or writing
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilBothDisabled, 1);
			dsv = nullptr;
		}

		//Special case will be the back buffer
		if (renderTarget.GetType() == ObjectType::BACK_BUFFER)
		{
			ID3D11RenderTargetView* rtv = m_Data->backBuffer.rtv;
			m_Data->deviceContext->OMSetRenderTargets(1, &rtv, dsv);

		}

		//Standard case, bind from render target container
		else
		{
			ID3D11RenderTargetView* rtv = m_Data->renderTargets[*renderTarget].rtv;
			m_Data->deviceContext->OMSetRenderTargets(1, &rtv, dsv);
		}
	}

	void SkyeDX11::BindRenderTargets(unsigned count, const ObjectHandle renderTargets[])
	{
		if (count < 1)
		{
			Warning("Warning: At least one render target is needed. Invalid render target count of: %i render targets", count);
			return;
		}

		//Get the current depth buffer
		ID3D11DepthStencilView* dsv = m_Data->currentDepthBuffer;

		//Setting the depth testing attributes
		if (m_Data->depthTesting && m_Data->depthWriting)
		{
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilBothEnabled, 1);
		}

		else if (m_Data->depthTesting)
		{
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilTestOnly, 1);
		}

		else if (m_Data->depthWriting)
		{
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilWriteOnly, 1);
		}

		else
		{
			//No depth testing or writing
			m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilBothDisabled, 1);
			dsv = nullptr;
		}

		//Allocate render target array
		ID3D11RenderTargetView **rtvArray = reinterpret_cast<ID3D11RenderTargetView**>(_malloca(sizeof(ID3D11RenderTargetView*) * count));

		for (unsigned i = 0; i < count; ++i)
		{
			//Get the current render target handle
			const ObjectHandle& renderTarget = renderTargets[i];

			//If handle is empty, assign null render target
			if (!renderTarget)
			{
				rtvArray[i] = nullptr;
			}

			else
			{
				rtvArray[i] = m_Data->renderTargets[*renderTarget].rtv;
			}
		}

		m_Data->deviceContext->OMSetRenderTargets(count, rtvArray, dsv);
	}

	void SkyeDX11::BindVertexShader(const ObjectHandle& vertexShader)
	{
		if (m_Data->lastVertexShader == vertexShader)
		{
			Warning("Warning: Trying to bind an already bound shader.");
			return;
		}

		m_Data->lastVertexShader = vertexShader;

		if (!vertexShader || vertexShader.GetType() != ObjectType::VERTEX_SHADER)
		{
			Warning("Warning: Trying to bind a null or invalid vertex shader.");
			return;
		}


		const D3DObject::VertexShader& shader = m_Data->vertexShaders[*vertexShader];

		m_Data->deviceContext->VSSetShader(shader.vertexShader, nullptr, 0);
		m_Data->deviceContext->IASetInputLayout(shader.layout);
	}

	void SkyeDX11::BindPixelShader(const ObjectHandle& pixelShader)
	{
		if (m_Data->lastPixelShader == pixelShader)
		{
			Warning("Warning: Trying to bind an already bound shader.");
			return;
		}

		m_Data->lastPixelShader = pixelShader;

		if (!pixelShader || pixelShader.GetType() != ObjectType::PIXEL_SHADER)
		{
			Warning("Warning: Trying to bind a null or invalid pixel shader.");
			return;
		}


		const D3DObject::PixelShader& shader = m_Data->pixelShaders[*pixelShader];

		m_Data->deviceContext->PSSetShader(shader.pixelShader, nullptr, 0);
	}

	void SkyeDX11::BindGeometryShader(const ObjectHandle& geometryShader)
	{
		//If handle is empty, bind an empty shader
		if (!geometryShader)
		{
			m_Data->deviceContext->GSSetShader(nullptr, nullptr, 0);
			return;
		}

		if (geometryShader.GetType() != ObjectType::GEOMETRY_SHADER)
		{
			Warning("Warning: Trying to bind an invalid geometry shader.");
			return;
		}

		const D3DObject::GeometryShader& shader = m_Data->geometryShaders[*geometryShader];

		m_Data->deviceContext->GSSetShader(shader.geometryShader, nullptr, 0);
	}

	void SkyeDX11::BindComputeShader(const ObjectHandle& computeShader)
	{
		//If handle is empty, bind an empty shader
		if (!computeShader)
		{
			m_Data->deviceContext->CSSetShader(nullptr, nullptr, 0);
			return;
		}

		if (computeShader.GetType() != ObjectType::COMPUTE_SHADER)
		{
			Warning("Warning: Trying to bind an invalid compute shader.");
			return;
		}

		const D3DObject::ComputeShader& shader = m_Data->computeShaders[*computeShader];

		m_Data->deviceContext->CSSetShader(shader.computeShader, nullptr, 0);
	}

  void SkyeDX11::BindNullVertexBuffer()
  {
    //Bind NULL buffer - this is used for the particles
    m_Data->deviceContext->IASetVertexBuffers(0, 0, 0, 0, 0);
  }

  void SkyeDX11::BindVertexBuffer(const ObjectHandle& vertexBuffer, unsigned stride)
	{
		if (!vertexBuffer)
		{
			Warning("Warning: Trying to bind a null object.");
			return;
		}

		//Make sure we are binding the right type of buffer
		if (vertexBuffer.GetType() != ObjectType::VERTEX_BUFFER)
		{
			Warning("Warning: Trying to bind an item that is not a vertex buffer.");
			return;
		}

		unsigned offset = 0;

		//Get buffer from container
		ID3D11Buffer* buffer = m_Data->vertexBuffers[*vertexBuffer].buffer;

		//Bind buffer
		m_Data->deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	}

	void SkyeDX11::BindIndexBuffer(const ObjectHandle& indexBuffer)
	{
		if (!indexBuffer)
		{
			Warning("Warning: Trying to bind a null object.");
			return;
		}

		if (indexBuffer.GetType() != ObjectType::INDEX_BUFFER)
		{
			Warning("Warning: Trying to bind an item that is not an index buffer.");
			return;
		}

		unsigned offset = 0;

		ID3D11Buffer* buffer = m_Data->indexBuffers[*indexBuffer].buffer;

		m_Data->deviceContext->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void SkyeDX11::BindConstantBuffer(unsigned slot, const ObjectHandle& constantBuffer, const ObjectType shaderType)
	{
		if (!constantBuffer)
		{
			Warning("Warning: Trying to bind a null object.");
			return;
		}

		if (constantBuffer.GetType() != ObjectType::CONSTANT_BUFFER)
		{
			Warning("Warning: Trying to bind an item that is not a constant buffer.");
			return;
		}

		const D3DObject::Buffer& constBuffer = m_Data->constantBuffers[*constantBuffer];

		ID3D11Buffer* buffer = constBuffer.buffer;

		//Verify which type of shader we are binding the buffer to
		//Then bind buffer to that shader type
		if (shaderType == ObjectType::VERTEX_SHADER)
		{
			m_Data->deviceContext->VSSetConstantBuffers(slot, 1, &buffer);
		}

		else if (shaderType == ObjectType::PIXEL_SHADER)
		{
			m_Data->deviceContext->PSSetConstantBuffers(slot, 1, &buffer);
		}

		else if (shaderType == ObjectType::GEOMETRY_SHADER)
		{
			m_Data->deviceContext->GSSetConstantBuffers(slot, 1, &buffer);
		}

		else if (shaderType == ObjectType::COMPUTE_SHADER)
		{
			m_Data->deviceContext->CSSetConstantBuffers(slot, 1, &buffer);
		}

		else
		{
			Warning("Warning: Trying to bind a constant buffer to an item that is not a shader.");
		}
	}

	void SkyeDX11::BindDepthBuffer(const ObjectHandle& depthBuffer)
	{
		if (!depthBuffer || depthBuffer.GetType() != ObjectType::DEPTH_BUFFER)
		{
			Warning("Warning: Trying to bind a null object or invalid object type.");
			return;
		}

		D3DObject::DepthBuffer& dBuffer = m_Data->depthBuffers[*depthBuffer];

		//Sets the current depth buffer to the buffer that is being bound.
		m_Data->currentDepthBuffer = dBuffer.depthStencilView;
	}


	void SkyeDX11::Release(const ObjectHandle& object)
	{
		//Call the appropriate release method per object type
		switch (object.GetType())
		{
		case DEPTH_BUFFER:
		{
			ReleaseDepthBuffer(object);
			break;
		}
		case TEXTURE:
		{
			ReleaseTexture(object);
			break;
		}
		case RENDER_TARGET:
		{
			ReleaseRenderTarget(object);
			break;
		}

		case VERTEX_SHADER:
		{
			ReleaseVertexShader(object);
			break;
		}
		case PIXEL_SHADER:
		{
			ReleasePixelShader(object);
			break;
		}
		case GEOMETRY_SHADER:
		{
			ReleaseGeometryShader(object);
			break;
		}
		case COMPUTE_SHADER:
		{
			ReleaseComputeShader(object);
			break;
		}
		case VERTEX_BUFFER:
		{
			ReleaseVertexBuffer(object);
			break;
		}
		case INDEX_BUFFER:
		{
			ReleaseIndexBuffer(object);
			break;
		}
		case CONSTANT_BUFFER:
		{
			ReleaseConstantBuffer(object);
			break;
		}

		case NONE: break;

		case BACK_BUFFER: break;

		case SHADER_RESOURCE_VIEW: break;

    case FONT_FACTORY:
      break;
    case FONT_WRAPPER:
      break;

    default: break;
		}

		//Resets the handle to a null object
		const_cast<ObjectHandle&>(object) = ObjectHandle();
	}

	void SkyeDX11::SetViewport(const Viewport& viewport)
	{
		m_Viewport = viewport;

		float halfW = m_Viewport.size.width * 0.5f;
		float halfH = m_Viewport.size.height * 0.5f;

		//Viewport transformation matrix

		// | width/2      0          0      (width/2) + X  |
		// |   0      -height/2      0      (height/2) + Y |
		// |   0          0     maxZ - minZ        0       |
		// |   0          0        minZ            1       |

		//MinZ defaults to 0.0f
		//MaxZ defaults to 1.0f
		m_Viewport.matrix = float4x4(halfW, 0.0f, 0.0f, halfW + m_Viewport.x,
			0.0f, -halfH, 0.0f, halfH + m_Viewport.y,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		CD3D11_VIEWPORT vp(static_cast<FLOAT>(viewport.x), static_cast<FLOAT>(viewport.y),
			static_cast<FLOAT>(viewport.size.width), static_cast<FLOAT>(viewport.size.height),
			static_cast<FLOAT>(viewport.minZ), static_cast<FLOAT>(viewport.maxZ));


		m_Data->deviceContext->RSSetViewports(1, &vp);
	}

	void SkyeDX11::SetPrimitiveTopology(const PrimitiveTopology topology)
	{
		switch (topology)
		{
		case PRIMITIVE_TOPOLOGY_UNDEFINED:
		{
			Warning("Warning: Setting primitive topology to an undefined topology.");
			break;
		}
		case PRIMITIVE_TOPOLOGY_POINTLIST:
		{
			m_Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		}
		case PRIMITIVE_TOPOLOGY_LINELIST:
		{
			m_Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		}
		case PRIMITIVE_TOPOLOGY_LINESTRIP:
		{
			m_Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			break;
		}
		case PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		{
			m_Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		}
		case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		{
			m_Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			break;
		}
		default: break;
		}
	}

	void SkyeDX11::SetBlendMode(const BlendMode mode)
	{
		//Do nothing if blend mode is the current mode
		if (m_Data->blendMode == mode)
			return;

		m_Data->blendMode = mode;

		//DX default values for sample mask and blend factor array
		unsigned sampleMask = 0xFFFFFFFF;

		float blendFactors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		switch (mode)
		{
		case BLEND_MODE_NONE:
		{
			m_Data->deviceContext->OMSetBlendState(NULL, 0, sampleMask);
			break;
		}
		default:
		{
			ID3D11BlendState* blendState = m_Data->blendState[mode];
			m_Data->deviceContext->OMSetBlendState(blendState, blendFactors, sampleMask);
		}
		}
	}

	void SkyeDX11::SetFillMode(const FillMode mode)
	{
		m_Data->fillMode = mode;
		InitializeRasterizerStates();
	}

	void SkyeDX11::SetCullMode(const CullMode mode)
	{
		switch (mode)
		{
			case CULL_MODE_NONE:
			{
				m_Data->deviceContext->RSSetState(m_Data->rsNone);
				break;
			}
			case CULL_MODE_FRONT_FACE:
			{
				m_Data->deviceContext->RSSetState(m_Data->rsFront);
				break;
			}
			case CULL_MODE_BACK_FACE:
			{
				m_Data->deviceContext->RSSetState(m_Data->rsBack);
				break;
			}
			case CULL_MODE_SCISSOR:
			{
				m_Data->deviceContext->RSSetState(m_Data->rsScissor);
				break;
			}
			case CULL_MODE_IMGUI:
			{
				m_Data->deviceContext->RSSetState(m_Data->rsImgui);
				break;
			}
			default: break;
		}
	}

	void SkyeDX11::SetAnisotropy(const Anisotropy anisotropy)
	{
		//If anisotropy is already what it is being set to, do nothing
		if (m_Data->anisotropy == anisotropy)
			return;

		//Set anisotropy and initialize sampler states
		m_Data->anisotropy = anisotropy;

		InitializeSamplerStates();
	}

	void SkyeDX11::SetDepthTesting(bool depthTesting)
	{
		m_Data->depthTesting = depthTesting;
	}

	void SkyeDX11::SetDepthWriting(bool depthWriting)
	{
		m_Data->depthWriting = depthWriting;
	}

	void SkyeDX11::SetScissorRect(unsigned rectCount, SkyeRect* rect)
	{
		m_Data->deviceContext->RSSetScissorRects(rectCount, reinterpret_cast<D3D11_RECT*>(rect));
	}

	void* SkyeDX11::GetDevice() const
	{
		return m_Data->device;
	}
	void* SkyeDX11::GetDeviceContext() const
	{
		return m_Data->deviceContext;
	}
	const Viewport& SkyeDX11::GetViewport(const ObjectHandle & target) const
	{
		if (target)
		{
			if (target.GetType() == ObjectType::BACK_BUFFER)
				return m_Data->backBuffer.viewport;

			return m_Data->renderTargets[*target].viewport;
		}

		return m_Viewport;
	}

  const Area& SkyeDX11::GetTextureSize(const ObjectHandle& texture)
  {
    return m_Data->textures[*texture].size;
  }

	const std::string& SkyeDX11::GetGPUInfo()
	{
		return m_Data->GPUInfo;
	}

	void SkyeDX11::CopyData(const ObjectHandle& object, const void* data, unsigned size)
	{
		//Check for null object
		if (!object)
		{
			Warning("Warning: Atempting to copy data to a null object.");
			return;
		}

		D3DObject::Buffer* buffer = nullptr;

		//Handle copy depending on object type
		switch (object.GetType())
		{
		case TEXTURE:
		{
			D3DObject::Texture& texture = m_Data->textures[*object];

			D3D11_MAPPED_SUBRESOURCE msr;
			ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

			//Map the the texture
			if (FAILED(m_Data->deviceContext->Map(texture.texture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
			{
				ErrorIf(true, "Error: Failed to map data to the texture.");
				return;
			}

			//Copy data onto the mapped texture
			std::memcpy(msr.pData, data, size);

			//Unmap texture
			m_Data->deviceContext->Unmap(texture.texture2D, 0);

			return;
		}

			//get the buffer from appropriate contaianer
		case VERTEX_BUFFER:
		{
			buffer = &m_Data->vertexBuffers[*object];
			break;
		}
		case INDEX_BUFFER:
		{
			buffer = &m_Data->indexBuffers[*object];
			break;
		}
		case CONSTANT_BUFFER:
		{
			buffer = &m_Data->constantBuffers[*object];
			break;
		}
		default: return;
		}

		//Hanlde buffer depending on it's usage
		switch (buffer->usage)
		{
		case USAGE_DEFAULT:
		{
			m_Data->deviceContext->UpdateSubresource(buffer->buffer, 0, nullptr, data, size, 1);
			return;
		}
		case USAGE_IMMUTABLE:
		{
			Warning("Warning: An Immutable buffer cannot be written to.");
			return;
		}
		case USAGE_DYNAMIC:
		{
			//Map, copy data, then unmap dynamic buffer
			D3D11_MAPPED_SUBRESOURCE msr;
			ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

			ErrorHandler(m_Data->deviceContext->Map(buffer->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr), "Error: Failed to map resource.");

			std::memcpy(msr.pData, data, size);

			m_Data->deviceContext->Unmap(buffer->buffer, 0);

			return;
		}
		case USAGE_STAGING: break;
		default: break;
		}
	}

	void SkyeDX11::CopyResource(const ObjectHandle& dest, const ObjectHandle& source)
	{
		//Check that both source and destination are of the same type
		if (dest.GetType() != source.GetType())
		{
			Warning("Warning: Destination and source objecta are of different types.");
			return;
		}

		//Temporary resource pointers
		ID3D11Resource* destResource(nullptr), *sourceResource(nullptr);

		//Get the resources according to object type
		switch (dest.GetType())
		{
		case BACK_BUFFER:
		{
			m_Data->backBuffer.rtv->GetResource(&destResource);
			break;
		}
		case DEPTH_BUFFER:
		{
			m_Data->depthBuffers[*dest].srv->GetResource(&destResource);
			break;
		}
		case TEXTURE:
		{
			m_Data->textures[*dest].srv->GetResource(&destResource);
			break;
		}
		case RENDER_TARGET:
		{
			m_Data->renderTargets[*dest].rtv->GetResource(&destResource);
		}

		default: break;
		}

		switch (source.GetType())
		{
		case BACK_BUFFER:
		{
			m_Data->backBuffer.rtv->GetResource(&sourceResource);
			break;
		}
		case DEPTH_BUFFER:
		{
			m_Data->depthBuffers[*source].srv->GetResource(&sourceResource);
			break;
		}
		case TEXTURE:
		{
			m_Data->textures[*source].srv->GetResource(&sourceResource);
			break;
		}
		case RENDER_TARGET:
		{
			m_Data->renderTargets[*source].rtv->GetResource(&sourceResource);
		}

		default: break;
		}

		//Copy data from source to destination
		m_Data->deviceContext->CopyResource(destResource, sourceResource);

		//Release temp pointers
		destResource->Release();
		sourceResource->Release();
	}

	void* SkyeDX11::Map(const ObjectHandle& buffer)
	{
		ID3D11Buffer* tempBuffer = nullptr;

		//Get the proper buffer depending on handle type
		switch (buffer.GetType())
		{
		case VERTEX_BUFFER:
		{
			tempBuffer = m_Data->vertexBuffers[*buffer].buffer;
			break;
		}
		case INDEX_BUFFER:
		{
			tempBuffer = m_Data->indexBuffers[*buffer].buffer;
			break;
		}
		case CONSTANT_BUFFER:
		{
			tempBuffer = m_Data->constantBuffers[*buffer].buffer;
			break;
		}

		default:
			return nullptr;
		}

		//Map the buffer to a mapped subresource
		D3D11_MAPPED_SUBRESOURCE msr;

		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//If mapping succeeds, return mapped data pointer
		if (!FAILED(m_Data->deviceContext->Map(tempBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
			return msr.pData;

		//else, return nullptr
		return nullptr;
	}

	void* SkyeDX11::Map(const ObjectHandle& texture, unsigned& rowPitch)
	{
		ID3D11Texture2D* tex = nullptr;

		switch (texture.GetType())
		{
			case TEXTURE:
			{
				tex = m_Data->textures[*texture].texture2D;
				break;
			}

			default:
			{
				return nullptr;
			}
		}

		//Map the texture to a mapped subresource
		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

		if (!FAILED(m_Data->deviceContext->Map(tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
		{
			rowPitch = msr.RowPitch;
			return msr.pData;
		}

		return nullptr;

	}

	void SkyeDX11::Unmap(const ObjectHandle& buffer)
	{
		ID3D11Buffer* tempBuffer = nullptr;

		//Get the proper buffer depending on handle type
		switch (buffer.GetType())
		{
			case VERTEX_BUFFER:
			{
				tempBuffer = m_Data->vertexBuffers[*buffer].buffer;
				break;
			}
			case INDEX_BUFFER:
			{
				tempBuffer = m_Data->indexBuffers[*buffer].buffer;
				break;
			}
			case CONSTANT_BUFFER:
			{
				tempBuffer = m_Data->constantBuffers[*buffer].buffer;
				break;
			}

			case TEXTURE:
			{
				ID3D11Texture2D* tex = m_Data->textures[*buffer].texture2D;
				m_Data->deviceContext->Unmap(tex, 0);
				return;
			}

			default:
				return;
		}

		//Unmap the buffer
		m_Data->deviceContext->Unmap(tempBuffer, 0);
	}

	void SkyeDX11::ClearRenderTarget(const ObjectHandle& renderTarget, const float4& color)
	{
		if (!renderTarget || renderTarget.GetType() != ObjectType::RENDER_TARGET)
		{
			Warning("Warning: Trying to clear an object that is either null or not a render target.");
			return;
		}

		ID3D11RenderTargetView* rtv = m_Data->renderTargets[*renderTarget].rtv;

		m_Data->deviceContext->ClearRenderTargetView(rtv, color.v);
	}

	void SkyeDX11::ClearRenderTargets(unsigned count, const ObjectHandle renderTargets[], const float4& color)
	{
		if (count < 1)
		{
			Warning("Warning: At least one render target is needed. Invalid render target count of: %i render targets", count);
			return;
		}


		for (unsigned i = 0; i < count; ++i)
		{
			//Get the current render target handle
			const ObjectHandle& renderTarget = renderTargets[i];

			//If handle is empty, assign null render target
			if (!renderTarget)
			{
				continue;
			}

			else
			{
				m_Data->deviceContext->ClearRenderTargetView(m_Data->renderTargets[*renderTarget].rtv, color.v);
			}
		}

	}

	void SkyeDX11::ClearDepthBuffer(const ObjectHandle& depthBuffer)
	{
		if (!depthBuffer || depthBuffer.GetType() != ObjectType::DEPTH_BUFFER)
		{
			Warning("Warning: Trying to clear an object that is either null or not a depth buffer.");
			return;
		}

		ID3D11DepthStencilView* dsv = m_Data->depthBuffers[*depthBuffer].depthStencilView;

		m_Data->deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, D3D11_MAX_DEPTH, 0);
	}

	void SkyeDX11::ResizeDepthBuffer(const ObjectHandle& depthBuffer, const Area& size)
	{
		const D3DObject::DepthBuffer& db = m_Data->depthBuffers[*depthBuffer];

		CreateDepthBuffer(const_cast<ObjectHandle&>(depthBuffer), db.toResizeWith, size);
	}

	void SkyeDX11::OnResize(WPARAM wParam, LPARAM lParam)
	{
		unsigned width = LOWORD(lParam);
		unsigned height = HIWORD(wParam);

		if (width != 0 && height != 0)
		{
			//Resize all buffers
			ResizeBuffers(width, height);

			//Set newly resized back buffer as the render target
			ID3D11RenderTargetView* backBuffer = m_Data->backBuffer.rtv;

			m_Data->deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);
		}
	}

	void SkyeDX11::OnActivate(WPARAM wParam, LPARAM lParam)
	{
	}

	void SkyeDX11::InternalCreateTexture(ObjectHandle& handle, const std::string& fileName, bool generateMipChain)
	{
		D3DX11_IMAGE_LOAD_INFO imageInfo;

		//Set default flags for image load info
		imageInfo.Width = D3DX11_DEFAULT;
		imageInfo.Height = D3DX11_DEFAULT;
		imageInfo.Depth = D3DX11_DEFAULT;

		if (generateMipChain)
		{
			//Generate mip maps
			imageInfo.FirstMipLevel = D3DX11_DEFAULT;
			imageInfo.MipLevels = D3DX11_DEFAULT;
		}

		else
		{
			//Does not generate mip maps
			imageInfo.FirstMipLevel = 0;
			imageInfo.MipLevels = 1;
		}

		imageInfo.Usage = static_cast<D3D11_USAGE>(D3DX11_DEFAULT);
		imageInfo.BindFlags = D3DX11_DEFAULT;
		imageInfo.CpuAccessFlags = D3DX11_DEFAULT;
		imageInfo.MiscFlags = D3DX11_DEFAULT;
		imageInfo.Format = DXGI_FORMAT_FROM_FILE;
		imageInfo.Filter = D3DX11_DEFAULT;
		imageInfo.MipFilter = D3DX11_DEFAULT;
		imageInfo.pSrcInfo = nullptr;

		//Create the shader resource using the load info
		ShaderResourceViewPtr srv;

		ErrorHandler(D3DX11CreateShaderResourceViewFromFile(m_Data->device, fileName.c_str(), &imageInfo, nullptr, &srv, nullptr),
			"Error: Could not load texture %s", fileName.c_str());

		//Get the SRV resource
		ID3D11Resource* resource;

		srv->GetResource(&resource);

		//Get a pointer to the texture2D interface
		Texture2DPtr textureResource;
		D3D11_TEXTURE2D_DESC texDesc;

		resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&textureResource));

		//Get the texture description from the interface
		textureResource->GetDesc(&texDesc);

		//Resources no longer needed
		resource->Release();
		textureResource.Release();

		//Assign newly created texture to the object handle
		if (handle && handle.GetType() == ObjectType::TEXTURE)
		{
			//If the handle already exists, update the data
			D3DObject::Texture& texture = m_Data->textures[*handle];
			texture.size = Area(texDesc.Width, texDesc.Height);

			if (texture.srv)
				texture.srv.Release();

			texture.srv = srv;
		}

		else
		{
			//Create the handle and assign the data
			D3DObject::Texture texture;
			texture.size = Area(texDesc.Width, texDesc.Height);

			texture.srv = srv;

			int index = NextAvailableIndex(m_Data->textures);

			//No free space in the texture container
			if (index == -1)
			{
				m_Data->textures.push_back(texture);
				handle = CreateHandle(ObjectType::TEXTURE, m_Data->textures.size() - 1);
			}

			//Insert into available slot
			else
			{
				m_Data->textures[index] = texture;
				handle = CreateHandle(ObjectType::TEXTURE, index);
			}
		}
	}

	void SkyeDX11::InternalCreateTexture(ObjectHandle& handle, const void* data, const Area& size, const DataFormat format)
	{
		//If texture already exists, release it safely
		if (handle)
		{
			D3DObject::Texture& texture = m_Data->textures[*handle];
			if (texture.srv)
				texture.srv.Release();
			if (texture.texture2D)
				texture.texture2D.Release();
		}

		//Set up texture description
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = size.width;
		textureDesc.Height = size.height;
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		unsigned stride = 0;

		//Handle the specified format
		switch (format)
		{
		case R_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_R8_UNORM;
			stride = sizeof(unsigned char);
			break;
		}
		case RG_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
			stride = sizeof(unsigned char) * 2;
			break;
		}
		case RGBA_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			stride = sizeof(unsigned char) * 4;
			break;
		}
		case BGRA_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			stride = sizeof(unsigned char) * 4;
			break;
		}
		case FLOAT4:
		{
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			stride = sizeof(float) * 4;
			break;
		}
		case FLOAT3:
		{
			textureDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			stride = sizeof(float) * 3;
			break;
		}
		case FLOAT2:
		{
			textureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			stride = sizeof(float) * 2;
			break;
		}
		case FLOAT1:
		{
			textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
			stride = sizeof(float);
			break;
		}
		default: break;
		}

		//Create texture with description (data is not copied yet)
		Texture2DPtr tex2D;
		HRESULT hr = m_Data->device->CreateTexture2D(&textureDesc, nullptr, &tex2D);

		//Check that there is data to copy.
		if (data)
		{
			//Map the texture to copy data onto it
			D3D11_MAPPED_SUBRESOURCE msr;
			ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

			//Calculate how many bytes will be copied.
			unsigned bytes = stride * size.width * size.height;

			hr = m_Data->deviceContext->Map(tex2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			ErrorIf(FAILED(hr),
				"Error: Failed to map texture data.");

			UCHAR* dest = static_cast<UCHAR*> (msr.pData);
			const UCHAR* source = static_cast<const UCHAR*> (data);

			//Copy data to texture and unmap.
			for (UINT i = 0; i < textureDesc.Height; ++i)
			{
				std::memcpy(dest, source, stride * textureDesc.Width);
				dest += msr.RowPitch;
				source += stride * textureDesc.Width;
			}
			//std::memcpy(msr.pData, data, bytes);
			m_Data->deviceContext->Unmap(tex2D, 0);
		}

		//Create srv with newly created texture
		ShaderResourceViewPtr srv;
		m_Data->device->CreateShaderResourceView(tex2D, nullptr, &srv);

		//If the handle is to an existing texture, update pointers
		if (handle)
		{
			D3DObject::Texture& texture = m_Data->textures[*handle];
			texture.size = size;
			texture.srv = srv;
			texture.texture2D = tex2D;
		}

		//Else, add it to the container
		else
		{
			D3DObject::Texture texture;
			texture.size = size;
			texture.srv = srv;
			texture.texture2D = tex2D;

			int index = NextAvailableIndex(m_Data->textures);

			//There are no free/available slots in the container
			if (index == -1)
			{
				//Create slot
				m_Data->textures.push_back(texture);
				handle = CreateHandle(ObjectType::TEXTURE, m_Data->textures.size() - 1);
			}

			//Use the available slot
			else
			{
				m_Data->textures[index] = texture;
				handle = CreateHandle(ObjectType::TEXTURE, index);
			}
		}
	}

	void SkyeDX11::InternalCreateTextureFromMemory(ObjectHandle& handle, const void* data, unsigned size)
	{
		ShaderResourceViewPtr srv;

		//Create Shader resource from memory, handle error case
    ErrorHandler(D3DX11CreateShaderResourceViewFromMemory(m_Data->device, data, size, 0, 0, &srv, 0),
			"Error: Failed to create texture from memory.");

		ID3D11Resource* resource;
		srv->GetResource(&resource);


		//Get a pointer to the texture2D interface
		Texture2DPtr textureResource;
		D3D11_TEXTURE2D_DESC textureDesc;

		resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&textureResource));

		//Get the texture description from the interface
		textureResource->GetDesc(&textureDesc);

		//Resources no longer needed
		resource->Release();
		textureResource.Release();

		if (handle)
		{
			//Handle already exists. Update handle data
			D3DObject::Texture& texture = m_Data->textures[*handle];

			texture.size = Area(textureDesc.Width, textureDesc.Height);

			if (texture.srv)
				texture.srv.Release();

			texture.srv = srv;
		}

		else
		{
			//Handle does not exist yet. Create it
			D3DObject::Texture texture;
			texture.size = Area(textureDesc.Width, textureDesc.Height);

			texture.srv = srv;

			int index = NextAvailableIndex(m_Data->textures);

			//No free slots in container, add a slot
			if (index == -1)
			{
				m_Data->textures.push_back(texture);
				handle = CreateHandle(ObjectType::TEXTURE, m_Data->textures.size() - 1);
			}

			//Use already available slot
			else
			{
				m_Data->textures[index] = texture;
				handle = CreateHandle(ObjectType::TEXTURE, index);
			}
		}
	}

	void SkyeDX11::InternalCreateRenderTarget(ObjectHandle& handle, const DataFormat format, const Area& size)
	{


		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		//Set up texture description for the desired render target
		textureDesc.Width = WINDOWSSYSTEM->GetScreenWidth();
		textureDesc.Height = WINDOWSSYSTEM->GetScreenHeight();

		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;


		//Handle specified texture format
		switch (format)
		{
		case R_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_R8_UNORM;
			break;
		}
		case RG_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
			break;
		}
		case RGBA_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case BGRA_UNORM:
		{
			textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			break;
		}
		case FLOAT4:
		{
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		case FLOAT3:
		{
			textureDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}
		case FLOAT2:
		{
			textureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		}
		case FLOAT1:
		{
			textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		}
		default: break;
		}

		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Create render target texture using supplied description
		Texture2DPtr texture2D;
		ErrorHandler(m_Data->device->CreateTexture2D(&textureDesc, nullptr, &texture2D),
			"Error: Failed to create render target texture.");

		//Set up render target view description
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		//Create render target view
		RenderTargetViewPtr rtv;
		ErrorHandler(m_Data->device->CreateRenderTargetView(texture2D, &renderTargetViewDesc, &rtv),
			"Error: Failed to create render target view.");

		//Set up shader resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//Create shader resource view
		ShaderResourceViewPtr srv;
		ErrorHandler(m_Data->device->CreateShaderResourceView(texture2D, &shaderResourceViewDesc, &srv),
			"Error: Failed to create shader resource view.");

		//Texture no longer needed since srv and rtv have been created
		texture2D.Release();

		if (handle)
		{
			//Handle already exists 
			D3DObject::RenderTarget& renderTarget = m_Data->renderTargets[*handle];

			//Free memory since handle is to an existing render target
			if (renderTarget.rtv)
				renderTarget.rtv.Release();

			if (renderTarget.srv)
				renderTarget.srv.Release();

			//Update data
			renderTarget.viewport.x = 0;
			renderTarget.viewport.y = 0;
			renderTarget.viewport.minZ = 0.0f;
			renderTarget.viewport.maxZ = 1.0f;
			renderTarget.viewport.size = Area(textureDesc.Width, textureDesc.Height);
			renderTarget.format = format;

			//Assign newly created rtv and srv
			renderTarget.rtv = rtv;
			renderTarget.srv = srv;

		}

		//Handle does not exist yet
		else
		{
			D3DObject::RenderTarget renderTarget;

			//Update data
			renderTarget.viewport.x = 0;
			renderTarget.viewport.y = 0;
			renderTarget.viewport.minZ = 0.0f;
			renderTarget.viewport.maxZ = 1.0f;
			renderTarget.viewport.size = Area(textureDesc.Width, textureDesc.Height);
			renderTarget.format = format;

			//Assign newly created rtv and srv
			renderTarget.rtv = rtv;
			renderTarget.srv = srv;

			int index = NextAvailableIndex(m_Data->renderTargets);


			if (index == -1)
			{
				//Create slot since there is no free space in container
				m_Data->renderTargets.push_back(renderTarget);
				handle = CreateHandle(ObjectType::RENDER_TARGET, m_Data->renderTargets.size() - 1);
			}

			else
			{
				//Use available slot in container
				m_Data->renderTargets[index] = renderTarget;
				handle = CreateHandle(ObjectType::RENDER_TARGET, index);
			}
		}
	}

	static char* LoadShaderBuffer(const std::string &fileName, unsigned &bytes)
	{
		//Set path for file
		std::string file = std::string("Assets/Shaders/") + fileName;

		//Open file for reading in binary form
		std::ifstream shaderFile(file, std::ios::in | std::ios::binary);

		//If file failed to open return nullptr
		if (!shaderFile.is_open())
			return nullptr;

		//find the end position of the file
		shaderFile.seekg(0, std::ios::end);

		//Calculate the amount of bytes will be read in
		bytes = static_cast<unsigned>(shaderFile.tellg());
		shaderFile.seekg(std::ios::beg);

		//Read in the data from the file
		char *data = new char[bytes];

		shaderFile.read(data, bytes);

		return data;
	}

	void SkyeDX11::InternalCreateVertexShader(ObjectHandle& handle, const std::string& sourceFileName, const InputLayout& inputLayout, bool precompiled, const std::string& entryPoint)
	{
		HRESULT result;

		VertexShaderPtr vertexShader;

		BlobPtr blob, error;

		void* code = nullptr;
		unsigned bytes = 0;

		//Create vertex shader, handle differently if precompiled or not
		if (precompiled)
		{
			//Since it is precompiled, just create the vertex shader.
			code = LoadShaderBuffer(sourceFileName, bytes);

			ErrorHandler(m_Data->device->CreateVertexShader(code, bytes, nullptr, &vertexShader),
				"Error: Failed to create vertex shader %s.", sourceFileName.c_str());
		}

		else
		{
			std::string target = "vs_5_0";

			unsigned compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
			compileFlags = D3DCOMPILE_DEBUG;
#endif

			//Compile shader
			result = D3DX11CompileFromFile(sourceFileName.c_str(), nullptr, nullptr,
				entryPoint.c_str(), target.c_str(), compileFlags, 0, nullptr, &blob,
				&error, nullptr);


			if (FAILED(result))
			{
				//Output error message if an error occurred at compile time.
				if (error)
				{
          char* compileErrors;
          unsigned long bufferSize, i;
          std::ofstream fout;

          // Get a pointer to the error message text buffer.
          compileErrors = (char*)(error->GetBufferPointer());

          // Get the length of the message.
          bufferSize = error->GetBufferSize();

          // Open a file to write the error message to.
          fout.open("shader-error.txt");

          // Write out the error message.
          for (i = 0; i < bufferSize; i++)
          {
            fout << compileErrors[i];
          }

          // Close the file.
          fout.close();

          //Temporary
					ErrorIf(true, "%s", reinterpret_cast<char*>(error->GetBufferPointer()));
					error.Release();
				}
			}
			else
			{
				//No error at compile time, so create vertex shader with compiled buffer.


				ErrorHandler(m_Data->device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader),
					"Error: Failed to create vertex shader %s", sourceFileName.c_str());
			}

			code = blob->GetBufferPointer();
			bytes = blob->GetBufferSize();
		}

		//Handle input layout info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		//int lastInputSlot = 0;
		int lastSemanticIndex = 0;
		std::string lastSemanticName = "";

		for (unsigned i = 0; i < inputLayout.size(); ++i)
		{
			const InputData& element = inputLayout[i];

			D3D11_INPUT_ELEMENT_DESC elementDesc;
			ZeroMemory(&elementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));

			//Element name
			elementDesc.SemanticName = element.semanticName.c_str();

			//Calculate byte offset per element in order to remain aligned
			elementDesc.AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;

			//Handle individual element format
			switch (element.format)
			{
			case FLOAT4:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			}
			case FLOAT3:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			}
			case FLOAT2:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			}
			case FLOAT1:
			{
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			}
			case UNSIGNED1:
			{
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
				break;
			}
			case UNSIGNED2:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				break;
			}
			case UNSIGNED3:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			}
			case UNSIGNED4:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			}
			case INT1:
			{
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
				break;
			}
			case INT2:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				break;
			}
			case INT3:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				break;
			}
			case INT4:
			{
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				break;
			}
			default: break;
			}

			//Handle if the element is instanced
			if (element.instanceData)
			{
				elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				elementDesc.InstanceDataStepRate = 1;
				elementDesc.InputSlot = 1;

				if (element.semanticName == lastSemanticName)
				{
					elementDesc.SemanticIndex = ++lastSemanticIndex;
				}
				else
					elementDesc.SemanticIndex = lastSemanticIndex = 0;
			}
			//If not instanced, default values of 0 will be correct 

			lastSemanticName = element.semanticName;
			inputLayoutDesc.push_back(elementDesc);
		}

		//Create input layout with the input element descriptions
		InputLayoutPtr dxInputLayout;

		ErrorHandler(m_Data->device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), code, bytes, &dxInputLayout),
			"Error: Failed to create input layout for shader %s.", sourceFileName.c_str());

		//result = m_Data->device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), code, bytes, &dxInputLayout);

		if (handle)
		{
			//Handle already exists, update data
			D3DObject::VertexShader& vs = m_Data->vertexShaders[*handle];

			//Safely release current data
			if (vs.layout)
				vs.layout.Release();
			if (vs.vertexShader)
				vs.vertexShader.Release();

			//Assign new data
			vs.layout = dxInputLayout;
			vs.vertexShader = vertexShader;
		}
		else
		{
			//Handle doesn't exist yet, create it
			D3DObject::VertexShader vs;

			//Assign new data
			vs.layout = dxInputLayout;
			vs.vertexShader = vertexShader;

			//find where in the container the handle can go
			int index = NextAvailableIndex(m_Data->vertexShaders);

			if (index == -1)
			{
				//No free space in container, push back
				m_Data->vertexShaders.push_back(vs);
				handle = CreateHandle(ObjectType::VERTEX_SHADER, m_Data->vertexShaders.size() - 1);
			}
			else
			{
				//Use the free space for the new handle
				m_Data->vertexShaders[index] = vs;
				handle = CreateHandle(ObjectType::VERTEX_SHADER, index);
			}
		}
	}

	void SkyeDX11::InternalCreatePixelShader(ObjectHandle& handle, const std::string& sourceFileName, bool precompiled, const std::string& entryPoint)
	{
		PixelShaderPtr pixelShader;
		HRESULT result;
		if (precompiled)
		{
			unsigned bytes = 0;
			char* code = LoadShaderBuffer(sourceFileName, bytes);

			ErrorHandler(m_Data->device->CreatePixelShader(code, bytes, nullptr, &pixelShader),
				"Error: Failed to create pixel shader.");

			delete[] code;
		}

		else
		{
			BlobPtr blob, error;
			std::string target = "ps_5_0";

			unsigned compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
			compileFlags |= D3DCOMPILE_DEBUG;
#endif
			//Compile shader
			result = D3DX11CompileFromFile(sourceFileName.c_str(), nullptr, nullptr,
				entryPoint.c_str(), target.c_str(), compileFlags, 0, nullptr, &blob,
				&error, nullptr);

			if (FAILED(result))
			{
				//Output error message if an error occurred at compile time.
				if (error)
				{
          char* compileErrors;
          unsigned long bufferSize, i;
          std::ofstream fout;

          // Get a pointer to the error message text buffer.
          compileErrors = (char*)(error->GetBufferPointer());

          // Get the length of the message.
          bufferSize = error->GetBufferSize();

          // Open a file to write the error message to.
          fout.open("shader-error.txt");

          // Write out the error message.
          for (i = 0; i < bufferSize; i++)
          {
            fout << compileErrors[i];
          }

          // Close the file.
          fout.close();

					ErrorIf(true, "%s", reinterpret_cast<char*>(error->GetBufferPointer()));
					error.Release();
				}
			}

			else
			{
				//No error at compile time, so create pixel shader with compiled buffer.
				ErrorHandler(m_Data->device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader),
					"Error: Failed to create pixel shader %s", sourceFileName.c_str());
			}
		}

		//If handle already exists, update data
		if (handle)
		{
			D3DObject::PixelShader& ps = m_Data->pixelShaders[*handle];
			if (ps.pixelShader)
				ps.pixelShader.Release();

			ps.pixelShader = pixelShader;
		}

		//Else create handle
		else
		{
			D3DObject::PixelShader ps;
			ps.pixelShader = pixelShader;

			int index = NextAvailableIndex(m_Data->pixelShaders);

			if (index == -1)
			{
				//No free space exists in container, push back
				m_Data->pixelShaders.push_back(ps);
				handle = CreateHandle(ObjectType::PIXEL_SHADER, m_Data->pixelShaders.size() - 1);
			}

			else
			{
				//Use available space in container
				m_Data->pixelShaders[index] = ps;
				handle = CreateHandle(ObjectType::PIXEL_SHADER, index);
			}
		}
	}

	void SkyeDX11::InternalCreateGeometryShader(ObjectHandle& handle, const std::string& sourceFileName, bool precompiled, const std::string& entryPoint)
	{
		HRESULT result;
		GeometryShaderPtr geometryShader;

		if (precompiled)
		{
			unsigned bytes = 0;
			char* code = LoadShaderBuffer(sourceFileName, bytes);

			ErrorHandler(m_Data->device->CreateGeometryShader(code, bytes, nullptr, &geometryShader),
				"Error: Failed to create \"%s\" geometry shader from precompiled data.", sourceFileName.c_str());

			delete[] code;
		}

		else
		{
			BlobPtr blob, error;
			std::string target = "gs_5_0";

			unsigned compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
			compileFlags |= D3DCOMPILE_DEBUG;
#endif
			//Compile shader
			result = D3DX11CompileFromFile(sourceFileName.c_str(), nullptr, nullptr,
				entryPoint.c_str(), target.c_str(), compileFlags, 0, nullptr, &blob,
				&error, nullptr);

			if (FAILED(result))
			{
				//Output error message if an error occurred at compile time.
				if (error)
				{
					ErrorIf(true, "%s", reinterpret_cast<char*>(error->GetBufferPointer()));
					error.Release();
				}
			}

			else
			{
				//No error at compile time, so create pixel shader with compiled buffer.
				ErrorHandler(m_Data->device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &geometryShader),
					"Error: Failed to create geometry shader %s", sourceFileName.c_str());
			}
		}

		if (handle)
		{
			//Since the handle already exists, update data
			D3DObject::GeometryShader& gs = m_Data->geometryShaders[*handle];

			if (gs.geometryShader)
				gs.geometryShader.Release();

			gs.geometryShader = geometryShader;
		}

		else
		{
			//Handle needs to be created.

			D3DObject::GeometryShader gs;

			gs.geometryShader = geometryShader;

			int index = NextAvailableIndex(m_Data->geometryShaders);

			if (index == -1)
			{
				//No available space in container, push back
				m_Data->geometryShaders.push_back(gs);
				handle = CreateHandle(ObjectType::GEOMETRY_SHADER, m_Data->geometryShaders.size() - 1);
			}

			else
			{
				//Use available space in container
				m_Data->geometryShaders[index] = gs;
				handle = CreateHandle(ObjectType::GEOMETRY_SHADER, index);
			}
		}
	}

	void SkyeDX11::InternalCreateVertexBuffer(ObjectHandle& handle, const BufferUsage usage, unsigned size, const void* initialData)
	{
		//Create zeroed out buffer description
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		//Establish description usage
		switch (usage)
		{
		case USAGE_DEFAULT:
		{
			desc.Usage = D3D11_USAGE_DEFAULT;
			break;
		}
		case USAGE_IMMUTABLE:
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			break;
		}
		case USAGE_DYNAMIC:
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		}
		case USAGE_STAGING:
		{
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			break;
		}
		default: break;
		}

		//Specify buffer description info
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//Create zeroed out subresource data
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));


		//Vertex3D vertices[] =
		//{
		//	Vertex3D(0.0f, 0.5f, 0.5f),
		//	Vertex3D(0.5f, -0.5f, 0.5f),
		//	Vertex3D(-0.5f, -0.5f, 0.0f),
		//};
		//
		//srd.pSysMem = vertices;
		srd.pSysMem = initialData;

		BufferPtr vBuffer;

		//Create buffer, if no initial data, it creates an empty buffer.
		//HRESULT result = m_Data->device->CreateBuffer(&desc, (vertices) ? &srd : nullptr, &vBuffer);

		//Create buffer, if no initial data, it creates an empty buffer.
		HRESULT result = m_Data->device->CreateBuffer(&desc, (initialData) ? &srd : nullptr, &vBuffer);


		//Handle the possibility of failed creation.
		ErrorHandler(result, "Error: Failed to create vertex buffer of size %i", size);

		if (handle)
		{
			//If handle already exists, release preexisting buffer and update data.
			D3DObject::Buffer& vertexBuffer = m_Data->vertexBuffers[*handle];

			if (vertexBuffer.buffer)
				vertexBuffer.buffer.Release();

			vertexBuffer.size = size;
			vertexBuffer.buffer = vBuffer;
			vertexBuffer.usage = usage;
		}

		else
		{
			//Handle needs to be created
			D3DObject::Buffer vertexBuffer;

			vertexBuffer.size = size;
			vertexBuffer.buffer = vBuffer;
			vertexBuffer.usage = usage;

			int index = NextAvailableIndex(m_Data->vertexBuffers);

			//If no available space in container, push it back
			if (index == -1)
			{
				m_Data->vertexBuffers.push_back(vertexBuffer);
				handle = CreateHandle(ObjectType::VERTEX_BUFFER, m_Data->vertexBuffers.size() - 1);
			}

			else
			{
				//Use the available free space in the container
				m_Data->vertexBuffers[index] = vertexBuffer;
				handle = CreateHandle(ObjectType::VERTEX_BUFFER, index);
			}
		}

	}

	void SkyeDX11::InternalCreateIndexBuffer(ObjectHandle& handle, const BufferUsage usage, unsigned size, const void* initialData)
	{
		//Create zeroed out buffer description
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		//Specify correct buffer usage
		switch (usage)
		{
		case USAGE_DEFAULT:
		{
			desc.Usage = D3D11_USAGE_DEFAULT;
			break;
		}
		case USAGE_IMMUTABLE:
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			break;
		}
		case USAGE_DYNAMIC:
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		}
		case USAGE_STAGING:
		{
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			break;
		}
		default: break;
		}

		//Specify buffer description data
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		//Zeroed out subresource data
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));
		srd.pSysMem = initialData;

		BufferPtr iBuffer;

		ErrorHandler(m_Data->device->CreateBuffer(&desc, (initialData) ? &srd : nullptr, &iBuffer),
			"Error: Failed to create index buffer of size %i", size);

		if (handle)
		{
			D3DObject::Buffer& indexBuffer = m_Data->indexBuffers[*handle];

			//Release memory if it is being used
			if (indexBuffer.buffer)
				indexBuffer.buffer.Release();

			//Update data
			indexBuffer.buffer = iBuffer;
			indexBuffer.usage = usage;
			indexBuffer.size = size;
		}

		else
		{
			D3DObject::Buffer indexBuffer;

			//Assign data
			indexBuffer.buffer = iBuffer;
			indexBuffer.usage = usage;
			indexBuffer.size = size;

			int index = NextAvailableIndex(m_Data->indexBuffers);

			if (index == -1)
			{
				//No free space in container, append object and create handle
				m_Data->indexBuffers.push_back(indexBuffer);
				handle = CreateHandle(ObjectType::INDEX_BUFFER, m_Data->indexBuffers.size() - 1);
			}

			else
			{
				//Use free space in the container and create handle
				m_Data->indexBuffers[index] = indexBuffer;
				handle = CreateHandle(ObjectType::INDEX_BUFFER, index);
			}
		}
	}

	unsigned ClosestMemoryAlignment(unsigned alignmentBy, unsigned currentSize)
	{
		unsigned desiredTarget = 0;

		while (desiredTarget < currentSize)
		{
			desiredTarget += alignmentBy;
		}

		return desiredTarget;
	}

	void SkyeDX11::InternalCreateConstantBuffer(ObjectHandle& handle, unsigned size)
	{
		//Create zeroed out buffer description
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		//Setup description
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		//Need to ensure that memory is aligned to 16 bytes.
		desc.ByteWidth = ((size % 16) == 0) ? size : ClosestMemoryAlignment(16, size);

		//Create Buffer
		BufferPtr buffer;

		ErrorHandler(m_Data->device->CreateBuffer(&desc, nullptr, &buffer),
			"Error: Failed to create constant buffer of %d bytes", size);

		if (handle)
		{
			D3DObject::Buffer& constBuffer = m_Data->constantBuffers[*handle];

			//Free buffer if it has data
			if (constBuffer.buffer)
				constBuffer.buffer.Release();

			//Update data
			constBuffer.buffer = buffer;
			constBuffer.usage = BufferUsage::USAGE_DYNAMIC;
			constBuffer.size = size;
		}

		else
		{
			D3DObject::Buffer constBuffer;

			//Update data
			constBuffer.buffer = buffer;
			constBuffer.usage = BufferUsage::USAGE_DYNAMIC;
			constBuffer.size = size;

			int index = NextAvailableIndex(m_Data->constantBuffers);

			if (index == -1)
			{
				//No free space exists in container, push back
				m_Data->constantBuffers.push_back(constBuffer);
				handle = CreateHandle(ObjectType::CONSTANT_BUFFER, m_Data->constantBuffers.size() - 1);
			}

			else
			{
				//Use available space in container
				m_Data->constantBuffers[index] = constBuffer;
				handle = CreateHandle(ObjectType::CONSTANT_BUFFER, index);
			}
		}
	}

	void SkyeDX11::InternalCreateDepthBuffer(ObjectHandle& handle, const ObjectHandle& toResizeWith, const Area& size)
	{
		D3D11_TEXTURE2D_DESC dbDesc;
		ZeroMemory(&dbDesc, sizeof(D3D11_TEXTURE2D_DESC));

		Area screenSize;

		//If we are resizing to back buffer, take the vieport size
		if (toResizeWith.GetType() == ObjectType::BACK_BUFFER || !toResizeWith)
			screenSize = m_Data->backBuffer.viewport.size;

		//Else if it is a render target, take it's size from the container
		else if (toResizeWith.GetType() == ObjectType::RENDER_TARGET)
			screenSize = m_Data->renderTargets[*toResizeWith].viewport.size;

		//Default size (usually 0,0)
		else
			screenSize = size;

		//Set up depth buffer description
		dbDesc.Width = screenSize.width;
		dbDesc.Height = screenSize.height;
		dbDesc.ArraySize = 1;
		dbDesc.MipLevels = 1;

		//This specifies no MSAA
		dbDesc.SampleDesc.Count = 1;
		dbDesc.SampleDesc.Quality = 0;

		dbDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		dbDesc.Usage = D3D11_USAGE_DEFAULT;
		dbDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		Texture2DPtr depthBufferTexture = nullptr;

		//Create depth buffer using description provided
		ErrorHandler(m_Data->device->CreateTexture2D(&dbDesc, nullptr, &depthBufferTexture),
			"Error: Failed to create depth buffer.");

		//HRESULT hr = m_Data->device->CreateTexture2D(&dbDesc, nullptr, &depthBufferTexture);

		//The depth-stencil state tells the output-merger stage how to perform the 
		//depth-stencil test. The depth-stencil test determines whether or not a 
		//given pixel should be drawn.
		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		ID3D11DepthStencilState * pDSState;
		m_Data->device->CreateDepthStencilState(&dsDesc, &pDSState);

		// Bind depth stencil state
		m_Data->deviceContext->OMSetDepthStencilState(pDSState, 1);

		//Bind the depth-stencil resource using a view.
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		// Create the depth stencil view
		DepthStencilViewPtr pDSV;
		ErrorHandler(m_Data->device->CreateDepthStencilView(depthBufferTexture, &descDSV, &pDSV),
			"Error: Failed to create depth stencil view.");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		//Set up shader resource view description
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		//No mip mapping
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		//Create the shader resource view;
		ShaderResourceViewPtr srv;

		ErrorHandler(m_Data->device->CreateShaderResourceView(depthBufferTexture, &srvDesc, &srv),
			"Error: Failed to create the shader resource view for the depth buffer.");

		//Depth buffer no longer needed.
		depthBufferTexture.Release();

		if (handle)
		{
			D3DObject::DepthBuffer& depthBuffer = m_Data->depthBuffers[*handle];

			//Free up memory of the current buffer
			if (depthBuffer.depthStencilView)
				depthBuffer.depthStencilView.Release();

			if (depthBuffer.srv)
				depthBuffer.srv.Release();

			//Update data
			depthBuffer.srv = srv;
			depthBuffer.depthStencilView = pDSV;
			depthBuffer.size = screenSize;
			depthBuffer.toResizeWith = toResizeWith ? toResizeWith : m_BackBuffer;
		}

		else
		{

			D3DObject::DepthBuffer depthBuffer;

			//Update data
			depthBuffer.srv = srv;
			depthBuffer.depthStencilView = pDSV;
			depthBuffer.size = screenSize;
			depthBuffer.toResizeWith = toResizeWith ? toResizeWith : m_BackBuffer;

			int index = NextAvailableIndex(m_Data->depthBuffers);

			if (index == -1)
			{
				//No free space in container, push object back
				m_Data->depthBuffers.push_back(depthBuffer);
				handle = CreateHandle(ObjectType::DEPTH_BUFFER, m_Data->depthBuffers.size() - 1);
			}

			else
			{
				//Use free space already in container
				m_Data->depthBuffers[index] = depthBuffer;
				handle = CreateHandle(ObjectType::DEPTH_BUFFER, index);
			}
		}
	}

  void SkyeDX11::InternalCreateFontWrapper(ObjectHandle& handle, const std::string& fontName)
  {
    if (!handle)
    {
      int index = NextAvailableIndex(m_Data->fontWrappers);
      std::wstring ws;
      ws.assign(fontName.begin(), fontName.end());

      if (index == -1)
      {
        D3DObject::FontWrapper font;
        ErrorHandler(m_Data->fontFactory->CreateFontWrapper(m_Data->device, ws.c_str(), &font.fontWrapper));
        m_Data->fontWrappers.push_back(font);
      }
      else
      {
        ErrorHandler(m_Data->fontFactory->CreateFontWrapper(m_Data->device, ws.c_str(), &m_Data->fontWrappers[handle].fontWrapper));
      }
    }    
  }

  void SkyeDX11::ReleaseTexture(const ObjectHandle& texture)
	{
		//Make sure object hasn't been released yet
		if (!texture)
		{
			Warning("Warning: texture has already been released.");
			return;
		}

		D3DObject::Texture& tex = m_Data->textures[*texture];

		//Release data
		if (tex.srv)
			tex.srv.Release();

		tex.deleted = true;
	}

	void SkyeDX11::ReleaseRenderTarget(const ObjectHandle& renderTarget)
	{
		//Make sure object hasn't been released yet
		if (!renderTarget)
		{
			Warning("Warning: Render target has already been released.");
			return;
		}

		D3DObject::RenderTarget& rTarget = m_Data->renderTargets[*renderTarget];

		//Release data
		if (rTarget.rtv)
			rTarget.rtv.Release();

		if (rTarget.srv)
			rTarget.srv.Release();

		rTarget.deleted = true;

	}

	void SkyeDX11::ReleaseVertexShader(const ObjectHandle& vertexShader)
	{
		if (!vertexShader)
		{
			Warning("Warning: Vertex shader has already been released.");
			return;
		}

		D3DObject::VertexShader& vs = m_Data->vertexShaders[*vertexShader];

		if (vs.layout)
			vs.layout.Release();

		if (vs.vertexShader)
			vs.vertexShader.Release();

		vs.deleted = true;
	}

	void SkyeDX11::ReleasePixelShader(const ObjectHandle& pixelShader)
	{
		if (!pixelShader)
		{
			Warning("Warning: Pixel shader has already been released.");
			return;
		}

		D3DObject::PixelShader& ps = m_Data->pixelShaders[*pixelShader];

		if (ps.pixelShader)
			ps.pixelShader.Release();

		ps.deleted = true;
	}

	void SkyeDX11::ReleaseGeometryShader(const ObjectHandle& geometryShader)
	{
		if (!geometryShader)
		{
			Warning("Warning: Geometry shader has aready been released.");
			return;
		}

		D3DObject::GeometryShader& gs = m_Data->geometryShaders[*geometryShader];

		if (gs.geometryShader)
			gs.geometryShader.Release();

		gs.deleted = true;
	}

	void SkyeDX11::ReleaseComputeShader(const ObjectHandle& computeShader)
	{
		if (!computeShader)
		{
			Warning("Warning: Compute shader has already been released.");
			return;
		}

		D3DObject::ComputeShader& cs = m_Data->computeShaders[*computeShader];

		if (cs.computeShader)
			cs.computeShader.Release();

		cs.deleted = true;
	}

	void SkyeDX11::ReleaseVertexBuffer(const ObjectHandle& vertexBuffer)
	{
		if (!vertexBuffer)
		{
			Warning("Warning: Vertex buffer has already been released.");
			return;
		}

		D3DObject::Buffer& buffer = m_Data->vertexBuffers[*vertexBuffer];

		if (buffer.buffer)
			buffer.buffer.Release();

		buffer.deleted = true;
	}

	void SkyeDX11::ReleaseIndexBuffer(const ObjectHandle& indexBuffer)
	{
		if (!indexBuffer)
		{
			Warning("Warning: Index buffer has already been released.");
			return;
		}

		D3DObject::Buffer& buffer = m_Data->indexBuffers[*indexBuffer];

		if (buffer.buffer)
			buffer.buffer.Release();

		buffer.deleted = true;
	}

	void SkyeDX11::ReleaseConstantBuffer(const ObjectHandle& constantBuffer)
	{
		if (!constantBuffer)
		{
			Warning("Warning: Constant buffer has already been released.");
			return;
		}

		D3DObject::Buffer& buffer = m_Data->constantBuffers[*constantBuffer];

		if (buffer.buffer)
			buffer.buffer.Release();

		buffer.deleted = true;
	}

	void SkyeDX11::ReleaseDepthBuffer(const ObjectHandle& depthBuffer)
	{
		if (!depthBuffer)
		{
			Warning("Warning: Depth buffer has already been released.");
			return;
		}

		D3DObject::DepthBuffer& db = m_Data->depthBuffers[*depthBuffer];

		if (db.depthStencilView)
			db.depthStencilView.Release();

		if (db.srv)
			db.srv.Release();

		db.deleted = true;
	}

	void SkyeDX11::InitializeDXGI()
	{
		//Create factory
		ErrorHandler(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_Data->factory)),
			"Error: Failed to create DXGI Factory.");

		//Enumerate adapters
		ErrorHandler(m_Data->factory->EnumAdapters(0, &m_Data->adapter),
			"Error: Failed to enumerate adapters available in the system.");

		//Enumerate video card outputs
		ErrorHandler(m_Data->adapter->EnumOutputs(0, &m_Data->output),
			"Error: Failed to enumerate video card outputs.");


		//GET VIDEO CARD INFO 
		//DXGI_ADAPTER_DESC adapterDesc;
		//ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));
		//
		//if (SUCCEEDED(m_Data->adapter->GetDesc(&adapterDesc)))
		//{
		//	unsigned vMemory = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		//}

	}

	void SkyeDX11::InitializeDevice()
	{
		std::vector<D3D_FEATURE_LEVEL> featureLevels;

		//Start from the desired feature level and go to lower feature levels
		for (int i = m_Data->lowestFeatureLevel; i >= 0; --i)
		{
			//Append all feature levels below the desired feature levels
			switch (static_cast<FeatureLevel>(i))
			{
			case FEATURE_LEVEL_DX9_1:
			{
				featureLevels.push_back(D3D_FEATURE_LEVEL_9_1);
				break;
			}
			case FEATURE_LEVEL_DX9_2:
			{
				featureLevels.push_back(D3D_FEATURE_LEVEL_9_2);
				break;
			}
			case FEATURE_LEVEL_DX9_3:
			{
				featureLevels.push_back(D3D_FEATURE_LEVEL_9_3);
				break;
			}
			case FEATURE_LEVEL_DX10_0:
			{
				featureLevels.push_back(D3D_FEATURE_LEVEL_10_0);
				break;
			}
			case FEATURE_LEVEL_DX10_1:
			{
				featureLevels.push_back(D3D_FEATURE_LEVEL_10_1);
				break;
			}
			case FEATURE_LEVEL_DX11_0:
			{
				featureLevels.push_back(D3D_FEATURE_LEVEL_11_0);
				break;
			}
			case FEATURE_LEVEL_DX11_1:
			{
				//Current header file does not have this level in the enum
				//featureLevels.push_back(D3D_FEATURE_LEVEL_11_1);
				break;
			}
			case FEATURE_LEVEL_DX12_0:
			{
				//Current header file does not have this level in the enum
				//featureLevels.push_back(D3D_FEATURE_LEVEL_12_0);
				break;
			}
			case FEATURE_LEVEL_DX12_1:
			{
				//Current header file does not have this level in the enum
				//featureLevels.push_back(D3D_FEATURE_LEVEL_12_1);
				break;
			}
			default: break;
			}
		}

		unsigned createFlags = 0;

#if defined(DEBUG) ||(_DEBUG)
		//createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL trueFeatureLevel;

		//Create D3D11 device using the desired feature level
		//ErrorHandler(D3D11CreateDevice(m_Data->adapter,         //D3D adapter to use 
		//	D3D_DRIVER_TYPE_HARDWARE, //Driver type to create
		//	nullptr,					//A handle to a DLL that implements a software rasterizer
		//	createFlags,				//Runtime layers to create
		//	&featureLevels[0],		//Array of supported feature levels
		//	featureLevels.size(),	//Size of feature level array
		//	D3D11_SDK_VERSION,		//SDK version for directX
		//	&m_Data->device,			//Pointer to the device to be created
		//	&trueFeatureLevel,		//First feature level in the array
		//	&m_Data->deviceContext), //Pointer to the device context to be created
		//	"Error: Failed to create D3D Device.");

		ErrorHandler(D3D11CreateDevice(nullptr,         //D3D adapter to use 
			D3D_DRIVER_TYPE_HARDWARE, //Driver type to create
			nullptr,					//A handle to a DLL that implements a software rasterizer
			createFlags,				//Runtime layers to create
			&featureLevels[0],		//Array of supported feature levels
			featureLevels.size(),	//Size of feature level array
			D3D11_SDK_VERSION,		//SDK version for directX
			&m_Data->device,			//Pointer to the device to be created
			&trueFeatureLevel,		//First feature level in the array
			&m_Data->deviceContext),
			"Error: Failed to create D3D Device.");

		ErrorIf(trueFeatureLevel != D3D_FEATURE_LEVEL_11_0, "DirectX 11 not supported.");

	}

	void SkyeDX11::InitializeSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		// To correctly create the swap chain, we must use the IDXGIFactory that was
		// used to create the device.  If we tried to use a different IDXGIFactory instance
		// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
		// This function is being called with a device from a different IDXGIFactory."
		IDXGIDevice * pDXGIDevice;
		HRESULT hr = m_Data->device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
		ErrorIf(hr != S_OK, "Failed To get IDXGIDevice");

		IDXGIAdapter * pDXGIAdapter;
		hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
		ErrorIf(hr != S_OK, "Failed To get IDXGIAdapter");

		IDXGIFactory * pIDXGIFactory;
		hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
		ErrorIf(hr != S_OK, "Failed To get IDXGIFactory");

		//Set up the description for the swap chain
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = WINDOWSSYSTEM->GetScreenWidth();
		swapChainDesc.BufferDesc.Height = WINDOWSSYSTEM->GetScreenHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.OutputWindow = m_hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//Create swap chain.
		ErrorHandler(pIDXGIFactory->CreateSwapChain(m_Data->device, &swapChainDesc, &m_Data->swapChain),
			"Error: Failed to create the swap chain. Make sure that a target window was specified.");

		//Assosiate the window to the DXGI
		ErrorHandler(pIDXGIFactory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER),
			"Error: Failed to assosiate window.");
	}

	void SkyeDX11::InitializeBackBuffer()
	{
		D3DObject::RenderTarget& backBuffer = m_Data->backBuffer;

		//Set the back buffer viewport depending on screen size resolution
		backBuffer.viewport.SetViewport(0, 0, Area(WINDOWSSYSTEM->GetScreenWidth(), WINDOWSSYSTEM->GetScreenHeight()));

		Texture2DPtr backBufferTexture;

		//Get the back buffer texture
		ErrorHandler(m_Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture)),
			"Error: Failed to get the back buffer texture.");

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		rtvDesc.Format = DX11Data::RENDER_TARGET_FORMAT;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//Create render target for back buffer
		ErrorHandler(m_Data->device->CreateRenderTargetView(backBufferTexture, &rtvDesc, &backBuffer.rtv),
			"Error: Failed to create back buffer render target view.");

		//HRESULT hr = m_Data->device->CreateRenderTargetView(backBufferTexture, &rtvDesc, &backBuffer.rtv);

		//Set render target
		ID3D11RenderTargetView* rtv = backBuffer.rtv;
		m_Data->deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
	}

	void SkyeDX11::InitializeDepthStencilStates()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		//Depth testing info
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		//read write masks for the stencil
		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		//front face info
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//back face info
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//Create depth stencil with testing and writing enabled
		ErrorHandler(m_Data->device->CreateDepthStencilState(&depthStencilDesc, &m_Data->depthStencilBothEnabled),
			"Error: Failed to create test and write depth stencil state.");

		//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		//Create depth stencil with only testing enabled
		ErrorHandler(m_Data->device->CreateDepthStencilState(&depthStencilDesc, &m_Data->depthStencilTestOnly),
			"Error: Failed to create test only depth stencil state.");

		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;

		ErrorHandler(m_Data->device->CreateDepthStencilState(&depthStencilDesc, &m_Data->depthStencilBothDisabled),
			"Error: Failed to create disabled depth stencil state.");

		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		ErrorHandler(m_Data->device->CreateDepthStencilState(&depthStencilDesc, &m_Data->depthStencilWriteOnly),
			"Error: Failed to create write only depth stencil state.");

		m_Data->deviceContext->OMSetDepthStencilState(m_Data->depthStencilBothDisabled, 1);
	}

	void SkyeDX11::InitializeRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		//Set the rasterizer description
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.FillMode = (m_Data->fillMode == FillMode::FILL_SOLID) ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

		//Depth testing info
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 1.0f;
		rasterizerDesc.DepthClipEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 1.0f;

		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;

		//Rect scissor enabled
		rasterizerDesc.ScissorEnable = TRUE;

		m_Data->deviceContext->RSSetState(nullptr);

		if (m_Data->rsScissor)
			m_Data->rsScissor.Release();

		//Create rect scissor rasterizer state
		ErrorHandler(m_Data->device->CreateRasterizerState(&rasterizerDesc, &m_Data->rsScissor),
			"Error: Failed to create rect scissor rasterizer state.");

		rasterizerDesc.ScissorEnable = FALSE;

		if (m_Data->rsNone)
			m_Data->rsNone.Release();

		//Create rasterizer state with no culling
		ErrorHandler(m_Data->device->CreateRasterizerState(&rasterizerDesc, &m_Data->rsNone),
			"Error: Failed to create non culling rasterizer state.");

		rasterizerDesc.CullMode = D3D11_CULL_FRONT;

		if (m_Data->rsFront)
			m_Data->rsFront.Release();

		//Create front face culling rasterizer state
		ErrorHandler(m_Data->device->CreateRasterizerState(&rasterizerDesc, &m_Data->rsFront),
			"Error: Failed to create front face culling rasterizer state.");

		rasterizerDesc.CullMode = D3D11_CULL_BACK;

		if (m_Data->rsBack)
			m_Data->rsBack.Release();

		//Create back face culling rasterizer state
		ErrorHandler(m_Data->device->CreateRasterizerState(&rasterizerDesc, &m_Data->rsBack),
			"Error: Failed to create back face culling rasterizer state.");

		//Back face culling is set by default
		m_Data->deviceContext->RSSetState(m_Data->rsBack);

		//IMGUI RASTERIZER STATE************************************************************
		if (m_Data->rsImgui)
			m_Data->rsImgui.Release();

		DXGI_SWAP_CHAIN_DESC sd;
		m_Data->swapChain->GetDesc(&sd);
		//Imgui rasterizer description
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.ScissorEnable = TRUE;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = (sd.SampleDesc.Count > 1) ? TRUE : FALSE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;

		//Create back imgui rasterizer state
		ErrorHandler(m_Data->device->CreateRasterizerState(&rasterizerDesc, &m_Data->rsImgui),
			"Error: Failed to create Imgui rasterizer state.");

		//END IMGUI RASTERIZER STATE***********************************************************
	}

	void SkyeDX11::InitializeSamplerStates()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		//Set up sampler description info
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MipLODBias = 0.0f;

		//Set up max anisotropy based on the data setup
		switch (m_Data->anisotropy)
		{
		case ANISOTROPY_1X:
		{
			samplerDesc.MaxAnisotropy = 1;
			break;
		}
		case ANISOTROPY_2X:
		{
			samplerDesc.MaxAnisotropy = 2;
			break;
		}
		case ANISOTROPY_4X:
		{
			samplerDesc.MaxAnisotropy = 4;
			break;
		}
		case ANISOTROPY_8X:
		{
			samplerDesc.MaxAnisotropy = 8;
			break;
		}
		case ANISOTROPY_16X:
		{
			samplerDesc.MaxAnisotropy = 16;
			break;
		}
		default:
		{
			samplerDesc.MaxAnisotropy = 1;
			break;
		}
		}

		//Set up clapmped texture addressing
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		//Create point filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateClamp[0]),
			"Error: Failed to create clamped point filtering sampler state.");

		//Create bilinear filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateClamp[1]),
			"Error: Failed to create bilinear filtering sampler state.");

		//Create trilinear filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateClamp[2]),
			"Error: Failed to create trilinear filtering sampler state");

		//Create anisotropic filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateClamp[3]),
			"Error: Failed to create anisotropic filtering sampler state.");

		//Set up wrapping texture addressing
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		//Create point filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateWrap[0]),
			"Error: Failed to create clamped point filtering sampler state.");

		//Create bilinear filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateWrap[1]),
			"Error: Failed to create bilinear filtering sampler state.");

		//Create trilinear filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateWrap[2]),
			"Error: Failed to create trilinear filtering sampler state");

		//Create anisotropic filtering sampler state
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

		ErrorHandler(m_Data->device->CreateSamplerState(&samplerDesc, &m_Data->samplerStateWrap[3]),
			"Error: Failed to create anisotropic filtering sampler state.");

		//Create array with the sampler states that were just created
		ID3D11SamplerState* samplerStates[8] =
		{
			m_Data->samplerStateClamp[0],   //Clamped point filtering
			m_Data->samplerStateClamp[1],	//Clamped bilinear filtering
			m_Data->samplerStateClamp[2],	//Clamped trilinear filtering
			m_Data->samplerStateClamp[3],	//Clamped anisotropic filtering
			m_Data->samplerStateWrap[0],	//Wrapped point filtering
			m_Data->samplerStateWrap[1],	//Wrapped bilinear filtering
			m_Data->samplerStateWrap[2],	//Wrapped trilinear filtering
			m_Data->samplerStateWrap[3],	//Wrapped anisotropic filtering

		};

		//Set the sampler states
		m_Data->deviceContext->PSSetSamplers(0, 8, samplerStates);

	}

	void SkyeDX11::InitializeBlendStates()
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		//Info used by all blend states
		blendDesc.IndependentBlendEnable = TRUE;
		blendDesc.AlphaToCoverageEnable = FALSE;

		//Alpha blend state info
		for (unsigned i = 0; i < MAX_BIND_SLOTS; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = i == 0 ? TRUE : FALSE;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		//Create alpha blend state
		ErrorHandler(m_Data->device->CreateBlendState(&blendDesc, &m_Data->blendState[BlendMode::BLEND_MODE_ALPHA]),
			"Error: Failed to create alpha blend state.");

		//Additive blend state info
		for (unsigned i = 0; i < MAX_BIND_SLOTS; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = i == 0 ? TRUE : FALSE;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		//Create additive blend state
		ErrorHandler(m_Data->device->CreateBlendState(&blendDesc, &m_Data->blendState[BlendMode::BLEND_MODE_ADDITIVE]),
			"Error: Failed to create additive blend state");

		//Additive render target blend state
		for (unsigned i = 0; i < MAX_BIND_SLOTS; ++i)
		{
			blendDesc.RenderTarget[i].BlendEnable = i == 0 ? TRUE : FALSE;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		//Create additive render target blend state
		ErrorHandler(m_Data->device->CreateBlendState(&blendDesc, &m_Data->blendState[BlendMode::BLEND_MODE_ADDITIVE_RENDER_TARGET]),
			"Error: Failed to create additive render target blend state");

		unsigned sampleMask = 0xFFFFFFFF;
		m_Data->deviceContext->OMSetBlendState(NULL, 0, sampleMask);
	}

  void SkyeDX11::InitializeFontWrapperFactory(void)
  {
    ErrorHandler(FW1CreateFactory(FW1_VERSION, &m_Data->fontFactory));
  }

  void SkyeDX11::ResizeBuffers(unsigned width, unsigned height)
	{
		//Set null render target as the current render target
		ID3D11RenderTargetView* rtv = nullptr;
		m_Data->deviceContext->OMSetRenderTargets(0, &rtv, nullptr);

		//Free the back buffer memory
		if (m_Data->backBuffer.rtv)
			m_Data->backBuffer.rtv.Release();

		//Resize buffers
		ErrorHandler(m_Data->swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH),
			"Error: Failed to resize back buffer.");

		//Reinitialize back buffer with the new size
		InitializeBackBuffer();

	}
}
