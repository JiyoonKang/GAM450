--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: gamespace.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
function ReloadComponents()
  for space_name, space in pairs(GameSpaces) do
    for object_handle, object in pairs(space) do
      if type(object) == "table" then
        for component_handle, component in pairs(object) do
          if type(component) == "table" then
            if component.SetupHooks then
              component:SetupHooks()
            end
            if component.Refresh then
              component:Refresh()
            end
          end
        end
      end
    end
  end
end

if not gamespace then
  gamespace = {}
end

--------------------- C++ Functions ---------------------------------

function gamespace.AddGameSpace(name, space)
  Log(DEBUG, "Added C++ GameSpace: "..name)
  GameSpaces[name] = {}
  GameSpaces[name]._pointer = space
  GameSpaces[name]._paused = false
  GameSpaces[name]._name = name
end

function gamespace.RemoveGameSpace(name)
  Log(DEBUG, "Removed C++ GameSpace: "..name)
  GameSpaces[name] = nil

  collectgarbage()
end

function gamespace.RemoveGameObject(space, handle)
  Log(INFO, "[GameSpace: "..space.."] Removed C++ GameObject: "..handle)

  if GameSpaces[space] == nil then return end
  if GameSpaces[space][handle] == nil then return end

  local object = GameSpaces[space][handle]

  for k, comp in pairs(object) do
    if type(comp) == "table" then
      if comp.Remove then
        comp:Remove()
      end
    end
  end

  GameSpaces[space][handle] = nil
end

function gamespace.PauseGameSpace(name, isPaused)
  if GameSpaces[name] == nil then
    GameSpaces[name] = {}
    Log(WARNING, "Attempted to paused C++ gamespace when it didn't exist in lua")
  end
  Log(INFO, "C++ GameSpace ["..name.."] paused="..tostring(isPaused))
  GameSpaces[name]._paused = isPaused
end

function gamespace.Get(space)
  return GameSpaces[space]
end



