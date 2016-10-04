/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Gamepad.h 
 * Author: Joel Shook
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <direct.h>
#include "Base\util\math\Float2.h"
#include <fstream>

namespace Input
{
  struct GamePad
  {
    GamePad() : connected(false){}
    //Don't use. Updates the controller each frame.
    void UpdateControllerState();
    void UpdateControllerState(XINPUT_STATE newState);

    //Returns left stick, x and y from -1 to 1 with 0 being the deadzone
    float2 LeftStick();
    //Returns right stick, x and y from -1 to 1 with 0 being the deadzone
    float2 RightStick();
    //Returns float from 0 to 1 with 0 being fully released, 1 being fully depressed 
    float LeftTrigger();
    //Returns float from 0 to 1 with 0 being fully released, 1 being fully depressed 
    float RightTrigger();
    //Returns if the button is currently down. All ID's start with XINPUT_GAMEPAD_ 
    bool ButtonPressed(WORD buttonID);
    //Returns if the button was just pressed. All ID's start with XINPUT_GAMEPAD_ 
    bool ButtonJustPressed(WORD buttonID);
    //Returns if the button was just released. All ID's start with XINPUT_GAMEPAD_ 
    bool ButtonJustReleased(WORD buttonID);
    //Set left and right motor vibration, left and right should be between 0 and 1
    void SetVibration(float leftMotor, float rightMotor);
    //Set left and right motor vibration. left is x, right is y, should be between 0 and 1
    void SetVibration(float2 motors);
    //Gets left and right motor vibration. left is x, right is y, should be between 0 and 1
    float2 GetVibration();
    //Stops all vibration in both motors
    void StopAllVibration();
    //Stops all vibration in left motor
    void StopLeftMotorVibration();
    //Stops all vibration in right motor
    void StopRightMotorVibration();

    float2 GetLeftStickDelta();
    float2 GetRightStickDelta();

    float2 GetLastLeftStick();
    float2 GetLastRightStick();

    float2 lStickDelta;
    float2 rStickDelta;

    float2 lastlStick;
    float2 lastrStick;

    XINPUT_STATE prevState;
    XINPUT_STATE state;
    float lStickMagnitude;
    float rStickMagnitude;
    bool connected;
    char index;
    bool changed;
    XINPUT_VIBRATION vibration;
  };

  struct InputFrame
  {
    XINPUT_STATE state;
    unsigned ID;
    float timeSinceLast;
  };

  class GamePadEvent
  {
  public:

    static void Update(void);

    static GamePad * GetGamePad(unsigned num);

    
    
  private:
    static GamePad gamepads[4];
    static bool recording;
    static bool playing;
    static std::ifstream infile;
    static std::ofstream outfile;
    static float timeSinceLastChange;

    static void ToggleRecording();
    static void TogglePlaying();
    static InputFrame nextInput;

  };
}

