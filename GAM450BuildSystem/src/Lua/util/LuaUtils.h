/**********************************************************************
Author: Wesley Pesetti
All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*********************************************************************/
#pragma once

#include <iostream>

struct lua_State;

__declspec(dllexport) void PrintIndent();

__declspec(dllexport)  void Lua_PrintTable(lua_State *L, int index);

__declspec(dllexport) void Lua_PrintIndex(lua_State *L, int index);

__declspec(dllexport) void Lua_stackDump(lua_State *L);

__declspec(dllexport) void Lua_stackTrace(lua_State *L);

void LuaStackTrace(lua_State* L);

void LuaStackDump(lua_State* L);