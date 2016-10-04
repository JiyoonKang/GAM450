/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AwesomiumObject.cpp 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/AwesomiumObject.h"

#include "Awesomium/WebCore.h"
#include "Awesomium/STLHelpers.h"

AwesomiumObject::AwesomiumObject() : m_WebView(nullptr), m_Surface(nullptr)
{
}

AwesomiumObject::~AwesomiumObject()
{
}

void AwesomiumObject::Initialize()
{
	Awesomium::WebURL url(Awesomium::WSLit(URL.c_str()));

	m_WebView->LoadURL(url);

	m_WebView->SetTransparent(true);
}

void AwesomiumObject::Destroy() const
{
	m_WebView->Destroy();
}

Awesomium::WebView* AwesomiumObject::GetWebView() const
{
	return m_WebView;
}

void AwesomiumObject::SetWebView(Awesomium::WebView* view)
{
	m_WebView = view;
}


AwesomiumD3DSurface* AwesomiumObject::GetSurface()
{
	m_Surface = (AwesomiumD3DSurface*)m_WebView->surface();

	return m_Surface;
}