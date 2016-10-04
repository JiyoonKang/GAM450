/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: KeyboardManager.cpp 
 * Author: Jorge Antonio Robles
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "KeyboardManager.h"

#include <cstring> //memcpy
#include <Windows.h> //GetAsyncKeyState

//Init static variables
bool Input::KeyboardEvent::m_CurrentKeyStates[Input::NUM_KEY_CODES] = { 0 };
bool Input::KeyboardEvent::m_PreviousKeyStates[Input::NUM_KEY_CODES] = { 0 };

namespace Input
{
  void KeyboardEvent::Update(void)
  {
    //Stores the previous key state
    memcpy(m_PreviousKeyStates, m_CurrentKeyStates, sizeof(bool) * VirtualKey::NUM_KEY_CODES);

    //		//Disable warning about forcing a short as a bool
    //#pragma warning( push )
    //#pragma warning( disable : 4800 )
    //		for (int i = 0; i < NUM_KEY_CODES; ++i)
    //		{
    //			//Gets the current state of the specified key
    //			m_CurrentKeyStates[i] = GetAsyncKeyState(i);
    //		}
    //#pragma warning (pop)

  }

  void KeyboardEvent::Clear(void)
  {
    memset(m_CurrentKeyStates, false, sizeof(bool) * VirtualKey::NUM_KEY_CODES);
    memset(m_PreviousKeyStates, false, sizeof(bool) * VirtualKey::NUM_KEY_CODES);
  }

  bool KeyboardEvent::IsKeyPressed(VirtualKey key)
  {
    return (!m_PreviousKeyStates[key] && m_CurrentKeyStates[key]);
  }

  bool KeyboardEvent::IsKeyHeld(VirtualKey key)
  {
    return (m_PreviousKeyStates[key] && m_CurrentKeyStates[key]);
  }

  bool KeyboardEvent::IsKeyReleased(VirtualKey key)
  {
    return (m_PreviousKeyStates[key] && !m_CurrentKeyStates[key]);
  }

  bool KeyboardEvent::IsKeyPressed(int key)
  {
    return IsKeyPressed(static_cast<VirtualKey>(key));
  }

  bool KeyboardEvent::IsKeyHeld(int key)
  {
    return  IsKeyHeld(static_cast<VirtualKey>(key));
  }

  bool KeyboardEvent::IsKeyReleased(int key)
  {
    return  IsKeyReleased(static_cast<VirtualKey>(key));
  }

  void KeyboardEvent::SetKey(int key, bool val)
  {
    m_CurrentKeyStates[key] = val;
  }
}

