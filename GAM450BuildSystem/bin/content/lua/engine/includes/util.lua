--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: util.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local function Pad()
  for i=1,padding do
    io.write("  ")
  end
end

ERROR = 0
WARNING = 1
DEBUG = 3
INFO = 4
VERBOSE = 5

local levelNames = {"ERROR", "WARNING", "DEBUG", "INFO", "VERBOSE"}

LogLevel = DEBUG
function Log(level, message)
  if level <= LogLevel then
    --print(engine.DateTime().." ["..levelNames[level].."]    "..message.." (Lua)")
print(" ["..levelNames[level].."]    "..message.." (Lua)")
  end
end



function PrintTable(tbl)
  printed = {}
  PrintTableStep(tbl)
end

function PrintTableStep(tbl)
  if not padding then padding = 0 end

  Pad()
  print("{")

  padding = padding + 1

  for k,v in pairs(tbl) do

    if (type(v) == "table") then
      if printed[tostring(v)] == nil then
        printed[tostring(v)] = true
        Pad()
        io.write(tostring(k)..":".."\n")
        PrintTableStep(v)
      else
        Pad()
        io.write(tostring(k)..": "..tostring(v).."\n")
      end
    else
      Pad()
      io.write(tostring(k)..": "..tostring(v).. "\n")
    end

  end

  padding = padding - 1

  Pad()
  print("}")
end

function loopcheck(table)
  for k,v in pairs(table) do
  end
end

function IsValid(object)
  if object == nil then return false end
  if type(object) == "table" then
    if not pcall(loopcheck, object) then
      return false
    end
  end
  return type(object) == "userdata" or type(object) == "table"
end

if not module then
    function module(modname,...)
        local function findtable(tbl,fname)
            for key in string.gmatch(fname,"([%w_]+)") do
                if key and key~="" then
                    local val = rawget(tbl,key)
                    if not val then
                        local field = {}
                        tbl[key]=field
                        tbl = field
                    elseif type(val)~="table" then
                        return nil
                    else
                        tbl = val
                    end
                end
            end
            return tbl
        end

        assert(type(modname)=="string")
        local value,modul = package.loaded[modname]
        if type(value)~="table" then
            modul = findtable(_G,modname)
            assert(modul,"name conflict for module '"..modname.."'" )
            package.loaded[modname] = modul
            SheepModules[modname] = modul
        else
            modul = value
        end

        local name = modul._NAME
        if not name then
            modul._M = modul
            modul._NAME = modname
            modul._PACKAGE = string.match(modname,"([%w%._]*)%.[%w_]*$")
        end
        local func = debug.getinfo(2,"f").func
        debug.setupvalue(func,1,modul)
        for _,f in ipairs{...} do f(modul) end
    end

    function package.seeall(modul)
        setmetatable(modul,{__index=_G})
    end
end

function RunString(command, ...)
  load("return function(...) " .. command .. " end")()(...)
end

if surface == nil then
  surface = {}
end

