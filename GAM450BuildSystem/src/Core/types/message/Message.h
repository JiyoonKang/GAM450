/*******************************************************************************
 * All content � 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Message.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include <string>
#include "Base\modules\handle\Handle.h"

struct lua_State;
namespace Lua
{  
  class LuaComponent;
}

namespace Core
{
	// Base message class. You can make your own message types by deriving from this
	// and then added an ID to the MessageIDType enum
	class Message
	{
   public:
    enum MessageType
    {
      None,
      GFXDeviceInit,
      PostDraw,
      GUIDraw,
      PostGUIDraw,
      WindowResize,
      WindowMinimize,
      WindowRestore,
      CameraChange,
      EngineReady,
      KeyboardEvent,
      MouseMove,
      MouseButton,
      MouseWheel,
      MouseHorizontal,
      UIMessage,
      CollisionEnter,
      Collision,
      CollisionExit,
      Pause,
      Quit,
      LogicUpdate
    };

    Message(MessageType id) : MessageId(id) {};
		virtual ~Message() {};

    virtual void pushToLua(lua_State *L, Lua::LuaComponent *component);

    MessageType MessageId;
  };

#define LUA_BIND_EVENTDEFS(luastate) \
  static int __None            = Core::Message::MessageType::None; \
  static int __GFXDeviceInit   = Core::Message::MessageType::GFXDeviceInit; \
  static int __PostDraw        = Core::Message::MessageType::PostDraw; \
  static int __GUIDraw         = Core::Message::MessageType::GUIDraw; \
  static int __PostGUIDraw     = Core::Message::MessageType::PostGUIDraw; \
  static int __WindowResize    = Core::Message::MessageType::WindowResize; \
  static int __WindowMinimize  = Core::Message::MessageType::WindowMinimize; \
  static int __WindowRestore   = Core::Message::MessageType::WindowRestore; \
  static int __CameraChange    = Core::Message::MessageType::CameraChange; \
  static int __EngineReady     = Core::Message::MessageType::EngineReady; \
  static int __KeyboardEvent   = Core::Message::MessageType::KeyboardEvent; \
  static int __MouseMove       = Core::Message::MessageType::MouseMove; \
  static int __MouseButton     = Core::Message::MessageType::MouseButton; \
  static int __MouseWheel      = Core::Message::MessageType::MouseWheel; \
  static int __MouseHorizontal = Core::Message::MessageType::MouseHorizontal; \
  static int __UIMessage       = Core::Message::MessageType::UIMessage; \
  static int __Pause           = Core::Message::MessageType::Pause; \
  static int __Quit            = Core::Message::MessageType::Quit; \
  static int __LogicUpdate     = Core::Message::MessageType::LogicUpdate; \
  \
  luabridge::getGlobalNamespace(luastate)\
    .beginNamespace("Events")\
      .addVariable("None", &__None)\
      .addVariable("GFXDeviceInit", &__GFXDeviceInit)\
      .addVariable("PostDraw", &__PostDraw)\
      .addVariable("GUIDraw", &__GUIDraw)\
      .addVariable("PostGUIDraw", &__PostGUIDraw)\
      .addVariable("WindowResize", &__WindowResize)\
      .addVariable("WindowMinimize", &__WindowMinimize)\
      .addVariable("WindowRestore", &__WindowRestore)\
      .addVariable("CameraChange", &__CameraChange)\
      .addVariable("EngineReady", &__EngineReady)\
      .addVariable("KeyboardEvent", &__KeyboardEvent)\
      .addVariable("MouseMove", &__MouseMove)\
      .addVariable("MouseButton", &__MouseButton)\
      .addVariable("MouseWheel", &__MouseWheel)\
      .addVariable("MouseHorizontal", &__MouseHorizontal)\
      .addVariable("UIMessage", &__UIMessage)\
      .addVariable("Pause", &__Pause)\
      .addVariable("Quit", &__Quit)\
      .addVariable("LogicUpdate", &__LogicUpdate)\
    .endNamespace()

#define MSG_LUA_USABLE virtual void pushToLua(lua_State *L, Lua::LuaComponent *component)

#define MSG_LUA_DEP(CLASS) \
  void CLASS::pushToLua(lua_State *L, Lua::LuaComponent *component) \
  { \
    lua_rawgeti(L, LUA_REGISTRYINDEX, component->RegRef); \
    luabridge::push(L, *this); \
    lua_pushvalue(L, -1); \
    lua_setfield(L, -3, "event"); \
  }

  class MessageUI : public Message
  {
	public:
		  MessageUI(const std::string & m) : Message(MessageType::UIMessage), msg(m) {}
		  
      MSG_LUA_USABLE;

      std::string msg;
	};

  class CollisionEnterMessage : public Message
  {
    public:
      CollisionEnterMessage(const Base::Handle& a, const Base::Handle& b) :
        Message(MessageType::CollisionEnter),
        body(a), other(b)
      {
      };

      Base::Handle body;
      Base::Handle other;
  };

  class CollisionMessage : public Message
  {
  public:
    CollisionMessage(const Base::Handle& a, const Base::Handle& b) :
      Message(MessageType::Collision),
      body(a), other(b)
    {
    };

    Base::Handle body;
    Base::Handle other;
  };

  class CollisionExitMessage : public Message
  {
  public:
    CollisionExitMessage(const Base::Handle& a, const Base::Handle& b) :
      Message(MessageType::CollisionExit),
      body(a), other(b)
    {
    };

    Base::Handle body;
    Base::Handle other;
  };
}

#define LUA_BIND_MESSAGE(luastate) \
  luabridge::getGlobalNamespace(luastate)\
    .beginClass<Core::Message>("Message")\
      .addData("MessageId", (unsigned Core::Message::*)(&Core::Message::MessageId), false)\
    .endClass()