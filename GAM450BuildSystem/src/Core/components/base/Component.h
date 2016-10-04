/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Component.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Core/types/generic/Generic.h"

#include "Lua/pch/precompiled.h"

using namespace Base;

namespace Core
{
  class GameSpace;
  class Factory;

  extern Factory* FACTORY;

  class GameComponent : public Generic
  {
    public:
      friend class GameObject;
      friend class TypeInfo;

      GameComponent() : Generic(eGameComponent) {};
      virtual ~GameComponent() {};   
      virtual GameComponent& operator=(const GameComponent& rhs);
      virtual bool operator==(const GameComponent& rhs) const;
      bool operator!=(const GameComponent& rhs) const;

      virtual void pushToLua(lua_State *L) {};

      virtual void Initialize() {};
      virtual void Remove() {};
      GameObject* GetOwner();

	    // The handle to the owner of this component
      Handle owner;

      // Each type of component has it's own unique ID
      EComponent typeID;
      
      const TypeInfo* typeinfo;

  private:
    bool m_toDelete = false;
  };
};

#define COMP_LUA_USABLE virtual void pushToLua(lua_State *L)

#define COMP_LUA_DEP(CLASS) \
  void CLASS::pushToLua(lua_State *L) \
{ \
  luabridge::push(L, this); \
}
