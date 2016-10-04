/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: LuaProxy.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Core/components/base/Component.h"

namespace Lua
{
 
  class LuaProxy : public Core::GameComponent
  {
  public:
    LuaProxy();
    ~LuaProxy();

    virtual void Initialize();
    virtual void Remove();

    void QueryLoadCommand();
    void ReceiveLoadCommend(const char* command);

    std::string componentType;
    std::string loadCommand;
  };
}