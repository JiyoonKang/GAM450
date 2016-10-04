/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: LuaProxy.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "LuaProxy.h"
#include "Lua/LuaEngine.h"

#include "Core/engine/core/Engine.h"

namespace Lua
{
  LuaProxy::LuaProxy() {}

  LuaProxy::~LuaProxy() {}

  void LuaProxy::Initialize()
  {
#if(0)
    luabridge::getGlobal(Core::ENGINE->Lua(), "AttachComponentToObject")(space->GetName().c_str(), 
      owner.operator unsigned(), self.operator unsigned(), componentType.c_str());

    if (loadCommand.length() > 0)
    {
      luabridge::getGlobal(Core::ENGINE->Lua(), "DeserializeComponent")(space->GetName().c_str(), 
        owner.operator unsigned(), self.operator unsigned(), loadCommand.c_str());
    }
#endif
  }
  
  void LuaProxy::Remove()
  {
    if (space != nullptr)
      luabridge::getGlobal(gLua->L(), "RemoveComponentFromGameObject")(space->GetName().c_str(), 
      owner.operator size_t(), self.operator size_t());

    componentType.~basic_string();
    loadCommand.~basic_string();
  }

  void LuaProxy::QueryLoadCommand()
  {
    luabridge::getGlobal(gLua->L(), "SerializeComponent")(space->GetName().c_str(),
      owner.operator unsigned(), self.operator unsigned(), this);
  }

  void LuaProxy::ReceiveLoadCommend(const char* command)
  {
    loadCommand = command;
  }
}
