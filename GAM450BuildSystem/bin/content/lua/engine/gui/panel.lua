--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: panel.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local META = GetMeta("Panel")

function META:Init()
  self.size = Vec2(32, 32)
  self.pos = Vec3(0, 0, 0)
  self.bgColor = Color(100, 100, 100)
  self.color = Color(180, 180, 180)
  self.visible = true
  self.valid = true
  self.camMode = 2
  self.priority = 0
  self.dynamicSize = false
  self.children = {}
end

function META:SetPriority(p)
  self.priority = p
end

function META:OnDestroy()

end

function META:SetVisible(v)
  self.visible = v
end

function META:Destroy()
  --print("Destroying "..self.className)
  self:OnDestroy()

  self.valid = false

  for k,v in pairs(self.children) do
    v:Destroy()
  end
end

function META:SetColor(color)
  self.color = color
end

function META:GetColor()
  return self.color
end

function META:SetBGColor(color)
  self.bgColor = color
end

function META:SetSize(w, h)
  if h == nil then
    self.size = w
  else
    self.size = Vec2(w, h)
  end
end

function META:SetDSize(w, h)
  self:SetSize(w, h)
  self.dynamicSize = true
end

function META:SetDynamicSizing(status)
  self.dynamicSize = status
end

function META:GetSize()
  return self.size
end

function META:SetPos(x, y, z)
  --print("SETPOS")
  if y == nil then
    if x.z then
      self.pos = x
    else
      self.pos = Vec3(x.x, x.y, 0)
    end
  else
    if z == nil then z = 0 end
    self.pos = Vec3(x, y, z)
  end
end

function META:GetPos()
  --print("Get Pos")
  return self:DrawPos()
end

function META:SetCamMode(mode)
  self.camMode = mode
end

function META:GetCamMode()
  return self.camMode
end

function META:DrawPos()
  --print("Draw Pos")
  if self.parent then
     --print("[Parnet]Draw Pos")
    return self.pos + self.parent:DrawPos()
  else
    -- print("[True]Draw Pos")
    return self.pos
  end
end

-- Do not override this function
function META:_BaseDelete()
  --print("Deleting "..self.className)
  self:Delete()

  for k,v in pairs(self.children) do
    v:_BaseDelete()
  end
end

function META:Delete()

end

function META:Prepare()

end

function META:Hovered()

end

function META:_BaseThink()
  self:Think()

  for k,v in pairs(self.children) do
    v:_BaseThink()
  end
end

function META:Think()

end

function META:GetPriority()
  if not self.parent then
    return self.priority + self.pos.z
  else
    return self.parent:GetPriority() - 0.01
  end
end

function META:_PreDraw()
  if not self.parent then
    surface.ForceZ(true, self.priority + self.pos.z)
    --surface.SetCamState(self.camMode)
  else
    surface.ForceZ(true, self:GetPriority() + self.pos.z)
  end

  -- if self.pos.z ~= 0 then
  --   surface.ForceZ(true, self:GetPriority() + self.pos.z)
  -- end
  
end

function META:_PostDraw()

  if not self.parent then
    surface.ForceZ(false, 0)
    --surface.SetCamState(0)
  end
end

function META:_BasePaint()
  self:_PreDraw()

  self:Paint()

  -- print("Child Paint")
  -- print(tostring(self))
  for k,v in pairs(self.children) do
    -- print(k.."="..tostring(v))
    if v.visible then
      v:_BasePaint()
    end
    -- print("endDraw")
  end
  -- print("End Child Paint")


  self:_PostDraw()
end

function META:Paint()
  -- print("Paint Panel")
  -- Do the painting yeah
end
