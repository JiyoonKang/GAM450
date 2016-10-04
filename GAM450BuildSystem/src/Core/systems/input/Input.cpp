/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Input.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file   Input.cpp
\project  Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch/precompiled.h"
#include "Input.h"
#include "Base/util/debug/DebugAssist.h"
#include "Core/systems/window/Window.h"

namespace Input
{
  InputHandler* INPUT = NULL;

	InputHandler::InputHandler(void)
	{
    INPUT = this;
	}

	InputHandler::~InputHandler(void)
	{

	}

	void InputHandler::Initialize(void)
	{
		PrintOut("Input Initialize");
	}

	void InputHandler::Shutdown(void)
	{
		PrintOut("Input Release");
	}

  void InputHandler::Update(float dt)
  {
    if (!WINDOWSSYSTEM->GetFocus())
    {
      return;       
    }
    MouseEvent::Update();
    GamePadEvent::Update();
    KeyboardEvent::Update();
  }

  InputHandler* InputHandler::GetInstance()
  {
	  static InputHandler* instance = nullptr;

	  if (!instance)
		  instance = new InputHandler();

	  return instance;
  }

  void InputHandler::ToggleKeyInput()
  {
	  AllowKeyInput = !AllowKeyInput;
  }

  void InputHandler::SetKeyInput(bool input)
  {
	  AllowKeyInput = input;
  }

  bool InputHandler::GetKeyInput()
  {
	  return AllowKeyInput;
  }
	//ResponseTo(Update, InputHandler)
	//{
	//	KeyboardEvent::Update();
	//	//int herro = KeyboardEvent::asdf();
	//	//
	//	//PrintOut("%i", herro);
	//}
}