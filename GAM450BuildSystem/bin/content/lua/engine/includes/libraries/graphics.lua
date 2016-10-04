--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: graphics.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

function WindowResize(width, height)
  SCREEN_WIDTH = width
  SCREEN_HEIGHT = height
  hook.Call("ScreenResize", width, height)
end

function ScrW()
  return SCREEN_WIDTH
end

function ScrH()
  return SCREEN_HEIGHT
end

function CheckDimensions()
  SCREEN_WIDTH = GetScrW()
  SCREEN_HEIGHT = GetScrH()
end

VWW = 640.0
VWH = 640.0

function ScreenScale(size)
  return size * (ScrW() / VWW)
end

function ScreenScaleY(size)
  return size * (ScrH() / VWH)
end