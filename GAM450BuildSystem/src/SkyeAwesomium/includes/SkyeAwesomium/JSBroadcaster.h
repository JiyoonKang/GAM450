/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: JSBroadcaster.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Awesomium/JSValue.h"
#include <string>

class JSBroadcaster
{
	public:
		JSBroadcaster(void);
		~JSBroadcaster(void);

		void SetCurrentGlobalWindow(Awesomium::JSValue currentGlobal);
		
		void InvokeJSMethod(const std::string& methodName);
		void AddArgument(const std::string& string);
		void AddArgument(const char* string);
		void AddArgument(const int& value);
		void AddArgument(const double& value);
		void AddArgument(const bool& boolean);

	private:
		Awesomium::JSValue ToJSValue(const std::string& string) const;
		Awesomium::JSValue ToJSValue(const char* string) const;
		Awesomium::JSValue ToJSValue(const int& value) const;
		Awesomium::JSValue ToJSValue(const double& value) const;
		Awesomium::JSValue ToJSValue(const bool& boolean) const;

		Awesomium::JSArray m_Args;
		Awesomium::JSValue m_Window;
};