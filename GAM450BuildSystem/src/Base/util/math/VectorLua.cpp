/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: VectorLua.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
//#include "VectorLua.h"
//#include "util/math/Float2.h"
//#include "util/math/Float3.h"
//#include "util/math/Float4.h"
//#include "Core/engine/lua/interface/LuaInterface.h"
//
//void Float2ToLua(lua_State* L, Base::Variable& var)
//{
//  lua_pushcfunction(L, Core::Lua::ErrorFunc); // Index 1
//
//  // Create a new table
//  lua_createtable(L, 2, 0); // Index 2
//
//  // Get the meta table
//  lua_getglobal(L, "_R"); // index 3
//  lua_getfield(L, -1, "__Vector2_MT"); // index 4
//
//  lua_getfield(L, -1, "new"); // Index 5
//
//  // Push arguments
//  lua_pushnumber(L, var.GetValue<float2>().x); // Index 6
//  lua_pushnumber(L, var.GetValue<float2>().y); // Index 7
//
//  lua_pcall(L, 2, 1, -7);
//}
//
//void Float2FromLua(lua_State* L, int index, Base::Variable* var)
//{
//  lua_getfield(L, -1, "x");
//  var->GetValue<float2>().x = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//
//  lua_getfield(L, -1, "y");
//  var->GetValue<float2>().y = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//}
//
//void Float3ToLua(lua_State* L, Base::Variable& var)
//{
//  lua_pushcfunction(L, Core::Lua::ErrorFunc); // Index 1
//
//  // Create a new table
//  lua_createtable(L, 2, 0); // Index 2
//
//  // Get the meta table
//  lua_getglobal(L, "_R"); // index 3
//  lua_getfield(L, -1, "__Vector3_MT"); // index 4
//
//  lua_getfield(L, -1, "new"); // Index 5
//
//  // Push arguments
//  lua_pushnumber(L, var.GetValue<float3>().x); // Index 6
//  lua_pushnumber(L, var.GetValue<float3>().y); // Index 7
//  lua_pushnumber(L, var.GetValue<float3>().z); // Index 8
//
//  lua_pcall(L, 2, 1, -8);
//
//}
//
//void Float3FromLua(lua_State* L, int index, Base::Variable* var)
//{
//  lua_getfield(L, -1, "x");
//  var->GetValue<float4>().x = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//
//  lua_getfield(L, -1, "y");
//  var->GetValue<float4>().y = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//
//  lua_getfield(L, -1, "z");
//  var->GetValue<float4>().z = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//}
//
//void Float4ToLua(lua_State* L, Base::Variable& var)
//{
//  lua_pushcfunction(L, Core::Lua::ErrorFunc); // Index 1
//
//  // Create a new table
//  lua_createtable(L, 2, 0); // Index 2
//
//  // Get the meta table
//  lua_getglobal(L, "_R"); // index 3
//  lua_getfield(L, -1, "__Vector4_MT"); // index 4
//
//  lua_getfield(L, -1, "new"); // Index 5
//
//  // Push arguments
//  lua_pushnumber(L, var.GetValue<float3>().x); // Index 6
//  lua_pushnumber(L, var.GetValue<float3>().y); // Index 7
//  lua_pushnumber(L, var.GetValue<float3>().z); // Index 8
//  lua_pushnumber(L, var.GetValue<float4>().w); // Index 9
//
//  lua_pcall(L, 2, 1, -9);
//}
//
//void Float4FromLua(lua_State* L, int index, Base::Variable* var)
//{
//  lua_getfield(L, -1, "x");
//  var->GetValue<float4>().x = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//
//  lua_getfield(L, -1, "y");
//  var->GetValue<float4>().y = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//
//  lua_getfield(L, -1, "z");
//  var->GetValue<float4>().z = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//
//  lua_getfield(L, -1, "w");
//  var->GetValue<float4>().w = (float)lua_tonumber(L, -1);
//  lua_pop(L, 1);
//}