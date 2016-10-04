/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Hooks.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Hooks.h"
#include "Core/types/space/Space.h"

#include <utility>

namespace Core
{
  Hook::Hook() : func(), owner(Handle::null) {}

  Hook::Hook(Handle owner, const Function& fn) : func(fn), owner(owner) {}

  bool Hook::operator==(const Hook& rhs) const
  {
    return owner == rhs.owner;
  }

  //////////////////////////////////////////////////////////////////////////

  HookCollection::HookCollection(GameSpace* space) : space(space) {}
 

  HookCollection::~HookCollection()
  {
    m_hooks.clear();
  }

  void HookCollection::Add(Handle owner, const Function& fn)
  {
    // Creates a new hook
    Hook newHook(owner, fn);

    //newHook->owner = owner;

    // Inserts the hook into the map
    m_hooks[owner] = newHook;
  }

  void HookCollection::Remove(Handle owner)
  {
    auto it = m_hooks.find(owner);

    if (it != m_hooks.end())
      m_hooks.erase(it);
  }

  void HookCollection::Trigger()
  {
    if (m_hooks.size() == 0)
      return;

    // Goes through every hook in the list and pulls them
    for (auto i = m_hooks.begin(); i != m_hooks.end(); ++i)
    {
      void* data = space->GetHandles().Get(i->second.owner);

      if (data)
      {
        i->second.func.Bind(data);
        i->second.func();
      }
    
    }
  }

  //////////////////////////////////////////////////////////////////////////
  HookManager::~HookManager()
  {
    ClearAll();
  }

  void HookManager::Verify(std::string eventName)
  {
    if (HookMap.find(eventName) == HookMap.end())
    {
      HookCollection* hc = new HookCollection(space);
      HookMap[eventName] = hc;
    }
  }
  void HookManager::Add(std::string eventName, Handle owner, const Function& func)
  {
    if (reinterpret_cast<unsigned int>(&HookMap.end()) == 0xCDCDCDCD)
    {
      int crash = 10;
    }

    Verify(eventName);

    if (reinterpret_cast<unsigned int>(&HookMap.end()) == 0xCDCDCDCD)
    {
      int crash = 10;
    }

    HookMap.at(eventName)->Add(owner, func);
  }

  void HookManager::Call(std::string eventName)
  {
    if (reinterpret_cast<unsigned int>(&HookMap.end()) == 0xCDCDCDCD)
    {
      int crash = 10;
    }
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Trigger();
    }
  }

  void HookManager::Remove(std::string eventName, Handle owner)
  {
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Remove(owner);
    }
  }

  void HookManager::Clear(std::string eventName)
  {
    HookMap.at(eventName)->~HookCollection();
    HookMap.erase(HookMap.find(eventName));
  }

  void HookManager::ClearAll()
  {
    if (!HookMap.empty())
    {
      for (auto it = HookMap.begin(); it != HookMap.end(); ++it)
      {
        HookCollection* col = it->second;
        col->~HookCollection();
      }
      HookMap.clear();
    }
  }

}