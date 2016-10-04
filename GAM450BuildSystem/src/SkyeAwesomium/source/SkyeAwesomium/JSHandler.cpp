/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: JSHandler.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/

#include "pch/precompiled.h"
#include "Awesomium/STLHelpers.h"
#include <includes/SkyeAwesomium/JSHandler.h>
#include "Base/util/debug/DebugAssist.h"
#include "Core/engine/core/Engine.h"

static auto const s_DispatchMessage = Awesomium::WSLit("DispatchMsg");

JSHandler::JSHandler(void) : Awesomium::JSMethodHandler(), Awesomium::WebViewListener::Load()
{

}

JSHandler::~JSHandler(void)
{

}

void JSHandler::OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id,
	const Awesomium::WebString& method_name, const Awesomium::JSArray& args)
{
	if (m_Skye.remote_id() == remote_object_id)
	{
		if (method_name != s_DispatchMessage)
		{
			ErrorIf(true, "Unknown method requested: %1%", (method_name));
		}

    Core::ENGINE->SystemMessage(Core::MessageUI(Awesomium::ToString(args[0].ToString())));
		//
		//	MessageUI m;
		//	m.msg = Awesomium::ToString(args[0].ToString());
		//
		//	// PrintOut(m.msg.c_str());
		//
		//	CORE->BroadcastMessage(&m);
	}
}

Awesomium::JSValue JSHandler::OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int remote_object_id,
	const Awesomium::WebString& method_name, const Awesomium::JSArray& args)
{
	OnMethodCall(caller, remote_object_id, method_name, args);
	return Awesomium::JSValue::Undefined();
}