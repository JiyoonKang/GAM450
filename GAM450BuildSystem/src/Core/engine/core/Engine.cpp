/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Engine.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "engine/core/Engine.h"
#include "systems/input/Input.h"
#include "Lua/LuaInterface.h"
#include "Lua/LuaEngine.h"

#include <iostream>
#include "LuaBridge.h"

namespace Core
{
  Engine* ENGINE = NULL;


  Engine::Engine() : m_running(true), L(nullptr), m_time(0)
  {
    ENGINE = this;
  }

  Engine::~Engine()
  {
    ENGINE = nullptr;

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      delete m_systems[i];

    m_systems.clear();
    m_spaces.clear();
    m_spaceMap.clear();
  }

  void Engine::AddSystem(ISystem* system)
  {
    m_systems.push_back(system);
    system->RegisterComponents();
  }

  void Engine::Initialize()
  {

    // Create the lua environment
    L = Lua::CreateEnvironment();
    // Call the post Init function
    luabridge::getGlobal(L, "PostInit")();

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Initialize();


  }

  void Engine::MakeWindow(void* hInstance, int show, bool fullScreen)
  {
  }

  void Engine::Shutdown()
  {

    // Make sure each game space has been properly cleaned up

	  for (unsigned int i = 0; i < m_spaces.size(); ++i)
	  {
		  GameSpace* space = m_spaces[i];
		  
		
		  luabridge::getGlobal(L, "gamespace")["RemoveGameSpace"](space->m_name);
					  
		  space->m_shuttingDown = true;
		  space->Cleanup();
		
		  m_spaceMap.erase(space->m_name);


		  //I HAVE ABSOLUTELY NO IDEA WHY THIS IS CRASHING IT... FUCK IF I CARE AT THIS POINT.
		  //if (space)
		  //{
		  //  delete space;
		  //}
    }

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Shutdown();

    Lua::Shutdown(L);

    L = nullptr;
  }

  void Engine::SystemMessage(Message& msg)
  {
    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->ReceiveMessage(msg);
  }

  const std::vector<std::string>& Engine::LuaComponentList(void) const
  {
    return m_luaComponentList;
  }

  void Engine::Step(void)
  {
    /* When a gamespace is first created it is not ready to have
    any logic executed on it until the start of the next frame */
    FRAMERATECONTROLLER->StartFrame();
    for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
      if (!(*it)->m_ready)
        (*it)->m_ready = true;

    for (unsigned int i = 0; i < m_systems.size(); ++i)
      m_systems[i]->Update(FRAMERATECONTROLLER->GetDT());

    // Takes care of any invalid spaces
    CleanUp();

    FRAMERATECONTROLLER->EndFrame();

    if (m_levelChange)
      GotoNextLevel();

    //if (m_loadLuaLevels)
    //  LoadLuaLevels();

  }

  void Engine::MainLoop()
  {
    Step();
  }

  bool Engine::Running() const
  {
    return m_running;
  }

  void Engine::Quit()
  {
    m_running = false;
  }

  unsigned int Engine::CurrentTime()
  {
    return ENGINE->m_time;
  }

  //////////////////////////////////////////////////////////////////////////
  // SPACES

  std::vector<GameSpace*>& Engine::Spaces()
  {
    return m_spaces;
  }

  GameSpace* Engine::CreateSpace(const char* name)
  {
    // Allocate space for a new space
    GameSpace* space = DBG_NEW GameSpace();

    if (name == nullptr)
      return nullptr;

    space->m_name = name;

    m_spaceMap[space->m_name] = space;
    m_spaces.push_back(space);

    // Get the global gamespace table and get the function within the table
    // then call the function with the name of the gamespace and the pointer
    luabridge::getGlobal(L, "gamespace")["AddGameSpace"](name, space);

    return space;
  }

  GameSpace* Engine::GetSpace(const char* name)
  {
    if (m_spaceMap.find(name) != m_spaceMap.end())
      return m_spaceMap[name];

    return nullptr;
  }

