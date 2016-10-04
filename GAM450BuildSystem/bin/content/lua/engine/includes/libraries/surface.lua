--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: surface.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
if not surface then surface = {} end

function surface.SetColor(r, g, b, a)
  local alpha = a
  if alpha == nil then alpha = 255 end

  surface.SetColorEx(r / 255, g / 255, b / 255, alpha / 255)
end