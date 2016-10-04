--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: flatpane.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local META = GetMeta("FlatPane")

function META:Init()
end

function META:Think()

end

function META:Paint()
  -- print("Paint FlatPane")
  local pos = self:DrawPos()
  local col = self.color

  surface.SetRotation(0)
  surface.SetColor(col.r, col.g, col.b, col.a)

  if self.dynamicSize then
    surface.DrawRect(ScreenScale(pos.x), ScreenScaleY(pos.y),
     ScreenScale(self.size.x), ScreenScaleY(self.size.y))
  else
    surface.DrawRect(pos.x, pos.y, self.size.x, self.size.y)
  end
  
end

gui.Register( "FlatPane", META, "Panel" )