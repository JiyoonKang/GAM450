/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: LuaDefaultBinds.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "Lua/pch/precompiled.h"

#include "Base/util/Macros.h"
#include "Core/components/transform/CTransform.h"
#include "Core/types/object/Object.h"
#include "Core/types/space/Space.h"
#include "Core/engine/core/Engine.h"
#include "Lua/components/LuaProxy.h"

#include "GameLogic/Sandbox.h"

// Register all classes and functions to lua
// NOTE: LuaBridge doesn't support overloaded functions

namespace Lua
{
  void RegisterClasses(lua_State *L)
  {
    luabridge::getGlobalNamespace(L).beginNamespace("engine")
      .beginClass<Core::Engine>("Engine")
      .endClass().endNamespace();

    luabridge::getGlobalNamespace(L).beginNamespace("Classes")
      .beginClass<Core::GameObject>("GameObject")
      .addFunction("Destroy", &Core::GameObject::Destroy)
      .addFunction("GetName", &Core::GameObject::GetName)
      .addFunction("GetArchetype", &Core::GameObject::GetArchetype)
      .addFunction("AddComponent", &Core::GameObject::AddComponent)
      //.addFunction("RemoveComponent", &Core::RemoveComponent)
      //.addFunction("GetComponent", &Core::GameObject::GetComponent)
      .addData("guid", &Core::GameObject::guid)
      .addData("name", &Core::GameObject::name)
      .addData("hooks", &Core::GameObject::hooks)
      .endClass().endNamespace();

    luabridge::getGlobalNamespace(L).beginNamespace("Classes")
      .beginClass<Core::GameSpace>("GameSpace")
      .addFunction("SetPaused", &Core::GameSpace::SetPaused)
      .addFunction("Paused", &Core::GameSpace::Paused)
      .addFunction("SetHidden", &Core::GameSpace::SetHidden)
      .addFunction("Hidden", &Core::GameSpace::Hidden)
      .addFunction("Ready", &Core::GameSpace::Ready)
      .addFunction("Valid", &Core::GameSpace::Valid)
      //.addFunction("GetName", &Core::GameSpace::GetName)
      .addFunction("Destroy", &Core::GameSpace::Destroy)
      .addFunction("GUID", &Core::GameSpace::GUID)
      .addFunction("GetHandles", &Core::GameSpace::GetHandles)
      .addFunction("GetComponents", &Core::GameSpace::GetComponent)
      .addFunction("CreateEmptyObject", &Core::GameSpace::CreateEmptyObject)
      .addFunction("CreateObjectFromArchetype", &Core::GameSpace::CreateObjectFromArchetype)
      .addFunction("Clear", &Core::GameSpace::Clear)
      .addData("hooks", &Core::GameSpace::hooks)
      .addData("fileName", &Core::GameSpace::m_fileName)
      .endClass().endNamespace();

    // Register Components

    //////////////////////////////////////////////////////////////////////////
    // Test
    luabridge::getGlobalNamespace(L)
      .beginClass<Object>("Object")
      .addConstructor<void(*) (std::string)>()
      .addFunction("GetName", &Object::GetName)
      .addFunction("SetName", &Object::SetName)
      .addFunction("PrintTest", &Object::PrintTest)
      .addFunction("SetNum", &Object::SetNum)
      .addFunction("GetNum", &Object::GetNum)
      .addData("value", &Object::value)
      .endClass();
  }
}