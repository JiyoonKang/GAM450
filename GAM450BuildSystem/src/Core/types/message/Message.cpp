/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Message.h 
 * Author: Austin Jensen
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Message.h"
#include "Lua/components/LuaComponent.h"

namespace Core
{
    void Message::pushToLua(lua_State *L, Lua::LuaComponent *component) {};

    MSG_LUA_DEP(MessageUI)
}