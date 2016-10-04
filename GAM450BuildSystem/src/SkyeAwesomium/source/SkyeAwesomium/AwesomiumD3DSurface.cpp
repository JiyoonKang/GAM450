/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AwesomiumD3DSurface.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  AwesomiumD3DSurface.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch/precompiled.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/AwesomiumD3DSurface.h"
#include "Graphics/includes/SkyeDX/SkyeDX11.h"

AwesomiumD3DSurface::AwesomiumD3DSurface(Skye::Renderer* renderContext, int width, int height) : BitmapSurface(width, height), m_renderContext(renderContext)
{
	m_renderContext->CreateTexture(m_Texture, nullptr, Skye::Area(width, height), Skye::DataFormat::RGBA_UNORM);

}

AwesomiumD3DSurface::~AwesomiumD3DSurface()
{
	//m_renderContext->Release(m_Texture);
}

void AwesomiumD3DSurface::UpdateTexture()
{
	if (is_dirty())
	{
		unsigned rowPitch = 0;

		unsigned char* data = static_cast<unsigned char*>(m_renderContext->Map(m_Texture, rowPitch));

		CopyTo(data, rowPitch, 4, true, false);

		m_renderContext->Unmap(m_Texture);
		
	}

	set_is_dirty(false);
}

Skye::ObjectHandle& AwesomiumD3DSurface::GetTexture()
{
	return m_Texture;
}

D3DTextureSurfaceFactory::D3DTextureSurfaceFactory(Skye::Renderer* renderContext) : m_renderContext(renderContext)
{
}

D3DTextureSurfaceFactory::~D3DTextureSurfaceFactory()
{
}

Awesomium::Surface* D3DTextureSurfaceFactory::CreateSurface(Awesomium::WebView* view, int width, int height)
{
	return new AwesomiumD3DSurface(m_renderContext, width, height);
}

void D3DTextureSurfaceFactory::DestroySurface(Awesomium::Surface* surface)
{
	delete static_cast<AwesomiumD3DSurface*>(surface);
}