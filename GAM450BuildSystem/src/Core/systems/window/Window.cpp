/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Window.cpp
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Window.h"

#include <shellapi.h>
#include "Base/util/debug/DebugAssist.h"
#include "Core/systems/input/Input.h"
#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>

#include <sstream>

#include "GameLogic/GameLogicSystem.h"
#include "Start/config/Config.h"
#include "SkyeAwesomium/includes/SkyeAwesomium/SkyeAwesomium.h"
#include "physics/core/PhysicsSystem.h"


bool Window::paused = false;
//#include <DebugAssist\DebugAssist.h>


auto s_WindowStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
//fake commment


//A global pointer to the windows system
Window* WINDOWSSYSTEM = NULL;

//Used by windows to identify our window class type
//(it's needed for registering/creating/unregistering the window)
LPCSTR windowsClassName = "Skye Engine";

Window::Window(const char* windowTitle, int ClientWidth, int ClientHeight, bool isFullScreen) :
m_FullScreen(isFullScreen),
m_Focus(true),
m_ScreenWidth(ClientWidth),
m_ScreenHeight(ClientHeight), Core::ISystem()
{
	////Check to make sure the windows system is created before the factory
	//ErrorIf(FACTORY != NULL, "Windows System already created");

	//Set the global pointer to the windows system
	WINDOWSSYSTEM = this;

	m_WindowedWidth = ClientWidth;
	m_WindowedHeight = ClientHeight;
	m_FullscreenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_FullscreenHeight = GetSystemMetrics(SM_CYSCREEN);

	windowsClassName = windowTitle;

	//The size passed to CreateWindow is the full size including the windows border and caption 
	//AdjustWindowRect will adjust the provided rect so that the client size of the window is the desired size
	RECT fullWinRect = { 0, 0, ClientWidth, ClientHeight };
	AdjustWindowRect(&fullWinRect,			//The rectangle for the full size of the window
		s_WindowStyle,	//The style of the window, which must match what is passed in to CreateWindow below
		FALSE);					//Does this window have a menu?

	//Register the window class for the game.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),	//The size of this structure (passing the size allows Microsoft to update their interfaces and maintain backward compatibility)
		CS_CLASSDC,							//The style of the window class--this is the base type (one device context for all windows in the process)
		MessageHandler,						//The name of the message handling function
		0L, 0L,								//The amount of extra memory to allocate for this class and window
		GetModuleHandle(NULL),				//Handle to the instance that has the windows procedure--NULL means use this file.
		NULL
		/*(HICON)LoadImage(NULL,
		"Textures/BrightLightFight.ico",
		IMAGE_ICON, 256, 256,
		LR_LOADFROMFILE)*/,					//Add an Icon as a resource and add them here
		LoadCursor(NULL, IDC_ARROW),		//Use the default arrow cursor
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL,								//The background brush and menu--these can be NULL
		windowsClassName, NULL };			//The class name and the small icon (NULL just uses the default)

	RegisterClassEx(&wc);

	//Store the handle to the instance
	hInstance = wc.hInstance;

	if (m_FullScreen)
	{
		//Create the game's window
		hWnd = CreateWindow(windowsClassName,	//The class name
			windowsClassName,					//The name for the title bar
			WS_POPUP,							//The style of the window (WS_BORDER, WS_MINIMIZEBOX, WS_MAXIMIZE, etc.)
			CW_USEDEFAULT, CW_USEDEFAULT,		//The x and y position of the window (screen coords for base windows, relative coords for child windows)
			GetSystemMetrics(SM_CXSCREEN),		//Width of the window, including borders
			GetSystemMetrics(SM_CYSCREEN),		//Height of the window, including borders and caption
			GetDesktopWindow(),					//The parent window
			NULL,								//The menu for the window
			hInstance,							//The handle to the instance of the window (ignored in NT/2K/XP)
			NULL);								//The lParam for the WM_CREATE message of this window	
	}
	else
	{
		//Create the game's window
		hWnd = CreateWindow(windowsClassName,	//The class name
			windowsClassName,					//The name for the title bar
			s_WindowStyle,						//The style of the window (WS_BORDER, WS_MINIMIZEBOX, WS_MAXIMIZE, etc.)
			m_FullscreenWidth / 2 - (m_WindowedWidth / 2),
			m_FullscreenHeight / 2 - (m_WindowedHeight / 2), //The x and y position of the window (screen coords for base windows, relative coords for child windows)
			m_WindowedWidth,	//Width of the window, including borders
			m_WindowedHeight,	//Height of the window, including borders and caption
			GetDesktopWindow(),					//The parent window
			NULL,								//The menu for the window
			hInstance,							//The handle to the instance of the window (ignored in NT/2K/XP)
			NULL);								//The lParam for the WM_CREATE message of this window
	}

	DragAcceptFiles(hWnd, true);

}

