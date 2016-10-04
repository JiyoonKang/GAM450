/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: SkyeAwesomium.cpp
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/SkyeAwesomium.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/JSHandler.h"
#include "Graphics/includes/GraphicsSystem/SkyeGraphics.h"

#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include <Core/systems/window/Window.h>
#include <includes/SkyeAwesomium/JSBroadcaster.h>
#include <Base/util/debug/DebugAssist.h>
#include <sstream>

#include "GameLogic/GameLogicSystem.h"
#include "physics/core/PhysicsSystem.h"

static auto const s_DispatchMessage = Awesomium::WSLit("DispatchMsg");
static auto const s_defaultURL = "file:///UserInterface/ToxicityUI/GameMenus/";


using namespace Awesomium;
SkyeAwesomium* AWESOMIUM = nullptr;

SkyeAwesomium::SkyeAwesomium() : m_WebCore(nullptr),
m_SurfaceFactory(nullptr),
m_MethodHandler(nullptr),
m_ActiveElement(0),
m_Previous(0),
m_SystemInitialized(false)
{
  AWESOMIUM = this;
}

SkyeAwesomium::~SkyeAwesomium()
{
  ShutDown();

  m_UIDictionary.clear();
  m_UIElements.clear();
  AWESOMIUM = nullptr;
}

void SkyeAwesomium::Initialize()
{
  // Create the WebCore singleton with default configuration
  WebConfig config;

  config.log_level = Awesomium::LogLevel::kLogLevel_None;

  m_WebCore = WebCore::instance()->Initialize(config);


  m_MethodHandler = new JSHandler();

  m_SurfaceFactory = new D3DTextureSurfaceFactory(GRAPHICS->GetRenderContext());
  m_WebCore->set_surface_factory(m_SurfaceFactory);

  InitRegisterUIElements();

  Resize();

  m_SystemInitialized = true;
}

void SkyeAwesomium::InitRegisterUIElements(void)
{
  register_helper rh;


  RegisterUIElement("Confirmation", rh);
  RegisterUIElement("ConfirmationMainMenu", rh);
  RegisterUIElement("Credits", rh);
  RegisterUIElement("HowToPlay", rh);
  RegisterUIElement("HUD", rh);
  RegisterUIElement("LevelSelectMainMenu", rh);
  RegisterUIElement("LevelSelectPauseMenu", rh);
  RegisterUIElement("Options", rh);
  RegisterUIElement("PauseMenu", rh);
  RegisterUIElement("MainMenu", rh);
  RegisterUIElement("SplashScreens", rh);

  //RegisterUIElement("TitleScreen", rh);
  //RegisterUIElement("LevelSelect", rh);
  //
  //RegisterUIElement("LevelSelectMainMenu", rh);
  //RegisterUIElement("HowToPlay", rh);
  //RegisterUIElement("OptionsMenu", rh);
  //RegisterUIElement("Credits", rh);
  //RegisterUIElement("HUD", rh);
  //RegisterUIElement("QuitGame", rh);
  //RegisterUIElement("PauseMenu", rh);
  //RegisterUIElement("SplashScreen", rh); // last element registered will be the one shown first




}

