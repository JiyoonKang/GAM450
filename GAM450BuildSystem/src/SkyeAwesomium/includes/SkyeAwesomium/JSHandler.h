/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: JSHandler.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Awesomium/WebCore.h"

class JSHandler : public Awesomium::WebViewListener::Load, public Awesomium::JSMethodHandler
{
	public:
		JSHandler(void);
		~JSHandler(void);

		virtual void OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id,
			const Awesomium::WebString& method_name, const Awesomium::JSArray& args) override;

		virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int remote_object_id,
			const Awesomium::WebString& method_name, const Awesomium::JSArray& args) override;

		virtual void OnBeginLoadingFrame(Awesomium::WebView* caller,
			int64 frame_id,
			bool is_main_frame,
			const Awesomium::WebURL& url,
			bool is_error_page) override
			{
			}

		/// This event occurs when a frame fails to load. See error_desc
		/// for additional information.
		virtual void OnFailLoadingFrame(Awesomium::WebView* caller,
			int64 frame_id,
			bool is_main_frame,
			const Awesomium::WebURL& url,
			int error_code,
			const Awesomium::WebString& error_desc)
		{
			// ignore
			//Warning("Failed to load url(%1%): %2%", Awesomium::ToString(url.spec()), Awesomium::ToString(error_desc));
		}

		/// This event occurs when the page finishes loading a frame.
		/// The main frame always finishes loading last for a given page load.
		virtual void OnFinishLoadingFrame(Awesomium::WebView* caller,
			int64 frame_id,
			bool is_main_frame,
			const Awesomium::WebURL& url)
		{
		}

		/// This event occurs when the DOM has finished parsing and the
		/// window object is available for JavaScript execution.
		virtual void OnDocumentReady(Awesomium::WebView* caller,
			const Awesomium::WebURL& url)
		{
		}

		Awesomium::JSObject m_Skye;
};