/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Mouse.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#include "Base\util\math\Float2.h"
namespace Input
{

  enum MOUSESTATES{LEFT, MIDDLE, RIGHT};

  class MouseEvent
  {
  public:

    static void Update(void);
    static void Clear(void);

    static bool IsLeftMouseButtonPressed();
    static bool IsLeftMouseButtonHeld();
    static bool IsLeftMouseButtonReleased();

    static bool IsMiddleMouseButtonPressed();
    static bool IsMiddleMouseButtonHeld();
    static bool IsMiddleMouseButtonReleased();

    static bool IsRightMouseButtonPressed();
    static bool IsRightMouseButtonHeld();
    static bool IsRightMouseButtonReleased();

    static int GetScrollDelta();
    static void SetScollDelta(int delta);
    static void SetMousePosition(int x, int y);

    static float2 GetMousePosition();
    static int GetMouseXPosition();
    static int GetMouseYPosition();

    static float2 GetMouseDelta();

    static void SetLeftMouseButton(bool val);
    static void SetMiddleMouseButton(bool val);
    static void SetRightMouseButton(bool val);

  private:
    static bool m_CurrentKeyStates[3];
    static bool m_PreviousKeyStates[3];
    static int m_ScrollDelta;
    static float2 m_MousePos;
    static float2 m_MouseDelta;
  };
}
