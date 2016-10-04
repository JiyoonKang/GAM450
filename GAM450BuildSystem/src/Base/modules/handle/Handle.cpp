/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Handle.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Handle.h"
#include "Base/util/Macros.h"
#include "Core/types/space/Space.h"
#include "Core/types/generic/Generic.h"
#include "Core/components/base/Component.h"

namespace Base
{
  const Handle Handle::null;

  Handle::Handle()
    : m_index(static_cast<size_t>(-1)), m_counter(static_cast<size_t>(-1)) {}

  Handle::Handle(unsigned index, unsigned counter)
    : m_index(index), m_counter(counter) {}

  Handle::Handle( unsigned handle )
  {
    *((unsigned *)this) = handle;
  }

  void Handle::Serialize(File& file, Variable var)
  {
#if(0)
    Handle& h = var.GetValue<Handle>();
    Core::GameSpace* space = (Core::GameSpace*)Serializer::Get()->GetUserData();
    Core::Generic* data = space->GetHandles().GetAs<Core::Generic>(h);

    Core::GameComponent* comp;
    const TypeInfo* type = GET_TYPE(int);

    if (data == nullptr)
      return;
    
    // From the generic type we can establish what sub type it is
    switch (data->genericType)
    {
    case eGameObject:
      // If it's an object, that's simple
      type = GET_TYPE(Core::GameObject);
      break;
    case eGameComponent:
      // Components take a little more work
      comp = static_cast<Core::GameComponent*>(data);
      type = GET_STR_TYPE(GET_ENUM(Core::Component)->m_literals[comp->typeID].c_str());
      break;
    }

    // Establish a variable, and serialize
    Variable oVar(type, data);
    oVar.Serialize(file);
#endif
  }
}