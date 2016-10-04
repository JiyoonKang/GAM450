#pragma once

#include "core/systems/System.h"

#include <vector>
#include "util/Singleton.h"
#include "util/LuaUtils.h"
#include "eventmanager\EventManager.h"

struct lua_State;
class Core::Message;

namespace Lua
{
  int l_print(lua_State *L);

  class LuaEngine : public Core::ISystem, public Singleton<LuaEngine>
  {
    public:
      LuaEngine();
      ~LuaEngine();

      lua_State *L();
      void RegisterComponents();
      void Initialize();
      void Update(float dt);
      void ReceiveMessage(Core::Message& msg);
      std::string GetName() const;
      void Shutdown();
      void InitPhase();
      void AddInitToList(LuaComponent *luaComp);
      const void *GetDebugData();
      std::string& GetPath();
      void SetPath(const char *path);
      void DefaultLuaRegistration();

      EventManager events;

    private:
        lua_State *m_L;
        std::string m_Path;
        std::vector<LuaComponent *> initList;
  };
}

#define gLua Lua::LuaEngine::GetSingletonPtr()