/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: LuaInterface.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

namespace Lua
{
  // Create a new lua environment
  lua_State* CreateEnvironment(void);

  // Loads a lua file via path name
  void LoadScript(lua_State* L, const char* name);

  // Function called when lua error occurs
  void ReportError(lua_State* L, int status);

  void RegisterClasses(lua_State* L);

  void Shutdown(lua_State* L);
}