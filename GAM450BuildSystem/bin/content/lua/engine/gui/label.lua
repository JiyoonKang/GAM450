--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: label.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local META = GetMeta("Label")

function META:Init()
  self.text = ""
  self.fontSize = 1

  self:SetFont("Arial")

  self.outlineWidth = 0
  self.outlineColor = Color(0, 0, 0, 255)

  self.alignX = TEXT_ALIGN_LEFT
  self.alignY = TEXT_ALIGN_TOP
end

function META:SetOutlineWidth(w)
  self.outlineWidth = w
end

function META:SetOutlineColor(col)
  self.outlineColor = col
end

function META:SetFontColor(col)
  self.color = col
end

function META:SetSize(x, y)
  self.fontSize = x
end

function META:SetDSize(x, y)
  self:SetSize(x, y)
  self.dynamicSize = true
end

function META:SetXAlignment(x)
  self.alignX = x
end
META.SetXAlign = META.SetXAlignment

function META:SetYAlignment(y)
  self.alignY = y
end
META.SetYAlign = META.SetYAlignment

function META:SetText(text)
  self.text = text
end

function META:GetText()
  return self.text
end

function META:SetFont(font)
  self.font = font
  self.fontID = surface.GetFontID(font)
end

function META:GetFont()
  return self.font
end

function META:SetFontSize(fsize)
  self.fontSize = fsize
end

function META:GetFontSize()
  return self.fontSize
end

function META:Think()

end

function META:Pressed()
  if self.OnPressed then
    self:OnPressed()
  end
end

function META:SetOnPressed(func)
  self.OnPressed = func
end

function META:DrawSize()
  return surface.MeasureString(self.text, self.fontSize, self.fontID)
end

function META:Paint()
  --print("Paint Label")
  local pos = self:DrawPos()

  surface.SetRotation(0)

  if self.dynamicSize then

    if self.outlineWidth > 0 then
      draw.SimpleTextOutlined(self.text, self.fontID, ScreenScale(pos.x),
       ScreenScaleY(pos.y), ScreenScale(self.fontSize), self.color,
        self.alignX, self.alignY,
       self.outlineWidth, self.outlineColor)
    else
      draw.SimpleText(self.text, self.fontID, ScreenScale(pos.x),
       ScreenScaleY(pos.y),
       ScreenScale(self.fontSize), self.color, self.alignX, self.alignY)
    end
  else
    if self.outlineWidth > 0 then
      draw.SimpleTextOutlined(self.text, self.fontID, pos.x, pos.y,
       self.fontSize, self.color, self.alignX, self.alignY,
       self.outlineWidth, self.outlineColor)
    else
      draw.SimpleText(self.text, self.fontID, pos.x, pos.y,
       self.fontSize, self.color, self.alignX, self.alignY)
    end

  end

  

end

gui.Register( "Label", META, "Panel" )