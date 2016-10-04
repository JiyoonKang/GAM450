--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: meta.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
function SetupMetatable(name, meta)

  print("Registered Lua Type: "..name)

  meta.__name = name;
  meta.__metaName = "__"..name.."_MT";
  meta.__base = nil

  function meta.__index(self, key, ...)
    _R.METAVALUES[tostring(self)] = _R.METAVALUES[tostring(self)] or {}

    if _R.METAVALUES[tostring(self)][key] then
      --print("Found meta value: ".. key)
      return _R.METAVALUES[tostring(self)][key]
    end

    if meta[key] == nil and meta.__base ~= nil and meta.__base[key] ~= nil then
      --print("Using base object for key: "..key)
      return meta.__base[key]
    end

    if key == "super" then
      if meta.__base then
        return meta.__base
      end
    end

    local compID = ComponentDB[key]
    if compID ~= nil then
      local comp = self:GetComponent(compID)
      if comp ~= false then
        return comp
      else
        --print("ERROR : Component does not exist on object!")
        return nil
        
      end
    end

    -- if key == "Transform" then
    --   return self:GetComponent(0)
    -- end

    if meta.__members ~= nil and meta.__members[key] ~= nil then
      --print("Auto getter?")
      return meta["Get"..key](self)
    end

    

    if meta[key] == nil then
      if type(self) == "table" then
        --print("using rawget")
        return rawget(self, key)
      end
    end

    --print(rawget(meta, "__name") .. "::"..key)
    return meta[key]
  end

  function meta.__newindex(self, key, value)
    if type(self) == "table" then
      rawset(self, key, value)
      return
    end

    if meta.__members ~= nil and meta.__members[key] ~= nil then
      meta["Set"..key](self, value)
      return
    end

    _R.METAVALUES[tostring(self)] = _R.METAVALUES[tostring(self)] or {}

    _R.METAVALUES[tostring(self)][key] = value
  end

  function meta:__gc()
    _R.METAVALUES[tostring(self)] = nil
  end

  function meta:__next(k)
    if type(self) == "table" then
      return next(self, k)
    end
    return next(_R.METAVALUES[tostring(self)], k)
  end

  function meta:__pairs()
    return meta.__next, self, nil
  end

  function meta:SetupHooks()
  end

  function meta:Refresh()
  end

  function meta:Init()
    self:SetupHooks()
  end

  function meta.__New()
    return setmetatable({}, meta)
  end

  if (meta.__members ~= nil) then

    for name, member in pairs(meta.__members) do
      meta["Set"..name] = function(self, value)
        engine.SetVariable(self, member, value)
      end

      meta["Get"..name] = function(self)
        return engine.GetVariable(self, member)
      end

    end

  end


end

-- function SetupCMetatables()  

--   for key, meta in pairs(_R) do
--     SetupMetatable(key, meta)
--   end

--   _R.METAVALUES = {}
-- end

function GetMeta(name)
  if _R["__"..name.."_MT"] == nil then
    --print("Metatable not found for: \""..name.."\" creating one!")
    _R["__"..name.."_MT"] = {}
    
    SetupMetatable(name, _R["__"..name.."_MT"])
  end
  
  return _R["__"..name.."_MT"]
end

function SetAs(table, meta)
  return setmetatable(table, GetMeta(meta))
end

function InheritMeta(meta, name)
  local imeta = GetMeta(name)

  meta.__base = imeta;
  --setmetatable(meta, {__index = imeta})
end