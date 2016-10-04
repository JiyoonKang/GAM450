/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Engine.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core/types/message/Message.h"
#include "Core/engine/factory/Factory.h"
#include "Audio/manager/AudioManager.h"

#include <stack>
#include <vector>
#include <unordered_map>

#include "GameLogic/manager/GameLogicManager.h"

namespace Core
{
  class GameWindow;
  class ISystem;
  class GameSpace;
  class Generic;
  class Base::Member;
  class Engine;

  extern Engine* ENGINE;

  class Engine
  {
  public:
    Engine();
    ~Engine();

    // Adds a system to the engine system list
    void AddSystem(ISystem* system);

    // Creates a window for the engine to use
    void MakeWindow(void* hInstance, int show, bool fullScreen);

    // Initializes all engine systems and the engine
    void Initialize(void);

    // Runs one step of the main loop
    // Conditions: Framerate is ready
    void MainLoop(void);

    // Runs one step of the game loop regardless of time
    void Step(void);

    // Prepare the engine for destruction
    void Shutdown(void);

    // Lowers the running flag
    void Quit(void);

    // Returns if the engine's running flag
    bool Running() const;


    // Sends a message to all systems in the engine
    void SystemMessage(Message& msg);

    static unsigned CurrentTime(void);


    /* ------- Lua ------ */

    const std::vector<std::string>& LuaComponentList(void) const;

    //////////////////////////////////////////////////////////////////////////
    // LEVELS

	inline void SetCurrentLevel(std::string level) { m_currentLevel = level; }

    inline std::string GetCurrentLevel() { return m_currentLevel; }

    // Loads a lua level
    void LoadLuaLevel(const char* path);

    // Changes the level at the end of the frame
    void ChangeLevel(const char* name);
    static void LuaChangeLevel(const char* name);

    //////////////////////////////////////////////////////////////////////////
    // SPACES

    // Returns a list of GameSpaces in the engine
    std::vector<GameSpace*>& Spaces(void);

    // Marks all spaces to be destroyed
    void ClearSpaces();

    // Creates a new game space
    GameSpace* CreateSpace(const char* name);

    // Retrieves a game space of a corresponding name
    GameSpace* GetSpace(const char* name);

    //////////////////////////////////////////////////////////////////////////
    // ENGINE LUA BINDS

    // Returns the lua environment
    lua_State* Lua(void) const { return L; }
    static GameSpace* LuaCreateSpace(const char* name);
    static GameSpace* LuaGetSpace(const char* name);
    static GameComponent* LuaGetComponent(const char* name, unsigned int handle, const char* type);
    static void LuaRemoveObjectFromEngine(const char* space, unsigned int handle);
    static void LuaQuit();
    static void LuaSetVariable(Generic* obj, Member* member, int dummy);
    static void LuaGetVariable(Generic* obj, Member* member);
    static void LuaClearSpaces();
    static void LuaRemoveSpace(const char* name);
    static void LuaGetComponentList(int count, int dummy);

    //////////////////////////////////////////////////////////////////////////
    // Public members
    
    // Object factory of the engine
    Factory ObjectFactory;

    bool m_editorActive = false;
    bool m_editorLights = true;

  private:

    /*---------- Cleanup -----------*/

    // Destroys any old spaces
    void CleanUp(void);

    // Actually destroys a game space [ENGINE USE ONLY]
    void RemoveSpace(GameSpace* space);

    /*---------- Levels -----------*/

    // Actually unloads the current level and loads a new one [ENGINE USE ONLY]
    void GotoNextLevel(void);

    // Loads any new lua levels
    void LoadLuaLevels(void);

    //////////////////////////////////////////////////////////////////////////
    // Private members

    /*---------- Level Data -----------*/

    // Name of the next level to load
    std::string m_nextLevel;

    std::string m_currentLevel;
    bool m_levelChange = false;

    /*---------- System Data ----------*/

    // Engine is running
    bool m_running;

    // All the systems
    std::vector<ISystem*> m_systems;

    // Map & Vector of all the spaces
    std::unordered_map<std::string, GameSpace*> m_spaceMap;
    std::vector<GameSpace*> m_spaces;
    std::vector<GameSpace*> m_spaceRemoveList;

    unsigned int m_time;


    /*---------- Lua Data ----------*/

    // The Lua environment
    lua_State* L;

    // Lua components
    std::vector<std::string> m_luaComponentList;

    // Lua Level Stack
    std::stack<std::string> m_luaLevelsToLoad;
    bool m_loadLuaLevels = false;

    friend class GameLogic::GameLogicManager;
    friend class GameSpace;
    friend class Factory;
    //friend class FramerateController;
   



  };

}