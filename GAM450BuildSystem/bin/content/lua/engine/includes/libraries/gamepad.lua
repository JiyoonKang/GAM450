--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: gamepad.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
GAMEPAD_A = 0
GAMEPAD_B = 1
GAMEPAD_X = 2
GAMEPAD_Y = 3
GAMEPAD_DPAD_UP = 4
GAMEPAD_DPAD_DOWN = 5
GAMEPAD_DPAD_LEFT = 6
GAMEPAD_DPAD_RIGHT = 7
GAMEPAD_LEFT_SHOULDER = 8
GAMEPAD_RIGHT_SHOULDER = 9
GAMEPAD_LEFT_THUMB = 10
GAMEPAD_RIGHT_THUMB = 11
GAMEPAD_START = 12
GAMEPAD_BACK = 13

if not gamepad then gamepad = {} end

function gamepad.ButtonDown(pads, btn)
  if not pads then pads = {0, 1, 2, 3} end
  for k,v in pairs(pads) do
    if GamePad(v):ButtonDown(btn) then
      return true
    end
  end
end

function gamepad.ButtonPressed(pads, btn)
  if not pads then pads = {0, 1, 2, 3} end
  for k,v in pairs(pads) do
    if GamePad(v):ButtonPressed(btn) then
      return true
    end
  end
end