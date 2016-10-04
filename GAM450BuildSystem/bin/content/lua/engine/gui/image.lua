--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: image.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local META = GetMeta("Image")

function META:Init()
  self:SetTexture("Default.png")
  self:SetColor(Color(255, 255, 255))
end

function META:SetTexture(tex)
  self.texture = tex
  self._textureID = surface.GetTextureID(self.texture)
end

function META:Paint()
  --print("Paint Image")
  local pos = self:DrawPos()

  surface.SetColor(self.color.r, self.color.g, self.color.b, self.color.a)
  surface.SetTexture(self._textureID)

  if self.dynamicSize then
    surface.DrawTexturedRectRotated(
      ScreenScale(pos.x), ScreenScaleY(pos.y),
      ScreenScale(self.size.x), ScreenScaleY(self.size.y), 0)
  else
    surface.DrawTexturedRectRotated(pos.x, pos.y, self.size.x, self.size.y, 0)
  end

  

  
end

gui.Register( "Image", META, "Frame" )