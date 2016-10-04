--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: base.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local META = GetMeta("LuaComponent")

function META:CanUpdate(deltatime)
  if self._space ~= nil then
    if GameSpaces[self._space] ~= nil then
      if GameSpaces[self._space]._paused then
        return false
      else
        return true
      end
    end
  end

  return true
end

function META:Owner()
  return GameSpaces[self._space]._pointer:GetObject(self._owner)
end

function META:Remove()

end

function META:Init()
  self:SetupHooks()
end