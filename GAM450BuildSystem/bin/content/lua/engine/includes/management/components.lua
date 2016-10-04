--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: components.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
function RegisterComponent(meta)
  print("Registered LuaComponent: "..meta.__name)
  if LuaComponents[meta.__name] == nil then
    LuaComponents[meta.__name] = meta.__name
  end
end

function QueryComponents()
  print("Querying Lua Components")

  local tbl = {}

  for k,v in pairs(LuaComponents) do
    tbl[#tbl + 1] = v
  end

  PrintTable(LuaComponents)
  engine.SendComponentList(#tbl, tbl)
end

-- Attaches a component to an object
-- If the object does not exist in the lua envioronment then it is created
-- An object consists of a data table of components
-- space = name of game space
-- owner = handle of the owning object
-- cid = handle of the component
-- cname = name of the component
function AttachComponentToObject(space, owner, cid, cname)

  -- Try to find the object
  local object = GameSpaces[space][owner]

  -- Create the object if it doesn't exist
  if not object then
    GameSpaces[space][owner] = {}
    object = GameSpaces[space][owner]
    object._self = owner
  end

  -- Create a table for the object
  object[cid] = {}

  local component = object[cid]
  local meta = GetMeta(cname)

  --PrintTable(meta)

  setmetatable(component, meta)

  component._owner = owner
  component._space = space
  component._cid = cid
  component._type = cname
  component._spacePtr = GameSpaces[space]._pointer

  print("[GameSpace: "..space.."] Attached LuaComponent: "..cname.." ["..cid.."] to C++ GameObject: "..owner)

  component:Init()
end


function RemoveComponentFromGameObject(space, handle, chandle)
  print("[GameSpace: "..space.."] Removed LuaComponent ("..chandle..") from C++ GameObject: "..handle)
  local comp = GameSpaces[space][handle][chandle]

  if comp ~= nil then
    if comp.Remove then
      comp:Remove()
    end
  end

  GameSpaces[space][handle][chandle] = nil
end

function DeserializeComponent(space, owner, cid, command)
  local object = GameSpaces[space][owner]

  if not object then return end

  local self = object[cid]
  --print(command)
  load(command)()(self)
end

function SerializeComponent(space, owner, cid, CLComp)
  --print("Serialize Comoonent: "..space.." "..owner.." "..cid)
  local object = GameSpaces[space][owner]

  if not object then return end

  local component = object[cid]

  local string = "return function(self) "

  for k,v in pairs(component) do

    if type(v) ~= "table" and type(v) ~= "userdata" then
      if string.sub(k, 0, 1) ~= "_" then
        if type(v) == "string" then
          string = string .. "self." .. tostring(k) .. " = '" ..tostring(v) .. "' "
        else
          string = string .. "self." .. tostring(k) .. " = " ..tostring(v) .. " "
        end
        
      end

    end

  end

  string = string .. "end"

  --print(string)

  CLComp:SendLoadCommand(string)
end