--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: luamanager.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
function LoadLuaLevel(path)
  filesystem.LoadSingleLuaFile(path, true)
end

function ReloadObjects()
  for space_name, space in pairs(LuaSpaces) do
    for object_id, object in pairs(space) do
      if type(object) == "table" then
        object:SetupHooks()
      end
    end
  end
end

function CreateLuaSpace(name)
  if LuaSpaces[name] ~= nil then
    DestroyLuaSpace(name)
  end

  print("Created LuaSpace: "..name)

  LuaSpaces[name] = setmetatable({}, META)
  LuaSpaces[name]._name = name

  return LuaSpaces[name]
end

function DestroyLuaSpace(name)
  print("Destroyed LuaSpace: "..name)
  LuaSpaces[name] = nil
  collectgarbage()
end

function CreateLuaObject(space, type, name)
  local META = GetMeta(type)

  if (name == nil) then
    name = #LuaSpaces[space] + 1
  end

  LuaSpaces[space][name] = META.__New()

  local object = LuaSpaces[space][name]

  object:Init()

  return object
end
