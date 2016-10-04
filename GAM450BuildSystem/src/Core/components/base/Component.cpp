/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Component.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Component.h"

namespace Core
{
  using namespace Base;

  bool GameComponent::operator==(const GameComponent& rhs) const
  {
    // Get the type of component
    const TypeInfo* lTypeData = FACTORY->GetComponentType(this->typeID);
    const TypeInfo* rTypeData = FACTORY->GetComponentType(rhs.typeID);

    if (lTypeData != rTypeData)
      return false;

    Member curMem;
    for (unsigned int i = 0; i < lTypeData->GetMembers().size(); ++i)
    {
      curMem = lTypeData->GetMembers()[i];

      const TypeInfo* memberType = curMem.Type();

      if (memberType == GET_TYPE(std::string))
      {
        std::string& lhss = Variable(memberType, (char*)this + curMem.Offset()).GetValue<std::string>();
        std::string& rhss = Variable(memberType, (char*)&rhs + curMem.Offset()).GetValue<std::string>();

        if (lhss == rhss)
          continue;
        else
          return false;
      }

      if (memcmp((char*)this + curMem.Offset(), (char*)&rhs + curMem.Offset(), curMem.Type()->Size()))
        return false;
    }

    return true;
  }

  bool GameComponent::operator!=(const GameComponent& rhs) const
  {
    return !operator==(rhs);
  }

  GameComponent& GameComponent::operator=(const GameComponent& rhs)
  {
    const TypeInfo* lTypeData = FACTORY->GetComponentType(this->typeID);
    const TypeInfo* rTypeData = FACTORY->GetComponentType(rhs.typeID);

    if (lTypeData != rTypeData)
      return *this;

    //Member curMem;
    for (unsigned int i = 0; i < lTypeData->GetMembers().size(); ++i)
    {
      // Get the actual member
      const Member member = lTypeData->GetMembers()[i];

      // Copy over the member data from the object into the archetype
      Variable LVar = Variable(member.Type(), (char*)this + member.Offset());
      Variable RVar = Variable(member.Type(), (char*)this + member.Offset());

      member.Type()->Copy(LVar.GetData(), RVar.GetData());
    }

    return *this;
  }

  GameObject* GameComponent::GetOwner()
  {
    return space->GetHandles().GetAs<GameObject>(owner);
  }

}