  void Engine::RemoveSpace(GameSpace* space)
  {
    m_spaceMap.erase(space->m_name);
    for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
    {
      if (*it == space)
      {
        // Move the back onto the location we are deleting
        *it = m_spaces.back();
        // Pop off the back of the vector
        m_spaces.pop_back();
        break;
      }
    }

    space->m_shuttingDown = true;
    space->Cleanup();

    luabridge::getGlobal(L, "gamespace")["RemoveGameSpace"](space->m_name);

    delete space;
  }

  void Engine::ClearSpaces()
  {
    for (unsigned int i = 0; i < m_spaces.size(); ++i)
    {
      m_spaces[i]->Destroy();
    }
  }

  void Engine::CleanUp()
  {
    for (auto it = m_spaceRemoveList.begin(); it != m_spaceRemoveList.end(); ++it)
      RemoveSpace(*it);

    m_spaceRemoveList.clear();
  }

  //////////////////////////////////////////////////////////////////////////
  // LEVELS

  void Engine::ChangeLevel(const char* name)
  {
    m_levelChange = true;
    m_nextLevel = name;
    
  }

  void Engine::GotoNextLevel()
  {
    if (!m_spaces.empty())
    {
      ClearSpaces();
      CleanUp();
    }

    //std::string filepath("content\\data\\levels\\");
    //filepath += m_nextLevel + ".level";

    FACTORY->LoadLevel(m_nextLevel.c_str(), nullptr);
    //FACTORY->LoadLevelFilePath(filepath.c_str(), m_nextLevel.c_str(), nullptr);

    m_nextLevel.clear();
    m_levelChange = false;
  }

  //////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////
  void Engine::LoadLuaLevel(const char* path)
  {
    std::string luaLevel;

    while (!m_luaLevelsToLoad.empty())
    {
      luaLevel = m_luaLevelsToLoad.top();
      luabridge::getGlobal(L, "LoadLuaLevel")(luaLevel.c_str());
      m_luaLevelsToLoad.pop();
    }

    m_loadLuaLevels = false;
  }

  void Engine::LuaChangeLevel(const char* name)
  {
    ENGINE->m_levelChange = true;
    ENGINE->m_nextLevel = name;
  }

  GameSpace* Engine::LuaCreateSpace(const char* name)
  {
    GameSpace* space = ENGINE->GetSpace(name);

    if (space == nullptr)
      return ENGINE->CreateSpace(name);
    else
    {
      std::string realName = name;
      realName += "[nametaken]";

      return ENGINE->LuaCreateSpace(realName.c_str());
    }
  }

  GameSpace* Engine::LuaGetSpace(const char* name)
  {
    return ENGINE->GetSpace(name);
  }

  GameComponent* Engine::LuaGetComponent(const char* name, unsigned int handle, const char* type)
  {
    GameSpace* space = (GameSpace*)ENGINE->GetSpace(name);
    // Error space not found

    EComponent cType = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
    return space->GetHandles().GetAs<GameObject>(handle)->GetComponent(cType);
  }

  void Engine::LuaRemoveObjectFromEngine(const char* space, unsigned int handle)
  {
    ENGINE->GetSpace(space)->GetHandles().GetAs<GameObject>(handle)->Destroy();
  }
  void Engine::LuaQuit()
  {
    ENGINE->Quit();
  }
  void Engine::LuaSetVariable(Generic* obj, Member* member, int dummy)
  {

  }
  void Engine::LuaGetVariable(Generic* obj, Member* member)
  {

  }
  void Engine::LuaClearSpaces()
  {
    ENGINE->ClearSpaces();
  }
  void Engine::LuaRemoveSpace(const char* name)
  {
    ENGINE->GetSpace(name)->m_valid = false;
  }
  void Engine::LuaGetComponentList(int count, int dummy)
  {

  }
}