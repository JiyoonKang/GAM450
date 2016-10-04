#include "pch/precompiled.h"

#include "../components/LuaComponent.h"
#include "LuaEngine.h"
#include "EventManager.h"
#include "Core\types\space\Space.h"

//MSG_LUA_DEP(LogicUpdate);
void LogicUpdate::pushToLua(lua_State *L, Lua::LuaComponent *component)
{
  //lua_rawgeti(L, LUA_REGISTRYINDEX, component->RegRef);
  luabridge::push(L, this);
  //lua_pushvalue(L, -1);
  //lua_setfield(L, -3, "event");
}

namespace Lua
{
  void EventManager::PushEvent(Core::Message *message)
  {
    eventQueue.push(message);
  }

  EventManager::EventManager()
  {

  }
  EventManager::~EventManager()
  {

  }

  // Returns false if there are no events to process. Processed events 
  // are removed from the queue automatically.
  bool EventManager::ProcessEvent(void)
  {
    if (eventQueue.size() > 0)
    {
      Core::Message *message = eventQueue.front();

      for (auto it = subscriberList.begin(); it != subscriberList.end(); ++it)
      {
        if (it->first && !reinterpret_cast<Core::GameSpace *>(it->first)->Paused())
        {
          auto messageIt = it->second.find(message->MessageId);
          if (messageIt != it->second.end())
          {
            auto messageMap = messageIt->second;
            for (auto mIt = messageMap.begin(); mIt != messageMap.end(); ++mIt)
            {
              for (auto pLIt = mIt->second.begin(), lIt = mIt->second.begin(); lIt != mIt->second.end(); ++lIt)
              {
                if (lIt->isFunction())
                {
                  message->pushToLua(gLua->L(), static_cast<LuaComponent *>(mIt->first));
                  static_cast<LuaComponent *>(mIt->first)->callLuaFunction(gLua->L(), *lIt, 1);
                }
                else if (lIt->isNil())
                {
                  // Remove the function in place here.
                  if (lIt == mIt->second.begin())
                  {
                    mIt->second.erase(lIt);
                    lIt = mIt->second.begin();
                  }
                  else
                  {
                    mIt->second.erase(lIt);
                    lIt = pLIt;
                  }
                }
                pLIt = lIt;
              }
            }
          }
        }
      }

      eventQueue.pop();
      return true;
    }
    return false;
  }

  void EventManager::Subscribe(LuaComponent *lComp, Core::Message::MessageType type, luabridge::LuaRef function)
  {
    subscriberList[lComp->space][type][lComp].push_back(function);
  }

  void EventManager::Unsubscribe(LuaComponent *lComp, Core::Message::MessageType type, luabridge::LuaRef function)
  {
    auto functionList = subscriberList[lComp->space][type][lComp];
    for (auto it = functionList.begin(); it != functionList.end(); ++it)
    {
      if (*it == function)
      {
        *it = luabridge::Nil();
        break;
      }
    }
  }

  bool EventManager::IsEmpty() const
  {
#pragma warning(disable : 4800)
#pragma warning(push)
    return !static_cast<bool>(eventQueue.size());
#pragma warning(pop)
  }
}

void __eventSubscribe(Lua::LuaComponent *lComp, int type, luabridge::LuaRef function)
{
  gLua->events.Subscribe(lComp, (Core::Message::MessageType)(type), function);
}

void __eventUnsubscribe(Lua::LuaComponent *lComp, int type, luabridge::LuaRef function)
{
  gLua->events.Unsubscribe(lComp, (Core::Message::MessageType)(type), function);
}