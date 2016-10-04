/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Config.cpp 
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Config.h"

#include "Core/systems/input/Input.h"
#include "Core/systems/window/Window.h"
#include "Audio/manager/AudioManager.h"
#include "Physics/core/PhysicsSystem.h"
#include "GameLogic/manager/GameLogicManager.h"
#include "Editor/InGameEditor/InGameEditor.h"

#include "Graphics/includes/SkyeDX/SkyeDX11.h"

#include "Graphics\includes\GraphicsSystem\SkyeGraphics.h"

#include "Core/engine/frameratecontroller/FrameRateController.h"
#include "GameLogic/GameLogicSystem.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/SkyeAwesomium.h"
#include "Lua/LuaEngine.h"

#include <windows.h>

static bool editor;
static bool vsync = true;

namespace Core
{
  void TypeRegistration();

}

Core::Engine* AllocateEngine(int argc, char** argv)
{
  Core::TypeRegistration();

  Core::Engine* Core = new Core::Engine();

  Core::FrameRateController * FrameRateController = new Core::FrameRateController();
  editor = true;

	#ifdef _DEBUG
  OpenConsole();

  Window* window = new Window("GAM450 Project", Config::desiredWidth, Config::desiredHeight, false);
#else
  Window* window = new Window("GAM450 Project", Config::desiredWidth, Config::desiredHeight, true);
#endif

  Core->AddSystem(new Input::InputHandler());

  Core->AddSystem(window);

  Core->AddSystem(new Physics::PhysicsSystem());

  // is this still a thing???
  Core->AddSystem(new GameLogic::GameLogicManager()); 
  
  Core->AddSystem(new Audio::AudioManager());

  //For Edtior
  SkyeGraphics* graphcis = new SkyeGraphics(window->hWnd);
  Core->AddSystem(graphcis);

  if (Config::installer)
    Core->AddSystem(new SkyeAwesomium());

  // no more sandbox, we will be running the game logic system instead
  Core->AddSystem(new GameLogic::GameLogicSystem());

  // only add an editor if we are not turning in/using an installer
  //if (!Config::installer)
    //Core->AddSystem(new Editor::InGameEditor(window->hWnd, graphcis));

  //Core->MakeWindow(GetModuleHandle(NULL), 1, !editor);

  if (Config::installer)
    ShowCursor(FALSE);

  return Core;
}

void InitEngine(void)
{
  Core::ENGINE->Initialize();

  Core::ENGINE->SystemMessage(Core::Message(Core::Message::EngineReady));
}