Window::~Window()
{
	WINDOWSSYSTEM = NULL;
	//Unregister the window class
	UnregisterClass(windowsClassName, hInstance);
}

void Window::Initialize(void)
{
	PrintOut("Window Initialize");
	ActivateWindow();
}

void Window::Shutdown(void)
{

}

//Process any windows messages and run the game until we get a quit message
//While we don't use the window handle, in other cases we might want to only process messages for this window
void Window::Update(float dt)
{
	MSG msg;
	//Look for any pending windows messages, remove them, then handle them
	//The second parameter is the window handle--NULL just means get any message from the current thread
	//The third and forth parameters are the start and end message types to process
	//The last parameter determines whether or not the message is removed
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) //It is important to get all windows messages available not just one
	{
		TranslateMessage(&msg);	//Makes sure WM_CHAR and similar messages are generated
		DispatchMessage(&msg);	//Calls the message procedure (see below) with this message

		//If we get a quit message, broadcast it to all systems
		if (msg.message == WM_QUIT)
		{
			exit(-1);
		}
	}
}

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//The message handling procedure for the game
LRESULT WINAPI MessageHandler(HWND hWnd,	 //The window the message is for (ours in this case)
	UINT msg,		 //The message type
	WPARAM wParam, //The primary data for the message
	LPARAM lParam) //The secondary data for the message (if any)
{

	ImGuiIO& io = ImGui::GetIO();

	Input::InputHandler::SetKeyInput(true);

	//Update imgui inputs
	if (ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		//if imgui wants to capture input, disable input for the rest of the application
		if (io.WantCaptureMouse || io.WantCaptureKeyboard || io.WantTextInput)
		{

			Input::KeyboardEvent::Clear();
			Input::MouseEvent::SetLeftMouseButton(false);
			Input::MouseEvent::SetMiddleMouseButton(false);
			Input::MouseEvent::SetRightMouseButton(false);
			Input::InputHandler::SetKeyInput(false);
			return true;
		}
	}

	//See what type of message it is
	switch (msg)
	{
	case WM_CHAR: //A character key was pressed
	{
					  Core::ENGINE->SystemMessage(KeyboardEventMessage(WM_CHAR, lParam, wParam));
					  break;
	}

	case WM_ACTIVATE:
	{
						if (GameLogic::LevelID::Invalid == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel())
							break;
						switch (wParam)
						{
						case WA_INACTIVE:
						{
											WINDOWSSYSTEM->SetFocus(false);

											if (Config::installer)
											{
												WINDOWSSYSTEM->MinimizeWindow();

												if (!GameLogic::GAMELOGIC_SYSTEM->GetPaused())
												{
													if (!(GameLogic::LevelID::Invalid == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() || GameLogic::LevelID::Menu == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel()))
													{
														Core::MessageUI goToPause("ChangeUI PauseMenu");
														Core::ENGINE->SystemMessage(goToPause);

													}
													Core::MessageUI pauseGame("Action Pause");

													Core::ENGINE->SystemMessage(pauseGame);

													Audio::AUDIOMANAGER->PauseAll();
												}
											}

											if (!Window::paused)
											{
												Audio::AUDIOMANAGER->PauseAll();
												Window::paused = true;
											}

											break;
						}

						case WA_ACTIVE:
						{
										  WINDOWSSYSTEM->SetFocus(true);
										  //WindowFocus m(WINDOWSSYSTEM->GetFocus());
										  if (Config::installer)
											  WINDOWSSYSTEM->ActivateWindow();
										  //CORE->BroadcastMessage(&m);
										  if (!GameLogic::GAMELOGIC_SYSTEM->GetPaused())
										  {
											  Audio::AUDIOMANAGER->ResumeAll();

											  Window::paused = false;
										  }
										  if ((GameLogic::LevelID::Invalid == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() || GameLogic::LevelID::Menu == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel()))
										  {
											  Core::MessageUI goToPause("ChangeUI MainMenu");
											  Core::ENGINE->SystemMessage(goToPause);
											  Core::MessageUI unPauseGame("Action unPause");
											  Core::ENGINE->SystemMessage(unPauseGame);
										  }
										  if (GameLogic::LevelID::Menu == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel())
										  {
											  AWESOMIUM->RestartMenu();
										  }
										  break;
						}

						case WA_CLICKACTIVE:
						{
											   WINDOWSSYSTEM->SetFocus(true);
											   //WindowFocus m(WINDOWSSYSTEM->GetFocus());
											   if (Config::installer)
												   WINDOWSSYSTEM->ActivateWindow();
											   //CORE->BroadcastMessage(&m);
											   if (!GameLogic::GAMELOGIC_SYSTEM->GetPaused())
											   {
												   Audio::AUDIOMANAGER->ResumeAll();
												   Window::paused = false;
											   }
											   if ((GameLogic::LevelID::Invalid == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() || GameLogic::LevelID::Menu == GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel()))
											   {
												   Core::MessageUI goToPause("ChangeUI MainMenu");
												   Core::ENGINE->SystemMessage(goToPause);
												   Core::MessageUI unPauseGame("Action unPause");
												   Core::ENGINE->SystemMessage(unPauseGame);
											   }
											   break;
						}
						}
						break;
	}

	case WM_LBUTTONDOWN:
	{

						   Input::MouseEvent::SetLeftMouseButton(true);
						   MouseButtonMessage m(MouseButtonMessage::LeftMouse, true, float2(WINDOWSSYSTEM->MousePosition.x, WINDOWSSYSTEM->MousePosition.y));
						   Core::ENGINE->SystemMessage(m);
						   break;
	}
	case WM_MBUTTONDOWN:
	{
						   Input::MouseEvent::SetMiddleMouseButton(true);
						   break;
	}
	case WM_RBUTTONDOWN:
	{
						   Input::MouseEvent::SetRightMouseButton(true);
						   MouseButtonMessage m(MouseButtonMessage::RightMouse, true, float2(WINDOWSSYSTEM->MousePosition.x, WINDOWSSYSTEM->MousePosition.y));
						   Core::ENGINE->SystemMessage(m);

						   break;
	}

	case WM_LBUTTONUP:
	{
						 Input::MouseEvent::SetLeftMouseButton(false);
						 MouseButtonMessage m(MouseButtonMessage::LeftMouse, false, float2(WINDOWSSYSTEM->MousePosition.x, WINDOWSSYSTEM->MousePosition.y));

						 Core::ENGINE->SystemMessage(m);
						 break;
	}
	case WM_MBUTTONUP:
	{
						 Input::MouseEvent::SetMiddleMouseButton(false);
						 break;
	}
	case WM_RBUTTONUP:
	{
						 Input::MouseEvent::SetRightMouseButton(false);
						 MouseButtonMessage m(MouseButtonMessage::RightMouse, false, float2(WINDOWSSYSTEM->MousePosition.x, WINDOWSSYSTEM->MousePosition.y));

						 Core::ENGINE->SystemMessage(m);
						 break;
	}

	case WM_MOUSEMOVE:
	{
						 Input::MouseEvent::SetMousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

						 WINDOWSSYSTEM->MousePosition = MAKEPOINTS(lParam);
						 Core::ENGINE->SystemMessage(MouseMove(float2(WINDOWSSYSTEM->MousePosition.x, WINDOWSSYSTEM->MousePosition.y)));
						 break;
	}

	case WM_MOUSEWHEEL:
	{
						  Input::MouseEvent::SetScollDelta(WHEEL_DELTA * HIWORD(wParam));
						  MessageMouseWheel m(GET_WHEEL_DELTA_WPARAM(wParam));

						  Core::ENGINE->SystemMessage(m);
						  break;
	}

	case WM_MOUSEHWHEEL:
	{
						   MessageHorizontalWheel m(GET_WHEEL_DELTA_WPARAM(-wParam));
						   Core::ENGINE->SystemMessage(m);
						   break;
	}
	case WM_HSCROLL:
	{
					   MessageHorizontalWheel m(GET_WHEEL_DELTA_WPARAM(-wParam));
					   Core::ENGINE->SystemMessage(m);
					   break;
	}

	case WM_KEYDOWN:
	{
					   switch (wParam)
					   {
						   //Ignore when TAB is pressed
					   case VK_TAB:
					   {
									  break;
					   }

					   case VK_ESCAPE:
					   {
										 Input::KeyboardEvent::SetKey(wParam, true);

										 //Core::ENGINE->SystemMessage(KeyboardEventMessage(WM_KEYDOWN, lParam, wParam));
										 if (!(GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() > GameLogic::LevelID::Menu && GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() < GameLogic::LevelID::Count))
											 break;
										 //if (GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel() == GameLogic::LevelID::Invalid) break;
										 if (!GameLogic::GAMELOGIC_SYSTEM->GetPaused())
										 {
											 Core::MessageUI goToPause("ChangeUI PauseMenu");
											 Core::MessageUI pauseGame("Action Pause");
											 Core::ENGINE->SystemMessage(goToPause);
											 Core::ENGINE->SystemMessage(pauseGame);
											 Audio::AUDIOMANAGER->PauseAll();
											 //Audio::AUDIOMANAGER->PauseMusic();
											 Input::GamePadEvent stopVibe;
											 stopVibe.GetGamePad(0)->StopAllVibration();
											 stopVibe.GetGamePad(1)->StopAllVibration();
											 stopVibe.GetGamePad(2)->StopAllVibration();
											 stopVibe.GetGamePad(3)->StopAllVibration();
										 }
										 else
										 {
											 Core::MessageUI goToGame("ChangeUI HUD");
											 Core::MessageUI unPauseGame("Action unPause");
											 Core::ENGINE->SystemMessage(goToGame);
											 Core::ENGINE->SystemMessage(unPauseGame);
											 //Audio::AUDIOMANAGER->ResumeMusic();
											 Audio::AUDIOMANAGER->ResumeAll();
										 }

										 break;
					   }

						   //Just send the raw keyboard event as a key down message
					   default:
					   {
								  Input::KeyboardEvent::SetKey(wParam, true);

								  Core::ENGINE->SystemMessage(KeyboardEventMessage(WM_KEYDOWN, lParam, wParam));
								  break;
					   }
					   }
					   break;
	}

	case WM_KEYUP: //A key was released
	{
					   Input::KeyboardEvent::SetKey(wParam, false);
					   break;
	}
	case WM_DESTROY:
	{
					   //A destroy message--time to kill the game
					   //Make sure we shut everything down properly by telling Windows
					   //to post a WM_QUIT message (the parameter is the exit code).
					   PostQuitMessage(0);
					   return 0;
	}

		//Handle files being droped into the window
	case WM_DROPFILES:
	{
						 UINT itemCount = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
						 if (itemCount)
						 {
							 char  buffer[512] = { 0 };
							 DragQueryFile((HDROP)wParam, 0, (LPSTR)buffer, 512);
							 DragFinish((HDROP)wParam);



						 }
						 return 0;
	}

	case WM_SYSKEYDOWN:
	{
						  //Eat the WM_SYSKEYDOWN message to prevent freezing the game when
						  //the alt key is pressed
						  switch (wParam)
						  {
						  case VK_LMENU:
						  case VK_RMENU:
							  return 0;//Stop Alt key from stopping the winProc
						  case VK_F4:
						  {
										//Check for Alt F4
										DWORD dwAltKeyMask = (1 << 29);
										if ((lParam & dwAltKeyMask) != 0)
											PostQuitMessage(0);
										return 0;
						  }
						  }
						  return 0;//
	}

	case WM_SIZE:
	{
					WINDOWSSYSTEM->SetScreenWidth(GetSystemMetrics(SM_CXSCREEN));		//Width of the window, including borders
					WINDOWSSYSTEM->SetScreenHeight(GetSystemMetrics(SM_CYSCREEN));
					Core::ENGINE->SystemMessage(ResizeMessage(WINDOWSSYSTEM->GetScreenWidth(), WINDOWSSYSTEM->GetScreenHeight()));

					break;
	}

	}//End switch

	//We didn't completely handle the message, so pass it on for Windows to handle.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void Window::ReceiveMessage(Core::Message& message)
{
	switch (message.MessageId)
	{
	case Core::Message::UIMessage:
	{
									 Core::MessageUI& m = dynamic_cast<Core::MessageUI&>(message);
									 //Split the string into tokens=================================
									 std::string buf; // Have a buffer string
									 std::stringstream ss(m.msg); // Insert the string into a stream
									 std::vector<std::string> args; // Create vector to hold our words
									 while (ss >> buf)
										 args.push_back(buf);

									 if (args.size() > 1 && strcmp("Action", args[0].c_str()) == 0)
									 {
										 if (strcmp("Quit", args[1].c_str()) == 0)
										 {
											 Audio::AUDIOMANAGER->StopAll();
											 Core::ENGINE->Quit();
										 }
									 }

									 else if (args.size() > 1 && strcmp("FullScreen", args[0].c_str()) == 0)
									 {
										 if (strcmp("true", args[1].c_str()) == 0)
										 {
											 SwitchToFullscreen();
										 }

										 else if (strcmp("false", args[1].c_str()) == 0)
										 {
											 SwitchToWindowed();
										 }
									 }
	}
		break;
	}
}


void Window::SetFocus(bool focus)
{
	m_Focus = focus;
}

float Window::GetAspectRatio() const
{
	return static_cast<float>(m_ScreenWidth) / static_cast<float>(m_ScreenHeight);
}

bool Window::IsFullScreen() const
{
	return m_FullScreen;
}

bool Window::GetFocus(void)
{
	return m_Focus;
}

void Window::SwitchToFullscreen(void)
{
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, m_FullscreenWidth, m_FullscreenHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

void Window::SwitchToWindowed(void)
{
	RECT fullWinRect = { 0, 0, m_WindowedWidth, m_WindowedHeight };

	AdjustWindowRect(&fullWinRect,		//The rectangle for the full size of the window
		s_WindowStyle,		//The style of the window, which must match what is passed in to CreateWindow below
		FALSE);			//Does this window have a menu?

	SetWindowLong(hWnd, GWL_STYLE, s_WindowStyle);

	SetWindowPos(hWnd, HWND_NOTOPMOST,
		m_FullscreenWidth / 2 - (fullWinRect.right - fullWinRect.left) / 2,
		m_FullscreenHeight / 2 - (fullWinRect.bottom - fullWinRect.top) / 2,
		fullWinRect.right - fullWinRect.left,
		fullWinRect.bottom - fullWinRect.top,
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

int Window::GetScreenWidth(void)
{
	RECT winRect;
	if (GetClientRect(hWnd, &winRect))
	{
		m_ScreenWidth = winRect.right - winRect.left;
	}
	return m_ScreenWidth;
}

int Window::GetScreenHeight(void)
{
	RECT winRect;
	if (GetClientRect(hWnd, &winRect))
	{
		m_ScreenHeight = winRect.bottom - winRect.top;
	}
	return m_ScreenHeight;
}

int Window::GetWindowWidth(void)
{
	int width = 0;
	RECT winRect;
	if (GetWindowRect(hWnd, &winRect))
	{
		width = winRect.right - winRect.left;
	}

	return width;
}

int Window::GetWindowHeight(void)
{
	int height = 0;
	RECT winRect;
	if (GetWindowRect(hWnd, &winRect))
	{
		height = winRect.bottom - winRect.top;
	}

	return height;
}

void Window::SetScreenWidth(unsigned int width)
{
	m_ScreenWidth = width;
}

void Window::SetScreenHeight(unsigned int height)
{
	m_ScreenHeight = height;
}

void Window::ActivateWindow()
{
	//Show the window (could also be SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED, etc.)
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	//Send a WM_PAINT message to the window
	UpdateWindow(hWnd);
}

void Window::MinimizeWindow()
{
	//Show the window (could also be SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED, etc.)
	ShowWindow(hWnd, SW_MINIMIZE);
	//Send a WM_PAINT message to the window
	UpdateWindow(hWnd);
}

MSG_LUA_DEP(ResizeMessage);
MSG_LUA_DEP(KeyboardEventMessage);
MSG_LUA_DEP(MouseMove);
MSG_LUA_DEP(MouseButtonMessage);
MSG_LUA_DEP(MessageMouseWheel);
MSG_LUA_DEP(MessageHorizontalWheel);