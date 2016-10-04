/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SkyeAwesomium.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once


#include "Core/systems/System.h"

#include "AwesomiumD3DSurface.h"

#include "AwesomiumObject.h"
#include "Awesomium/JSValue.h"

#include <vector>
#include <unordered_map>


// Various macro definitions
#define WIDTH   800
#define HEIGHT  600
typedef std::unordered_map<std::string, unsigned> ElementDictionary;
typedef std::vector<AwesomiumObject> ElementList;

namespace Awesomium
{
	class WebCore;
	class WebView;
}

class JSHandler;
class AwesomiumD3DSurface;

class SkyeAwesomium : public Core::ISystem
{
	struct register_helper {};
	
	public:
		SkyeAwesomium(void);
		~SkyeAwesomium(void);

		//Base class methods
		virtual void Initialize(void) override;
		virtual void Update(float dt) override;
		virtual void ReceiveMessage(Core::Message& message) override;

		virtual std::string GetName(void) const override { return "SkyeAwesomium"; }

		//Utility Methods
		AwesomiumD3DSurface* GetActiveSurface(void);
		void RegisterUIElement(const std::string& elementName, const std::string& URL);
		void RegisterUIElement(const std::string& pageName, register_helper& h);
		void SetActiveUIByName(const std::string& name);
    void SetActiveUIByID(const unsigned& ID);
    void UnregisterUIElement(const std::string& elementName);
	void RestartMenu(void);

    Awesomium::JSValue GetCurrentGlobalWindow(void);

	private:
    void InitRegisterUIElements(void);
		void ShutDown(void);
		void CreateUIElement(void);
		void Resize(void);
		void SendToJS(void);
    void UpdateInput();

		Awesomium::WebCore* m_WebCore;
		D3DTextureSurfaceFactory* m_SurfaceFactory;

		ElementDictionary m_UIDictionary;
		ElementList m_UIElements;

		JSHandler* m_MethodHandler;

		unsigned m_ActiveElement;
		unsigned m_Previous;
		bool m_SystemInitialized;

};

extern SkyeAwesomium* AWESOMIUM;