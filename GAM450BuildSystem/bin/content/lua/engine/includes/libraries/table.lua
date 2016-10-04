--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: table.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
function table.Merge(dest, source)

  for k,v in pairs(source) do
  
    if ( type(v) == 'table' and type(dest[k]) == 'table' ) then
      -- don't overwrite one table with another;
      -- instead merge them recurisvely
      table.Merge(dest[k], v)
    else
      dest[k] = v
    end
  end
  
  return dest
end
