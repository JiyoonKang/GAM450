#pragma once

extern "C"
{
  #include "lua52\include\lua.h"
  #include "lua52\include\lualib.h"
  #include "lua52\include\lauxlib.h"
}

#include "LuaBridge\LuaBridge.h"

#include <map>

#include "../hotloading/FileInfo.h"
#include "Core/components/base/Component.h"

namespace Lua
{
  class LuaComponent : public Core::GameComponent
#ifndef DEPLOYMENT
    , public FileInfo 
#endif
  {
    public:
      std::string Name;

      LuaComponent(const char *name = "");
      ~LuaComponent();

      void Initialize();
      void Remove();

      const char *getName() const;
      void setName(const char *newName);

      void loadLuaScript(lua_State *L);
      void callLuaFunction(lua_State *L, const char *function, int args = 0, int returns = 0);
      void callLuaFunction(lua_State *L, luabridge::LuaRef function, int args = 0, int returns = 0);
      void shutdownLuaScript(lua_State *L);

      static void Serialize(File& file, Variable var);
      static void Deserialize(File& file, Variable var);

      void setProp(const char *key, luabridge::LuaRef val);
      luabridge::LuaRef getProp(const char *key) const;

      // Reference handle to the function environment in the
      // registry index of the lua stack
      int RegRef;

      //COMP_LUA_USABLE;

    private:   
      std::map<const char *, luabridge::LuaRef> Vars;
      
      bool report_errors(lua_State *L, int state);
  };

#define LUA_BIND_LUACOMPONENT(luastate)\
  (luabridge::getGlobalNamespace(luastate)\
    .beginClass<LuaComponent>("LuaComponent")\
      .addProperty("propLuaNewProp__", &LuaComponent::getProp, &LuaComponent::setProp)\
      .addProperty("Name", &LuaComponent::getName, &LuaComponent::setName)\
    .endClass())
};

