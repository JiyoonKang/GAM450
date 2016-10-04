/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AwesomiumD3DSurface.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Awesomium/Surface.h"
#include "Awesomium/BitmapSurface.h"

#include "Graphics/includes/SkyeDX/DXTypes.h"
#include "Graphics/includes/SkyeDX/ObjectHandle.h"

class AwesomiumD3DSurface : public Awesomium::BitmapSurface
{
	public:
		AwesomiumD3DSurface(Skye::Renderer* renderContext, int width, int height);
		virtual ~AwesomiumD3DSurface(void);

		void UpdateTexture(void);
		Skye::ObjectHandle& GetTexture(void);


	private:

		Skye::ObjectHandle m_Texture;
		Skye::Renderer* m_renderContext;
};

class D3DTextureSurfaceFactory : public Awesomium::SurfaceFactory
{
	public:
		D3DTextureSurfaceFactory(Skye::Renderer* renderContext);
		virtual ~D3DTextureSurfaceFactory(void);

		virtual Awesomium::Surface* CreateSurface(Awesomium::WebView* view, int width, int height) override;

		virtual void DestroySurface(Awesomium::Surface* surface) override;

	private:

		Skye::Renderer* m_renderContext;
};