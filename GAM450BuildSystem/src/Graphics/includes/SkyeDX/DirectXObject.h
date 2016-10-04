/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DirectXObject.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  DirectXObject.h
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <includes\SkyeDX\ResourcePointers.h>
#include <includes\SkyeDX\DXTypes.h>
#include <includes\SkyeDX\Area.h>
#include <includes\SkyeDX\Viewport.h>
#include <includes\SkyeDX\ObjectHandle.h>

namespace Skye
{
	namespace D3DObject
	{
		struct D3DObject
		{
			D3DObject(void) : deleted(false) {}
			bool deleted;
		};

		struct Texture : D3DObject
		{
			Texture2DPtr texture2D;
			ShaderResourceViewPtr srv;
			Area size;
		};

		struct RenderTarget : D3DObject
		{
			RenderTargetViewPtr rtv;
			ShaderResourceViewPtr srv;
			DataFormat format;
			float downSamplePercentage;
			Viewport viewport;
		};

		struct VertexShader : D3DObject
		{
			VertexShaderPtr vertexShader;
			InputLayoutPtr layout;
		};

		struct PixelShader : D3DObject
		{
			PixelShaderPtr pixelShader;
		};

		struct GeometryShader : D3DObject
		{
			GeometryShaderPtr geometryShader;
		};

		struct ComputeShader : D3DObject
		{
			ComputeShaderPtr computeShader;
		};

		struct Buffer : D3DObject
		{
			BufferPtr buffer;
			BufferUsage usage;
			unsigned size;
		};

		struct DepthBuffer : D3DObject
		{
			ShaderResourceViewPtr srv;
			DepthStencilViewPtr depthStencilView;
			ObjectHandle toResizeWith;
			Area size;
		};

    struct FontWrapper : D3DObject
    {
      FontWrapperPtr fontWrapper;
    };

    struct FontWrapperDrawCall : D3DObject
    {
      std::wstring text;
      float fontSize;
      float xPos;
      float yPos;
      unsigned color;
    };
	}
}