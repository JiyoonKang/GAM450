--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: frame.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local META = GetMeta("Frame")

function META:Init()
  self.border = 4
end

function META:SetBorder(b)
  self.border = b
end

function META:GetBorder()
  return self.border
end

function META:Think()

end

function META:Paint()
  local border = self.border

  local pos = self:DrawPos()

  draw.RoundedBox(4, pos.x, pos.y,
    self.size.x, self.size.y, self.bgColor)

  draw.RoundedBox(4, border + pos.x, border + pos.y,
    self.size.x - border * 2, self.size.y - border * 2, self.color)
end

gui.Register( "Frame", META, "Panel" )