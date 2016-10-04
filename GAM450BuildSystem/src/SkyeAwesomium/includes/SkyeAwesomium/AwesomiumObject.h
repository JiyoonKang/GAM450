/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: AwesomiumObject.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include <string>


class AwesomiumD3DSurface;

namespace Awesomium
{
	class WebView;
}

class AwesomiumObject
{
	public:
		AwesomiumObject(void);
		~AwesomiumObject(void);

		void Initialize(void);
		void Destroy(void) const;

		Awesomium::WebView* GetWebView(void) const;
		AwesomiumD3DSurface* GetSurface(void);

		void SetWebView(Awesomium::WebView* view);

		std::string URL;
	private:

		Awesomium::WebView* m_WebView;
		AwesomiumD3DSurface* m_Surface;
};