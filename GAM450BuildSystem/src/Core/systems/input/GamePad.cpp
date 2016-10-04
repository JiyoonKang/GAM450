/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: GamePad.cpp 
 * Author: Joel Shook
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include "Gamepad.h"
#include <cmath>
#include <ctime>
#include "Core/systems/input/Input.h"
#include "Core/engine/frameratecontroller/FrameRateController.h"
#include "GameLogic/manager/GameLogicManager.h"
#include "GameLogic/GameLogicSystem.h"
#include "GameLogic/Level.h"
namespace Input
{
  void GamePad::UpdateControllerState()
  {
    DWORD result;

    lastlStick.x = (state.Gamepad.sThumbLX / 32767.f);
    lastlStick.y = (state.Gamepad.sThumbLY / 32767.f);

    lastrStick.x = (state.Gamepad.sThumbRX / 32767.f);
    lastrStick.y = (state.Gamepad.sThumbRY / 32767.f);

    prevState = state;
    result = XInputGetState(index, &state);

    lStickDelta.x = (state.Gamepad.sThumbLX / 32767.f) - lastlStick.x;
    lStickDelta.y = (state.Gamepad.sThumbLY / 32767.f) - lastlStick.y;

    rStickDelta.x = (state.Gamepad.sThumbRX / 32767.f) - lastrStick.x;
    rStickDelta.y = (state.Gamepad.sThumbRY / 32767.f) - lastrStick.y;

    
    if (prevState.dwPacketNumber == state.dwPacketNumber)
      changed = false;
    else
      changed = true;
    if (result == ERROR_SUCCESS)
    {
      lStickMagnitude = sqrtf(
        (float)(state.Gamepad.sThumbLX * state.Gamepad.sThumbLX)
        + (float)(state.Gamepad.sThumbLY * state.Gamepad.sThumbLY));
      rStickMagnitude = sqrtf(
        (float)(state.Gamepad.sThumbRX * state.Gamepad.sThumbRX)
        + (float)(state.Gamepad.sThumbRY * state.Gamepad.sThumbRY));
      if (lStickMagnitude > 32767)
        lStickMagnitude = 32767;

      lStickMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

      if (lStickMagnitude < 0)
        lStickMagnitude = 0;

      if (rStickMagnitude > 32767)
        rStickMagnitude = 32767;

      rStickMagnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

      if (rStickMagnitude < 0)
        rStickMagnitude = 0;


      connected = true;
    }
    else
    {
      connected = false;
    }
  }

  void GamePad::UpdateControllerState(XINPUT_STATE newState)
  {

    float oldLX = (state.Gamepad.sThumbLX / 32767.f);
    float oldLY = (state.Gamepad.sThumbLY / 32767.f);

    float oldRX = (state.Gamepad.sThumbRX / 32767.f);
    float oldRY = (state.Gamepad.sThumbRY / 32767.f);

    prevState = state;
    state = newState;
    changed = true;

    lStickDelta.x = (state.Gamepad.sThumbLX / 32767.f) - oldLX;
    lStickDelta.y = (state.Gamepad.sThumbLY / 32767.f) - oldLY;
    rStickDelta.x = (state.Gamepad.sThumbRX / 32767.f) - oldRX;
    rStickDelta.y = (state.Gamepad.sThumbRY / 32767.f) - oldRY;

    lStickMagnitude = sqrtf(
      (float)(state.Gamepad.sThumbLX * state.Gamepad.sThumbLX)
      + (float)(state.Gamepad.sThumbLY * state.Gamepad.sThumbLY));
    rStickMagnitude = sqrtf(
      (float)(state.Gamepad.sThumbRX * state.Gamepad.sThumbRX)
      + (float)(state.Gamepad.sThumbRY * state.Gamepad.sThumbRY));
    if (lStickMagnitude > 32767)
      lStickMagnitude = 32767;

    lStickMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

    if (lStickMagnitude < 0)
      lStickMagnitude = 0;

    if (rStickMagnitude > 32767)
      rStickMagnitude = 32767;

    rStickMagnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

    if (rStickMagnitude < 0)
      rStickMagnitude = 0;


    connected = true;
  }

