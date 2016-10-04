#pragma once

#include "pch/precompiled.h"

#include "Core/types/message/Message.h"

#include <map>
#include <list>
#include <queue>

namespace Lua
{
  class LuaComponent;
}

class LogicUpdate : public Core::Message
{
public:
  LogicUpdate(float Dt) : Message(MessageType::LogicUpdate), dt(Dt) {};

  MSG_LUA_USABLE;

  float dt;
};

namespace Lua
{
    class EventManager
    {
    private:
      // void * is actually GameSpace *, boost got mad at me!
      std::map<void*, std::map<int, std::map<void*, std::list<luabridge::LuaRef>>>> subscriberList;

      std::queue<Core::Message *> eventQueue;

    public:
      EventManager();
      ~EventManager();

      void PushEvent(Core::Message *message);

      bool IsEmpty() const;

      // Returns false if there are no events to process. Processed events 
      // are removed from the queue automatically.
      bool ProcessEvent(void);

      void Subscribe(LuaComponent *lComp, Core::Message::MessageType type, luabridge::LuaRef function);
      void Unsubscribe(LuaComponent *lComp, Core::Message::MessageType type, luabridge::LuaRef function);
    };
}

void __eventSubscribe(Lua::LuaComponent *lComp, int type, luabridge::LuaRef function);
void __eventUnsubscribe(Lua::LuaComponent *lComp, int type, luabridge::LuaRef function);

#define LUA_BIND_LOGICUPDATE(luastate) \
  (luabridge::getGlobalNamespace(luastate)\
    .deriveClass<LogicUpdate, Core::Message>("LogicUpdate")\
      .addData("dt", &LogicUpdate::dt)\
    .endClass())

#define LUA_BIND_EVENTMANAGER(luastate) \
  (luabridge::getGlobalNamespace(luastate)\
    .beginNamespace("Events")\
      .addFunction("Subscribe", &__eventSubscribe)\
      .addFunction("Unsubscribe", &__eventUnsubscribe)\
    .endNamespace())