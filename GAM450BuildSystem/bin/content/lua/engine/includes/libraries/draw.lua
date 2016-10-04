--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: draw.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
--[[ Draw Library: GUI wrapper and helper functions ]]

local pairs = pairs
local table = table
local surface = surface
local string = string
local math = math
local setmetatable = setmetatable
local Color = Color
local tostring = tostring
local surface = surface

if not draw then draw = {} end

TEXT_ALIGN_LEFT = 0
TEXT_ALIGN_CENTER = 1
TEXT_ALIGN_RIGHT = 2
TEXT_ALIGN_TOP = 3
TEXT_ALIGN_BOTTOM = 4

local Corner8   = nil 
local Corner16  = nil 

function draw.UpdateCorners()
  Corner8 = surface.GetTextureID("gui/corner8.png")
  Corner16 = surface.GetTextureID("gui/corner16.png")
end

function draw.SimpleText(text, font, x, y, size, color, xalign, yalign)

  text = tostring( text )
  font = font or 0
  x = x or 0
  y = y or 0
  xalign = xalign or TEXT_ALIGN_LEFT
  yalign = yalign or TEXT_ALIGN_TOP


  local stringSize

  stringSize = surface.MeasureString(text, size, font)

  if (xalign == TEXT_ALIGN_CENTER) then
    x = x - stringSize.x/2
  elseif (xalign == TEXT_ALIGN_RIGHT) then
    x = x - stringSize.x
  end
  
  if (yalign == TEXT_ALIGN_CENTER) then
    y = y - stringSize.y/2
  elseif ( yalign == TEXT_ALIGN_BOTTOM ) then
    y = y - stringSize.y
  end
  
  surface.SetPos( math.ceil( x ), math.ceil( y ) );
  
  if (color ~= nil) then
    local alpha = 255
    if (color.a) then alpha = color.a end

    surface.SetColor( color.r, color.g, color.b, alpha)
  else
    surface.SetColor(255, 255, 255, 255)
  end
  

  surface.DrawString(text, size, font)
  
  return w, h
  
end

--text, font, x, y, size, color, xalign, yalign, outlinewidth, outlinecolor
function draw.SimpleTextOutlined(text, font, x, y, size, color, xalign, yalign, outlinewidth, outlinecolor)

  local steps = (outlinewidth*2) / 3
  if ( steps < 1 )  then steps = 1 end
  
  for _x=-outlinewidth, outlinewidth, steps do
    for _y=-outlinewidth, outlinewidth, steps do
      draw.SimpleText(text, font, x + (_x), y + (_y), size,
       outlinecolor, xalign, yalign)
    end
  end
  
  return draw.SimpleText(text, font, x, y, size, color, xalign, yalign)
  
end

local gmatch = string.gmatch
local find = string.find
local ceil = math.ceil
local max = math.max
function draw.DrawText(text, font, x, y, size, color, xalign )

  if (font == nil) then font = "Arial" end
  if (text ~= nil) then text = tostring( text ) end
  if (x == nil) then x = 0 end
  if (y == nil) then y = 0 end

  local curX = x
  local curY = y
  local curString = ""
  
  --surface.SetFont(font)
  local stringSize = surface.MeasureString("\n", size, font)
  local lineHeight = stringSize.y
  local tabWidth = 50
  
  for str in gmatch( text, "[^\n]*" ) do
    if #str > 0 then
      if find( str, "\t" ) then -- there's tabs, some more calculations required
        for tabs, str2 in gmatch( str, "(\t*)([^\t]*)" ) do
          curX = ceil( (curX + tabWidth * max(#tabs-1,0)) / tabWidth )
          * tabWidth
          
          if #str2 > 0 then
            SimpleText( str2, font, curX, curY, color, xalign )
          
            local str2Size = surface.MeasureString(str2, size, font)
            curX = curX + str2Size.x
          end
        end
      else -- there's no tabs, this is easy
        SimpleText( str, font, curX, curY, size, color, xalign )
      end
      
      curX = x
      curY = curY + (lineHeight/2)
    end
  end
end

function draw.RoundedBox( bordersize, x, y, w, h, color )

  return draw.RoundedBoxEx( bordersize, x, y, w, h, color, true, true, true, true )

end

function draw.RoundedBoxEx( bordersize, x, y, w, h, color, a, b, c, d )

  x = math.Round( x )
  y = math.Round( y )
  w = math.Round( w )
  h = math.Round( h )

  surface.SetColor( color.r, color.g, color.b, color.a )

  surface.SetRotation(0)
  
  -- Draw as much of the rect as we can without textures
  surface.DrawRect( x+bordersize, y, w-bordersize*2, h )
  surface.DrawRect( x, y+bordersize, bordersize, h-bordersize*2 )
  surface.DrawRect( x+w-bordersize, y+bordersize, bordersize, h-bordersize*2 )
  
  if Corner8 == nil then
    draw.UpdateCorners()
  end

  local tex = Corner8
  if ( bordersize > 8 ) then tex = Corner16 end
  
  surface.SetTexture( tex )
  
  if ( a ) then
    surface.DrawTexturedRectRotated( x , y, bordersize, bordersize, 0 ) 
  else
    surface.DrawRect( x, y, bordersize, bordersize )
  end
  
  if ( b ) then
    surface.DrawTexturedRectRotated( x + w - bordersize, y, bordersize, bordersize, math.pi * 1.5 ) 
  else
    surface.DrawRect( x + w - bordersize, y, bordersize, bordersize )
  end
 
  if ( c ) then
    surface.DrawTexturedRectRotated( x , y + h - bordersize, bordersize, bordersize, math.pi * 0.5 )
  else
    surface.DrawRect( x, y + h - bordersize, bordersize, bordersize )
  end
 
  if ( d ) then
    surface.DrawTexturedRectRotated( x + w - bordersize, y + h - bordersize, bordersize, bordersize, math.pi )
  else
    surface.DrawRect( x + w - bordersize, y + h - bordersize, bordersize, bordersize )
  end
  
end