  float2 GamePad::LeftStick()
  {
    float2 result;
    if (lStickMagnitude <= 0 || !connected)
    {
      return result;
    }
    result.x = (state.Gamepad.sThumbLX / 32767.f);
    result.y = (state.Gamepad.sThumbLY / 32767.f);
    return result;
  }

  float2 GamePad::RightStick()
  {
    float2 result;
    if (rStickMagnitude <= 0 || !connected)
    {
      return result;
    }
    result.x = (state.Gamepad.sThumbRX / 32767.f);
    result.y = (state.Gamepad.sThumbRY / 32767.f);
    return result;
  }

  float GamePad::LeftTrigger()
  {
    if (!connected) return 0;
    float temp = state.Gamepad.bLeftTrigger - (float)XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    if (temp < 0) return 0;
    return (temp) / 255.0f;
  }

  float GamePad::RightTrigger()
  {
    if (!connected) return 0;
    float temp = state.Gamepad.bRightTrigger - (float)XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    if (temp < 0) return 0;
    return (temp) / 255.0f;
  }

  bool GamePad::ButtonPressed(WORD buttonID)
  {
    if (!connected) return false;
    if (state.Gamepad.wButtons & buttonID)
      return true;

    return false;
  }

  bool GamePad::ButtonJustPressed(WORD buttonID)
  {
    if (!connected) return false;
    if ((state.Gamepad.wButtons & buttonID))
    {
      if (!(prevState.Gamepad.wButtons & buttonID))
        return true;
    }
    return false;
  }

  bool GamePad::ButtonJustReleased(WORD buttonID)
  {
    if (!connected) return false;
    if (!(state.Gamepad.wButtons & buttonID))
    {
      if ((prevState.Gamepad.wButtons & buttonID))
        return true;
    }
    return false;
  }

  void GamePad::SetVibration(float leftMotor, float rightMotor)
  {
    if (!connected) return;

    vibration.wLeftMotorSpeed = (unsigned int)(leftMotor * 65535.0f);
    vibration.wRightMotorSpeed = (unsigned int)(rightMotor * 65535.0f);
    XInputSetState(index, &vibration);
  }

  void GamePad::SetVibration(float2 motors)
  {
    if (!connected) return;

    vibration.wLeftMotorSpeed = (unsigned int)(motors.x * 65535.0f);
    vibration.wRightMotorSpeed = (unsigned int)(motors.y * 65535.0f);
    XInputSetState(index, &vibration);
  }

  float2 GamePad::GetVibration()
  {
    if (!connected) float2();
    float2 result;
    result.x = vibration.wLeftMotorSpeed / 65535.0f;
    result.y = vibration.wRightMotorSpeed / 65535.0f;
    return result;
  }

  void GamePad::StopAllVibration()
  {
    if (!connected) return;
    vibration.wLeftMotorSpeed = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(index, &vibration);
  }

  void GamePad::StopLeftMotorVibration()
  {
    if (!connected) return;
    vibration.wLeftMotorSpeed = 0;
    XInputSetState(index, &vibration);
  }

  void GamePad::StopRightMotorVibration()
  {
    if (!connected) return;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(index, &vibration);
  }

  float2 GamePad::GetLeftStickDelta()
  {
    return lStickDelta;
  }

  float2 GamePad::GetRightStickDelta()
  {
    return rStickDelta;
  }

  bool GamePadEvent::playing = false;
  bool GamePadEvent::recording = false;
  InputFrame GamePadEvent::nextInput;
  std::ifstream GamePadEvent::infile;
  std::ofstream GamePadEvent::outfile;
  float GamePadEvent::timeSinceLastChange = 0.0f;