void SkyeAwesomium::UpdateInput()
{
  JSBroadcaster broadcaster;

  broadcaster.SetCurrentGlobalWindow(GetCurrentGlobalWindow());
  if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_W)
    || Input::KeyboardEvent::IsKeyPressed(Input::KEY_UP))
  {
    broadcaster.InvokeJSMethod("OnTickUp");
  }
  
  if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_S)
    || Input::KeyboardEvent::IsKeyPressed(Input::KEY_DOWN))
  {
    broadcaster.InvokeJSMethod("OnTickDown");
  }
  //std::cout << "LEVEL NAME:" << Core::ENGINE->GetCurrentLevel() << "..." << std::endl;
  GameLogic::LevelID b = GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel();
  std::string n = Core::ENGINE->GetCurrentLevel();
  bool t = Input::KeyboardEvent::IsKeyPressed(Input::KEY_ESCAPE);
  if (Input::KeyboardEvent::IsKeyPressed(Input::KEY_SPACE) || Input::KeyboardEvent::IsKeyPressed(Input::KEY_ENTER) || (b == GameLogic::LevelID::Invalid && t))
  {
    broadcaster.InvokeJSMethod("OnSelect");
  }

   for (unsigned i = 0; i < 4; ++i)
  {
    if (Input::GamePadEvent::GetGamePad(i)->connected)
    {
      if (Input::GamePadEvent::GetGamePad(i)->lastlStick.y <= 0.9f && Input::GamePadEvent::GetGamePad(i)->LeftStick().y > 0.9f)
      {
        broadcaster.InvokeJSMethod("OnTickUp");
      }
      if (Input::GamePadEvent::GetGamePad(i)->lastlStick.y >= -0.9f && Input::GamePadEvent::GetGamePad(i)->LeftStick().y < -0.9f)
      {
        broadcaster.InvokeJSMethod("OnTickDown");
      }

      if (Input::GamePadEvent::GetGamePad(i)->lastrStick.y <= 0.9f && Input::GamePadEvent::GetGamePad(i)->RightStick().y > 0.9f)
      {
        broadcaster.InvokeJSMethod("OnTickUp");
      }
      if (Input::GamePadEvent::GetGamePad(i)->lastrStick.y >= -0.9f && Input::GamePadEvent::GetGamePad(i)->RightStick().y < -0.9f)
      {
        broadcaster.InvokeJSMethod("OnTickDown");
      }


      if (Input::GamePadEvent::GetGamePad(i)->ButtonJustPressed(XINPUT_GAMEPAD_DPAD_UP))
      {
        broadcaster.InvokeJSMethod("OnTickUp");
      }
      if (Input::GamePadEvent::GetGamePad(i)->ButtonJustPressed(XINPUT_GAMEPAD_DPAD_DOWN))
      {
        broadcaster.InvokeJSMethod("OnTickDown");
      }

      if (Input::GamePadEvent::GetGamePad(i)->ButtonJustPressed(XINPUT_GAMEPAD_A) || Input::GamePadEvent::GetGamePad(i)->ButtonJustPressed(XINPUT_GAMEPAD_START))
      {
        broadcaster.InvokeJSMethod("OnSelect");
      }
      if (Input::GamePadEvent::GetGamePad(i)->ButtonJustPressed(XINPUT_GAMEPAD_START))
      {
        if ((GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() > GameLogic::LevelID::Menu && GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() < GameLogic::LevelID::Count))
        {
          if (!GameLogic::GAMELOGIC_SYSTEM->GetPaused())
          {
            Core::MessageUI goToPause("ChangeUI PauseMenu");
            Core::MessageUI pauseGame("Action Pause");
            Core::ENGINE->SystemMessage(goToPause);
            Core::ENGINE->SystemMessage(pauseGame);
            Audio::AUDIOMANAGER->PauseAll();
            //Audio::AUDIOMANAGER->PauseMusic();
          Input::GamePadEvent stopVibe;
          stopVibe.GetGamePad(0)->StopAllVibration();
          stopVibe.GetGamePad(1)->StopAllVibration();
          stopVibe.GetGamePad(2)->StopAllVibration();
          stopVibe.GetGamePad(3)->StopAllVibration();
          
          }
          else
          {
            Core::MessageUI goToGame("ChangeUI HUD");
            Core::MessageUI unPauseGame("Action unPause");
            Core::ENGINE->SystemMessage(goToGame);
            Core::ENGINE->SystemMessage(unPauseGame);
            Audio::AUDIOMANAGER->ResumeAll();
            //Audio::AUDIOMANAGER->ResumeMusic();
          }
        }
      }

    }
  }

}

