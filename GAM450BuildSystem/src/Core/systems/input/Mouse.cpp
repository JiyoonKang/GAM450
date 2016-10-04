/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Mouse.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "Mouse.h"

namespace Input
{


  void MouseEvent::Update(void)
  {
    memcpy(m_PreviousKeyStates, m_CurrentKeyStates, sizeof(bool) * 3);
    m_MouseDelta.xy = float2(0.f, 0.f);
    m_ScrollDelta = 0;
    if (MouseEvent::IsLeftMouseButtonHeld())
    {
      int b = 10;
    }
  }

  void MouseEvent::Clear(void)
  {
    memset(m_CurrentKeyStates, false, sizeof(bool) * 3);
    memset(m_PreviousKeyStates, false, sizeof(bool) * 3);
  }

  bool MouseEvent::IsLeftMouseButtonPressed()
  {
    return (!m_PreviousKeyStates[LEFT] && m_CurrentKeyStates[LEFT]);
  }

  bool MouseEvent::IsLeftMouseButtonHeld()
  {
    return (m_PreviousKeyStates[LEFT] && m_CurrentKeyStates[LEFT]);
  }

  bool MouseEvent::IsLeftMouseButtonReleased()
  {
    return (m_PreviousKeyStates[LEFT] && !m_CurrentKeyStates[LEFT]);
  }

  bool MouseEvent::IsMiddleMouseButtonPressed()
  {
    return (!m_PreviousKeyStates[MIDDLE] && m_CurrentKeyStates[MIDDLE]);
  }

  bool MouseEvent::IsMiddleMouseButtonHeld()
  {
    return (m_PreviousKeyStates[MIDDLE] && m_CurrentKeyStates[MIDDLE]);
  }

  bool MouseEvent::IsMiddleMouseButtonReleased()
  {
    return (m_PreviousKeyStates[MIDDLE] && !m_CurrentKeyStates[MIDDLE]);
  }

  bool MouseEvent::IsRightMouseButtonPressed()
  {
    return (!m_PreviousKeyStates[RIGHT] && m_CurrentKeyStates[RIGHT]);
  }

  bool MouseEvent::IsRightMouseButtonHeld()
  {
    return (m_PreviousKeyStates[RIGHT] && m_CurrentKeyStates[RIGHT]);
  }

  bool MouseEvent::IsRightMouseButtonReleased()
  {
    return (m_PreviousKeyStates[RIGHT] && !m_CurrentKeyStates[RIGHT]);
  }

  int MouseEvent::GetScrollDelta()
  {
    return m_ScrollDelta;
  }

  void MouseEvent::SetScollDelta(int delta)
  {
    m_ScrollDelta = delta;
  }

  void MouseEvent::SetLeftMouseButton(bool val)
  {
    m_CurrentKeyStates[LEFT] = val;
  }

  void MouseEvent::SetMiddleMouseButton(bool val)
  {
    m_CurrentKeyStates[MIDDLE] = val;
  }

  void MouseEvent::SetRightMouseButton(bool val)
  {
    m_CurrentKeyStates[RIGHT] = val;
  }

  void MouseEvent::SetMousePosition(int x, int y)
  {
    m_MouseDelta = float2(float(x), float(y)) - m_MousePos;
    m_MousePos = float2(float(x), float(y));
  }

  float2 MouseEvent::GetMousePosition()
  {
    return m_MousePos;
  }

  int MouseEvent::GetMouseXPosition()
  {
    return static_cast<int>(m_MousePos.x);
  }

  int MouseEvent::GetMouseYPosition()
  {
    return static_cast<int>(m_MousePos.y);
  }

  float2 MouseEvent::GetMouseDelta()
  {
    return m_MouseDelta;
  }

  float2 MouseEvent::m_MouseDelta;

  float2 MouseEvent::m_MousePos;

  int MouseEvent::m_ScrollDelta = 0;

  bool MouseEvent::m_PreviousKeyStates[3];

  bool MouseEvent::m_CurrentKeyStates[3];

}