  void GamePadEvent::Update(void)
  {
    if (Input::KeyboardEvent::IsKeyReleased(KEY_6))
    {
      ToggleRecording();
    }
    if (Input::KeyboardEvent::IsKeyReleased(KEY_7))
    {
      TogglePlaying();
    }
    timeSinceLastChange += Core::FRAMERATECONTROLLER->GetDT();
    for (unsigned i = 0; i < 4; ++i)
    {
      gamepads[i].index = i;
      if (!playing)
      {
        gamepads[i].UpdateControllerState();
      }
      if (recording)
      {
        if (gamepads[i].changed)
        {
          outfile.write((const char *)(&timeSinceLastChange), sizeof(float));
          outfile.write((const char *)(&i), sizeof(unsigned));
          outfile.write((const char *)(&gamepads[i].state), sizeof(XINPUT_STATE));
          timeSinceLastChange = 0.0f;
        }

      }
    }
    if (playing)
    {
      while (infile.is_open() && nextInput.timeSinceLast - timeSinceLastChange <= 0 && infile.eof() == false)
      {
        gamepads[nextInput.ID].index = nextInput.ID;
        gamepads[nextInput.ID].UpdateControllerState(nextInput.state);
        infile.read((char*)(&nextInput.timeSinceLast), sizeof(float));
        infile.read((char*)(&nextInput.ID), sizeof(unsigned));
        infile.read((char*)(&nextInput.state), sizeof(XINPUT_STATE));
        timeSinceLastChange = 0.0f;
      }
      if (infile.eof() || !infile.is_open())
      {
        TogglePlaying();
        TogglePlaying();
      }
    }
  }

  GamePad * GamePadEvent::GetGamePad(unsigned num)
  {
    if (num >= 4)
      return nullptr;
    return &gamepads[num];
  }

  void GamePadEvent::ToggleRecording()
  {
    recording = !recording;
    playing = false;
    if (recording)
    {
      if (infile.is_open()) infile.close();
      if (outfile.is_open()) outfile.close();

      time_t now = time(0);
      tm *gmtm = gmtime(&now);
      std::string filename = "content//Input//inputfile_"; //..\\..\\..\\bin\\content\\input
      filename += std::to_string(gmtm->tm_year + 1900);
      filename += "_";
      filename += std::to_string(gmtm->tm_mon + 1);
      filename += "_";
      filename += std::to_string(gmtm->tm_mday);
      filename += "_";
      filename += std::to_string(gmtm->tm_hour - 8);
      filename += "_";
      filename += std::to_string(gmtm->tm_min);
      filename += "_";
      filename += std::to_string(gmtm->tm_sec);
      filename += ".input";
      outfile.open(filename, std::ios::binary);
      timeSinceLastChange = 0.0f;
      GameLogic::LevelID temp = GameLogic::GAMELOGIC_SYSTEM->GetCurrentLevel();
      outfile.write((char *)(&temp), sizeof(GameLogic::LevelID));
    }
    else
    {
      outfile.close();
    }
  }

  void GamePadEvent::TogglePlaying()
  {
    playing = !playing;
    recording = false;

    if (outfile.is_open()) outfile.close();
    if (playing)
    {
      if (infile.is_open()) infile.close();

      nextInput.timeSinceLast = 0;
      nextInput.ID = 0;
      SecureZeroMemory(&nextInput.state, sizeof(nextInput.state));

      WIN32_FIND_DATA fd[256];
      int count = 0;
      std::string name = "content\\Input\\";
      FILE_INFO_BY_HANDLE_CLASS fileInfo = FILE_INFO_BY_HANDLE_CLASS::FileNameInfo;
      HANDLE h = FindFirstFile((name + "*.input").c_str(), &fd[0]);
      
      
      int i = 1;
      while (FindNextFile(h, &fd[i++]));
      FindClose(h);

      unsigned index = rand() % i;

      infile.open(name + fd[index].cFileName, std::ios_base::binary);
      GameLogic::LevelID temp;
      infile.read((char*)&temp, sizeof(GameLogic::LevelID));
      GameLogic::GAMELOGIC_SYSTEM->SwitchLevel(temp);
      timeSinceLastChange = 0.0f;

    }
    else
    {
      infile.close();
    }

  }

  Input::GamePad GamePadEvent::gamepads[4];

}