/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Window.h
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once 

#include "Core/systems/System.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Base/util/math/VectorMath.h"
#include "Lua/components/LuaComponent.h"


///Basic manager for windows. Implements the windows message pump and
///broadcasts user input messages to all the systems.


class Window : public Core::ISystem
{
public:
	Window(const char* windowTitle, int ClientWidth, int ClientHeight, bool isFullScreen);	//The constructor
	~Window();															//The destructor

	void ActivateWindow();								//Activate the game window so it is actually visible
	void MinimizeWindow();

	virtual void Initialize(void) override;
	virtual void Shutdown(void) override;
	virtual void Update(float dt);						//Update the system every frame
	virtual std::string GetName() const { return "Window"; }	//Get the string name of the system


	virtual void ReceiveMessage(Core::Message& message);


	int GetScreenWidth(void);
	int GetScreenHeight(void);
	void SetScreenWidth(unsigned int width);
	void SetScreenHeight(unsigned int height);
	int GetWindowWidth(void);
	int GetWindowHeight(void);
	bool GetFocus(void);
	void SetFocus(bool focus);
	float GetAspectRatio()const;

	bool IsFullScreen()const;

	HWND hWnd;											//The handle to the game window
	HINSTANCE hInstance;								//The handle to the instance
	POINTS MousePosition;
	static bool paused;

private:

	void SwitchToFullscreen(void);
	void SwitchToWindowed(void);
	bool m_FullScreen;
	bool m_Focus;
	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;

	int m_WindowedWidth;
	int m_WindowedHeight;
	int m_FullscreenWidth;
	int m_FullscreenHeight;

};

LRESULT WINAPI MessageHandler(HWND hWnd,	 //The window the message is for (ours in this case)
	UINT msg,		 //The message type
	WPARAM wParam, //The primary data for the message
	LPARAM lParam);


class ResizeMessage : public Core::Message
{
public:
	ResizeMessage(WPARAM w, LPARAM	l)
		: Message(Core::Message::WindowResize), wParam(w), lParam(l) {}

	MSG_LUA_USABLE;

	WPARAM wParam;
	LPARAM lParam;

};

#define LUA_BIND_RESIZEMESSAGE(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.deriveClass<ResizeMessage, Core::Message>("ResizeMessage")\
	.addData("wParam", &ResizeMessage::wParam)\
	.addData("lParam", &ResizeMessage::lParam)\
	.endClass())

class KeyboardEventMessage : public Core::Message
{
public:
	KeyboardEventMessage(UINT m, LPARAM l, WPARAM w)
		: Message(KeyboardEvent), msg(m), lParam(l), wParam(w) {}

	MSG_LUA_USABLE;

	UINT msg;
	LPARAM lParam;
	WPARAM wParam;

};

#define LUA_BIND_KEYBOARDEVENTMESSAGE(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.deriveClass<KeyboardEventMessage, Core::Message>("KeyboardEventMessage")\
	.addData("wParam", &KeyboardEventMessage::wParam)\
	.addData("lParam", &KeyboardEventMessage::lParam)\
	.addData("msg", &KeyboardEventMessage::msg)\
	.endClass())

///Message signaling that the mouse has moved.
class MouseMove : public Core::Message
{
public:
	MouseMove(float2 position) : Message(MessageType::MouseMove), MousePosition(position) {};

	MSG_LUA_USABLE;

	float2 MousePosition;
};

#define LUA_BIND_MOUSEMOVE(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.deriveClass<MouseMove, Core::Message>("MouseMove")\
	.addData("MousePosition", &MouseMove::MousePosition)\
	.endClass())

class MouseButtonMessage : public Core::Message
{
public:
	enum MouseButtonIndexId
	{
		LeftMouse,
		RightMouse
	};
	MouseButtonMessage(MouseButtonIndexId button, bool state, float2 position)
		: Message(MessageType::MouseButton), MouseButtonIndex(button), ButtonIsPressed(state), MousePosition(position) {};

	MSG_LUA_USABLE;

	MouseButtonIndexId MouseButtonIndex;
	bool ButtonIsPressed;
	float2 MousePosition;
};

#define LUA_BIND_MOUSEBUTTONMESSAGE(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.deriveClass<MouseButtonMessage, Core::Message>("MouseButtonMessage")\
	.addData("MouseButtonIndex", &MouseButtonMessage::MouseButtonIndex)\
	.addData("ButtonIsPressed", &MouseButtonMessage::ButtonIsPressed)\
	.addData("MousePosition", &MouseButtonMessage::MousePosition)\
	.endClass())

class MessageMouseWheel : public Core::Message
{
public:
	MessageMouseWheel(WPARAM w) : Message(MessageType::MouseWheel), wParam(w) {};

	MSG_LUA_USABLE;

	WPARAM wParam;
};

#define LUA_BIND_MESSAGEMOUSEWHEEL(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.deriveClass<MessageMouseWheel, Core::Message>("MessageMouseWheel")\
	.addData("wParam", &MessageMouseWheel::wParam)\
	.endClass())

class MessageHorizontalWheel : public Core::Message
{
public:
	MessageHorizontalWheel(WPARAM w) : Message(MessageType::MouseHorizontal), wParam(w) {};

	MSG_LUA_USABLE;

	WPARAM wParam;
};

#define LUA_BIND_MESSAGEHORIZONTALWHEEL(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.deriveClass<MessageHorizontalWheel, Core::Message>("MessageHorizontalWheel")\
	.addData("wParam", &MessageHorizontalWheel::wParam)\
	.endClass())

#define LUA_BIND_FLOAT2(luastate) \
	(luabridge::getGlobalNamespace(luastate)\
	.beginClass<float2>("float2")\
	.addData("x", &float2::x)\
	.addData("y", &float2::y)\
	.endClass())

extern Window* WINDOWSSYSTEM;