void SkyeAwesomium::Update(float dt)
{
  m_WebCore->Update();

  if (m_UIElements.size() != 0 && m_UIElements[m_ActiveElement].GetSurface() != nullptr)
  {
    m_UIElements[m_ActiveElement].GetSurface()->UpdateTexture();

    GRAPHICS->SetUITexture(m_UIElements[m_ActiveElement].GetSurface()->GetTexture());
  }

  UpdateInput();
}
#include <iostream>
void SkyeAwesomium::ReceiveMessage(Core::Message& message)
{
  switch (message.MessageId)
  {
  case Core::Message::KeyboardEvent:
  {
    KeyboardEventMessage* m = reinterpret_cast<KeyboardEventMessage*>(&message);

    m_UIElements[m_ActiveElement].GetWebView()->InjectKeyboardEvent(Awesomium::WebKeyboardEvent(m->msg, m->wParam, m->lParam));
    break;
  }

  case Core::Message::MouseMove:
  {
    MouseMove* m = reinterpret_cast<MouseMove*>(&message);
    //m_UIElements[m_ActiveElement].GetWebView()->InjectMouseMove((int)m->MousePosition.x, (int)m->MousePosition.y);
    break;
  }

  case Core::Message::MouseWheel:
  {
    MessageMouseWheel* m = reinterpret_cast<MessageMouseWheel*>(&message);
   // m_UIElements[m_ActiveElement].GetWebView()->InjectMouseWheel(m->wParam, 0);
    break;
  }

  case Core::Message::MouseHorizontal:
  {
    MessageMouseWheel* m = reinterpret_cast<MessageMouseWheel*>(&message);
   // m_UIElements[m_ActiveElement].GetWebView()->InjectMouseWheel(0, m->wParam);
    break;
  }

  case Core::Message::MouseButton:
  {
    MouseButtonMessage* m = reinterpret_cast<MouseButtonMessage*>(&message);

    if (m->MouseButtonIndex == MouseButtonMessage::LeftMouse)
    {
      if (m->ButtonIsPressed)
      {
        m_UIElements[m_ActiveElement].GetWebView()->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Left);
      }
      else
      {
        m_UIElements[m_ActiveElement].GetWebView()->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Left);
      }
    }

    if (m->MouseButtonIndex == MouseButtonMessage::RightMouse)
    {
      if (m->ButtonIsPressed)
      {
        //m_UIElements[m_ActiveElement].GetWebView()->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Right);
      }
      else
      {
        //m_UIElements[m_ActiveElement].GetWebView()->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Right);
      }
    }

    break;
  }

  case Core::Message::WindowResize:
  {
    if (m_SystemInitialized)
      Resize();
    break;
  }

  case Core::Message::UIMessage:
  {
    Core::MessageUI& m = dynamic_cast<Core::MessageUI&>(message);

    //Split the string into tokens=================================
    std::string buf; // Have a buffer string
    std::stringstream ss(m.msg); // Insert the string into a stream
    std::vector<std::string> args; // Create vector to hold our words
    while (ss >> buf)
      args.push_back(buf);

    if (args.size() > 0)
    {
      if (args.size() > 1 && strcmp("ChangeUI", args[0].c_str()) == 0)
      {
        if (strcmp("MainMenu", args[1].c_str()) == 0)
        {
          Audio::AUDIOMANAGER->StopAll();
          GameLogic::GAMELOGIC_SYSTEM->SwitchLevel(GameLogic::LevelID::Menu);
          GameLogic::GAMELOGIC_SYSTEM->PlayMusic("MenuTrackLoopV1 (2)");
        
          //UnregisterUIElement("MainMenu");
          //
          //register_helper rh;
          //
          //RegisterUIElement("MainMenu", rh);
        }
        SetActiveUIByName(args[1]);
      }

      if (args.size() > 2 && strcmp("JSMethod", args[0].c_str()) == 0)
      {
        JSBroadcaster broadcaster;
        broadcaster.SetCurrentGlobalWindow(GetCurrentGlobalWindow());

        for (unsigned i = 2; i < args.size(); ++i)
          broadcaster.AddArgument(args[i]);

        broadcaster.InvokeJSMethod(args[1]);
      }

      if (args.size() > 1 && strcmp("Action", args[0].c_str()) == 0)
      {
        // if we are pausing
        if (strcmp("Pause", args[1].c_str()) == 0)
        {
          // pause game logic and physics system
          GameLogic::GAMELOGIC_SYSTEM->SetPaused(true);
          Physics::PHYSICS->SetFrameStepping(true);
        }
        else if (strcmp("unPause", args[1].c_str()) == 0)
        {
          // unpause game logic and physics system
          GameLogic::GAMELOGIC_SYSTEM->SetPaused(false);
          Physics::PHYSICS->SetFrameStepping(false);
        }
      }
    }
    break;
  }
  }
}

AwesomiumD3DSurface* SkyeAwesomium::GetActiveSurface()
{
  return m_UIElements[m_ActiveElement].GetSurface();
}
void SkyeAwesomium::UnregisterUIElement(const std::string& elementName)
{
  //Look for the UI element in the list of currently existent elements
  if (m_UIDictionary.find(elementName) != m_UIDictionary.end())
  {
    //Found element to erase, remove from list
    m_UIElements.erase(m_UIElements.begin() + m_UIDictionary[elementName]);

    m_UIDictionary.erase(elementName);
  }

  //If not found, will not erase anything
}

void SkyeAwesomium::RestartMenu()
{
	UnregisterUIElement("MainMenu");
	
	register_helper rh;
	
	RegisterUIElement("MainMenu", rh);
}

