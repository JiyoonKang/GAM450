/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Generic.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/modules/handle/Handle.h"
#include "Base/modules/introspection/enum/Enum.h"

using namespace Base;

namespace Core
{
  class GameSpace;

  class Generic
  {
  public:
    EGenerics genericType;

    Generic(EGenerics type) : genericType(type), space(nullptr), self() {};
    Generic() : genericType(eAbstract), space(nullptr), self() {};
    virtual ~Generic() {};

    virtual void Initialize() {};

    // Self handle
    Handle self;

    // Pointer to the space which the generic
    GameSpace* space;
  };

  struct GenericLookup
  {
    GenericLookup() {};
    GenericLookup(Handle handle, GameSpace* sp) : self(handle), space(sp) {};
  
    Handle self;
    GameSpace* space;
  };
}