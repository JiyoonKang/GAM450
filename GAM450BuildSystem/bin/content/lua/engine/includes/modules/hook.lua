--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: hook.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
local pairs = pairs
local type = type
local IsValid = IsValid
local setmetatable = setmetatable


local print = print
local PrintTable = PrintTable

module("hook")

local Hooks = {}


function GetTable() return Hooks end

function Add(event, name, func, checkfunc)
	if type(func) ~= "function" and type(func) ~= "string" then return end
	if type(event) ~= "string" then return end
	
	if Hooks[event] == nil then
		Hooks[event] = setmetatable({}, {__mode = 'k' })
	end

	Hooks[event][name] = {func, checkfunc}
end

function Remove(event, name)
	if type(event) ~= "string" then return end
	
	Hooks[event][name] = nil
end

function Run(event, ...)
	return Call(name, ...)
end

function Call(event, ...)

	if Hooks[event] ~= nil then

		--PrintTable(HookTable)

		for k,v in pairs(Hooks[event]) do

			if type(k) == "string" then

				if v[2] ~= nil then
					if v[2](...) then
						v[1](...)
					end
				else
					v[1](...)
				end

			else

				if IsValid(k) then -- Assume it is a component or object

					-- If the object is valid, pass the first argument as self
					if v[2] ~= nil then
						if v[2](k, ...) then
							v[1](k, ...)
						end
					else
						v[1](k, ...)
					end

				else
					-- If the object is not valid, remove the hook
					--print("Invalid! Removing!")
					Hooks[event][k] = nil
				end

			end

		end

	end
end