void SkyeAwesomium::RegisterUIElement(const std::string& elementName, const std::string& URL)
{
  //Look for the UI element in the list of currently existent elements
  if (m_UIDictionary.find(elementName) != m_UIDictionary.end())
  {
    PrintOut("UI Element %s already exists, element not registered", elementName.c_str());
    SetActiveUIByName(elementName);
  }

  //If not found, create it
  else
  {
    AwesomiumObject tempObject;
    tempObject.URL = URL;

    m_UIDictionary.insert(std::make_pair(elementName, m_UIElements.size()));
    //m_UIDictionary[elementName] = m_UIElements.size();
    m_UIElements.push_back(tempObject);
  }
}

void SkyeAwesomium::RegisterUIElement(const std::string& pageName, register_helper& h)
{
  //Look for the UI element in the list of currently existent elements
  if (m_UIDictionary.find(pageName) != m_UIDictionary.end())
  {
    PrintOut("UI Element %s already exists, element not registered", pageName.c_str());
    SetActiveUIByName(pageName);
  }

  else
  {
    //static const std::string defaultURL("file:///Assets/UserInterface/");
    //const std::string pageURL(defaultURL + pageName + ".html");
    const std::string pageURL(s_defaultURL + pageName + ".html");

    RegisterUIElement(pageName, pageURL);
    SetActiveUIByName(pageName);

    m_UIElements[m_ActiveElement].SetWebView(m_WebCore->CreateWebView(WINDOWSSYSTEM->GetScreenWidth(), WINDOWSSYSTEM->GetScreenHeight()));
    m_UIElements[m_ActiveElement].Initialize();

    JSValue jsObj;

    do
    {
      jsObj = m_UIElements[m_ActiveElement].GetWebView()->CreateGlobalJavascriptObject(Awesomium::WSLit("Skye"));

    } while (!jsObj.IsObject());

    m_MethodHandler->m_Skye = jsObj.ToObject();

    m_MethodHandler->m_Skye.SetCustomMethod(Awesomium::WSLit("DispatchMsg"), false);

    m_UIElements[m_ActiveElement].GetWebView()->set_js_method_handler(m_MethodHandler);
  }
}

void SkyeAwesomium::SetActiveUIByName(const std::string& name)
{
  ErrorIf(m_UIDictionary.find(name) == m_UIDictionary.end(),
    "UI Element with the name '%s' does not exist", name.c_str());

  //Make current web view loose focus
  if (m_UIElements[m_ActiveElement].GetWebView())
    m_UIElements[m_ActiveElement].GetWebView()->Unfocus();

  //Update current and previous IDs
  m_Previous = m_ActiveElement;
  m_ActiveElement = m_UIDictionary.find(name)->second;

  //Bring focus to new current web view
  if (m_UIElements[m_ActiveElement].GetWebView())
    m_UIElements[m_ActiveElement].GetWebView()->Focus();
}

void SkyeAwesomium::SetActiveUIByID(const unsigned& ID)
{
  ErrorIf(m_UIElements.size() <= ID, "UI Element with ID: '%i' does not exist", ID);

  if (m_UIElements[m_ActiveElement].GetWebView())
    m_UIElements[m_ActiveElement].GetWebView()->Unfocus();

  m_Previous = m_ActiveElement;
  m_ActiveElement = ID;

  if (m_UIElements[m_ActiveElement].GetWebView())
    m_UIElements[m_ActiveElement].GetWebView()->Focus();
}

void SkyeAwesomium::ShutDown()
{
  m_WebCore->Shutdown();

  delete m_SurfaceFactory;
  delete m_MethodHandler;
}

void SkyeAwesomium::CreateUIElement()
{
}

void SkyeAwesomium::Resize()
{
  for (unsigned int i = 0; i < m_UIElements.size(); ++i)
  {
    if (m_UIElements[i].GetWebView())
    {
      m_UIElements[i].GetWebView()->Resize(WINDOWSSYSTEM->GetScreenWidth(), WINDOWSSYSTEM->GetScreenHeight());
    }
  }
}

void SkyeAwesomium::SendToJS()
{
  JSBroadcaster broadcaster;
  broadcaster.SetCurrentGlobalWindow(GetCurrentGlobalWindow());

  broadcaster.AddArgument("bobbitty");
  broadcaster.AddArgument(69);

  broadcaster.InvokeJSMethod("SendToJS");
}

Awesomium::JSValue SkyeAwesomium::GetCurrentGlobalWindow()
{
  return m_UIElements[m_ActiveElement].GetWebView()->ExecuteJavascriptWithResult(WSLit("window"), WSLit(""));
}