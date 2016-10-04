/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ResourcePointers.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  ResourcePointers.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <comdef.h>
#include <includes\SkyeDX\DXIncludes.h>

#define SMART_POINTER_TYPEDEF(Interface, IID, name) typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<Interface, &IID>> name

namespace Skye
{

	SMART_POINTER_TYPEDEF(ID3D11Device, __uuidof(ID3D11Device), DevicePtr);
	SMART_POINTER_TYPEDEF(ID3D11DeviceContext, __uuidof(ID3D11DeviceContext), DeviceContextPtr);

	SMART_POINTER_TYPEDEF(IDXGISwapChain, __uuidof(IDXGISwapChain), SwapChainPtr);
	SMART_POINTER_TYPEDEF(IDXGIFactory, __uuidof(IDXGIFactory), FactoryPtr);

	SMART_POINTER_TYPEDEF(IDXGIAdapter, __uuidof(IDXGIAdapter), AdapterPtr);
	SMART_POINTER_TYPEDEF(IDXGIOutput, __uuidof(IDXGIOutput), OutputPtr);

	SMART_POINTER_TYPEDEF(ID3D11Texture2D, __uuidof(ID3D11Texture2D), Texture2DPtr);
	SMART_POINTER_TYPEDEF(ID3D11ShaderResourceView, __uuidof(ID3D11ShaderResourceView), ShaderResourceViewPtr);
	SMART_POINTER_TYPEDEF(ID3D11RenderTargetView, __uuidof(ID3D11RenderTargetView), RenderTargetViewPtr);
	SMART_POINTER_TYPEDEF(ID3D11UnorderedAccessView, __uuidof(ID3D11UnorderedAccessView), UnorderedAccessViewPtr);
	SMART_POINTER_TYPEDEF(ID3D11DepthStencilView, __uuidof(ID3D11DepthStencilView), DepthStencilViewPtr);
	SMART_POINTER_TYPEDEF(ID3D11DepthStencilState, __uuidof(ID3D11DepthStencilState), DepthStencilStatePtr);

	SMART_POINTER_TYPEDEF(ID3D11VertexShader, __uuidof(ID3D11VertexShader), VertexShaderPtr);
	SMART_POINTER_TYPEDEF(ID3D11PixelShader, __uuidof(ID3D11PixelShader), PixelShaderPtr);
	SMART_POINTER_TYPEDEF(ID3D11GeometryShader, __uuidof(ID3D11GeometryShader), GeometryShaderPtr);
	SMART_POINTER_TYPEDEF(ID3D11ComputeShader, __uuidof(ID3D11ComputeShader), ComputeShaderPtr);

	SMART_POINTER_TYPEDEF(ID3D11InputLayout, __uuidof(ID3D11InputLayout), InputLayoutPtr);
	SMART_POINTER_TYPEDEF(ID3D11Buffer, __uuidof(ID3D11Buffer), BufferPtr);
	SMART_POINTER_TYPEDEF(ID3D10Blob, __uuidof(ID3D10Blob), BlobPtr);

	SMART_POINTER_TYPEDEF(ID3D11RasterizerState, __uuidof(ID3D11RasterizerState), RasterizerStatePtr);
	SMART_POINTER_TYPEDEF(ID3D11BlendState, __uuidof(ID3D11BlendState), BlendStatePtr);
	SMART_POINTER_TYPEDEF(ID3D11SamplerState, __uuidof(ID3D11SamplerState), SamplerStatePtr);

  SMART_POINTER_TYPEDEF(IFW1Factory, __uuidof(IFW1Factory), FW1FontFactoryPtr);
  SMART_POINTER_TYPEDEF(IFW1FontWrapper, __uuidof(IFW1FontWrapper), FontWrapperPtr);
}