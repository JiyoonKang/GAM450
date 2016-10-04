/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: LuaInterface.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "LuaInterface.h"

#include "LuaBridge/LuaBridge.h"

#include "Core/types/object/Object.h"
#include "Base/modules/introspection/manager/IntrospectionManager.h"
#include <direct.h>
#include <stdlib.h>
#include <iostream>

namespace Lua
{
  
  static int SetPath(lua_State* L, const char* path)
  {
    lua_getglobal(L, "package"); // -1
    // Gets the path field from the top of the stack -2
    lua_getfield(L, -1, "path");
    // Grab the string from the stack
    std::string cur_path = lua_tostring(L, -1);
    //cur_path.append(1,';');
    //cur_path.append(path);
    lua_pop(L, 1); // Get rid of the string on the stack -1
    lua_pushstring(L, path); // push the string -2
    // Set the field "path" in the table at -2 with the value at the top
    lua_setfield(L, -2, "path");
    lua_pop(L, 1); // Pop off the package table from the stack
    return 0;
  }

  void ReportError(lua_State* L, int status)
  {
    if (status != 0)
    {
      printf("-- %s\n", lua_tostring(L, -1));
      lua_pop(L, 1); // remove error message
    }
  }

  lua_State* CreateEnvironment(void)
  {

    char cDirectory[512]; // buffer
    std::string lDirectory;
    _getcwd(cDirectory, sizeof(cDirectory));

    //lDirectory = "LUA_PATH=";
    lDirectory = cDirectory;
    lDirectory += "\\content";

    // Creates a new lua state.
    lua_State *L = luaL_newstate();

    // Opens all standard Lua libraries into the given state.
    luaL_openlibs(L);
    luaopen_lfs(L);

    lua_pop(L, -1); // Pop the stack
    SetPath(L, lDirectory.c_str());

    RegisterClasses(L);

    LoadScript(L, "content/lua/engine/includes/interface.lua");

    luabridge::getGlobal(L, "filesystem")["LoadLuaFiles"]("content/lua/");
    luabridge::getGlobal(L, "LuaLoaded")();


    return L;
  }

  void LoadScript(lua_State* L, const char* name)
  {
    int lscript = luaL_dofile(L, name);
    ReportError(L, lscript);
  }


  void Shutdown(lua_State* L)
  {
    // Destroys all objects in the given Lua state.
    lua_close(L);
  }

}