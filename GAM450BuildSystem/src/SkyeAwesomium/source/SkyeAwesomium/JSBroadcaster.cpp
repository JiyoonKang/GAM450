/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: JSBroadcaster.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/JSBroadcaster.h"
#include "Awesomium/STLHelpers.h"

JSBroadcaster::JSBroadcaster(): m_Window(Awesomium::JSValue())
{
}

JSBroadcaster::~JSBroadcaster()
{
}

void JSBroadcaster::SetCurrentGlobalWindow(Awesomium::JSValue currentGlobal)
{
	m_Window = currentGlobal;
}

void JSBroadcaster::InvokeJSMethod(const std::string& methodName)
{
	if (m_Window.IsObject())
	{
		m_Window.ToObject().Invoke(Awesomium::WSLit(methodName.c_str()), m_Args);
		m_Args.Clear();
	}
}


void JSBroadcaster::AddArgument(const std::string& string)
{
	m_Args.Push(ToJSValue(string));
}

void JSBroadcaster::AddArgument(const char* string)
{
	m_Args.Push(ToJSValue(string));
}

void JSBroadcaster::AddArgument(const int& value)
{
	m_Args.Push(ToJSValue(value));
}

void JSBroadcaster::AddArgument(const double& value)
{
	m_Args.Push(ToJSValue(value));
}

void JSBroadcaster::AddArgument(const bool& boolean)
{
	m_Args.Push(ToJSValue(boolean));
}

Awesomium::JSValue JSBroadcaster::ToJSValue(const std::string& string) const
{
	return Awesomium::JSValue(Awesomium::WebString(Awesomium::WSLit(string.c_str())));
}

Awesomium::JSValue JSBroadcaster::ToJSValue(const char* string) const
{
	return Awesomium::JSValue(Awesomium::WebString(Awesomium::WSLit(string)));
}

Awesomium::JSValue JSBroadcaster::ToJSValue(const int& value) const
{
	return Awesomium::JSValue(value);
}

Awesomium::JSValue JSBroadcaster::ToJSValue(const double& value) const
{
	return Awesomium::JSValue(value);
}

Awesomium::JSValue JSBroadcaster::ToJSValue(const bool& boolean) const
{
	return Awesomium::JSValue(boolean);
}