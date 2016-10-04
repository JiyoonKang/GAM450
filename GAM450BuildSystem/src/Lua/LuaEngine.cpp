#include "pch/precompiled.h"

#include "lua52/include/lj_obj.h"
#include "LuaEngine.h"
#include "core/types/message/Message.h"
#include "components/LuaComponent.h"
#include "eventmanager/EventManager.h"
#include "../Core/engine/factory/Factory.h"

#include <stdio.h>

// Binding Includes!
#include "Core/systems/window/Window.h"

namespace Lua
{
  int l_print(lua_State *L)
  {
    int top = lua_gettop(L);

    for (int i = 1; i <= top; i++)
    {
      if (lua_istable(L, i))
        Lua_PrintTable(L, i);
      else
        Lua_PrintIndex(L, i);
    }

    putchar('\n');

    return 0;
  }

  static int l_panic(lua_State *L)
  {
    printf("Panic Error: %s, Lua could not recover!\n", lua_tostring(L, lua_gettop(L)));
    __debugbreak();
    return 0;
  }

  static const struct luaL_Reg utilLib[] =
  {
    { "print", l_print },
    { NULL, NULL } // Sentinel Entry
  };

  LuaEngine::LuaEngine() : ISystem(), Singleton<LuaEngine>(), m_Path(std::string(""))
  {
    Initialize();
  }

  LuaEngine::~LuaEngine()
  {
    //Shutdown(); // Note to Austin: This was crashing the game on exit, because
                  // Shutdown gets called by the engine before destruction.
                  // You should check here to close lua only if it's not already closed.
  }

  lua_State *LuaEngine::L() { return m_L; }

  void LuaEngine::Initialize()
  {
    m_L = luaL_newstate();

    // Open the standard libraries for lua (math, table, string)
    luaL_openlibs(m_L);

    // Register the utility functions in utilLib statically
    // to the global Lua environment and then clean up the stack.
    lua_getglobal(m_L, "_G");
    luaL_register(m_L, nullptr, utilLib);
    lua_pop(m_L, 1);

    // Set the function to handle unrecoverable lua errors
    lua_atpanic(m_L, &l_panic);

    DefaultLuaRegistration();

    SetPath("../bin/content/lua/scripts/");
    // Stop the auto-garbage collector to manually run the cycles
    // lua_gc(m_L, LUA_GCSTOP, 0);
  }

  void LuaEngine::Update(float dt)
  {
    // Run a garbage collection cycle
    // lua_gc(m_L, LUA_GCCOLLECT, 0);
    LogicUpdate updateMessage(dt);
    events.PushEvent(&updateMessage);

    while (!events.IsEmpty())
    {
      events.ProcessEvent();
    }
  }

  void LuaEngine::Shutdown()
  {
    lua_close(m_L);
  }

  void LuaEngine::ReceiveMessage(Core::Message& msg)
  {
    events.PushEvent(&msg);
    events.ProcessEvent();
  }

  std::string LuaEngine::GetName() const { return std::string("Lua"); }

  const void *LuaEngine::GetDebugData() { return NULL; }

  std::string& LuaEngine::GetPath() { return m_Path; }

  void LuaEngine::SetPath(const char *path) { m_Path = std::string(path); }

  void LuaEngine::DefaultLuaRegistration()
  {
    LUA_BIND_FLOAT2(m_L);

    LUA_BIND_LUACOMPONENT(m_L);

    LUA_BIND_EVENTDEFS(m_L);
    LUA_BIND_EVENTMANAGER(m_L);
    LUA_BIND_MESSAGE(m_L);
    LUA_BIND_LOGICUPDATE(m_L);
    LUA_BIND_RESIZEMESSAGE(m_L);
    LUA_BIND_KEYBOARDEVENTMESSAGE(m_L);
    LUA_BIND_MOUSEMOVE(m_L);
    LUA_BIND_MOUSEBUTTONMESSAGE(m_L);
    LUA_BIND_MESSAGEMOUSEWHEEL(m_L);
    LUA_BIND_MESSAGEHORIZONTALWHEEL(m_L);
  }

  void LuaEngine::AddInitToList(LuaComponent *luaComp)
  {
    initList.push_back(luaComp);
  }

  void LuaEngine::InitPhase()
  {
    LuaComponent *lComp = nullptr;
    for (auto it = initList.begin(); it != initList.end(); ++it)
    {
      lComp = *it;
      if (lComp)
      {
        lComp->Initialize();
      }
    }

    initList.clear();
  }

  void LuaEngine::RegisterComponents(void)
  {
    REGISTER_COMPONENT(LuaComponent, LuaComponent);
  }
}