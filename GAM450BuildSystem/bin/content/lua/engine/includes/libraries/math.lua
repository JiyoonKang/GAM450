--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: math.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
function math.lerp(start, finish, t)
  return start + t*(finish - start)
end



function math.Dist( x1, y1, x2, y2 )
  local xd = x2-x1
  local yd = y2-y1
  return math.sqrt( xd*xd + yd*yd )
end

math.Distance = math.Dist -- alias


function math.Clamp( _in, low, high )
  if (_in < low ) then return low end
  if (_in > high ) then return high end
  return _in
end


function math.Rand( low, high )
  return low + ( high - low ) * math.random()
end


math.Max = math.max

math.Min = math.min


function math.EaseInOut( fProgress, fEaseIn, fEaseOut ) 

  if (fEaseIn == nil) then fEaseIn = 0 end
  if (fEaseOut == nil) then fEaseOut = 1 end

  local fSumEase = fEaseIn + fEaseOut; 

  if( fProgress == 0.0 or fProgress == 1.0 ) then return fProgress end

  if( fSumEase == 0.0 ) then return fProgress end
  if( fSumEase > 1.0 ) then
    fEaseIn = fEaseIn / fSumEase; 
    fEaseOut = fEaseOut / fSumEase; 
  end

  local fProgressCalc = 1.0 / (2.0 - fEaseIn - fEaseOut); 

  if( fProgress < fEaseIn ) then
    return ((fProgressCalc / fEaseIn) * fProgress * fProgress); 
  elseif( fProgress < 1.0 - fEaseOut ) then
    return (fProgressCalc * (2.0 * fProgress - fEaseIn)); 
  else 
    fProgress = 1.0 - fProgress; 
    return (1.0 - (fProgressCalc / fEaseOut) * fProgress * fProgress); 
  end
end



local function KNOT( i, tinc )

  return ( i - 3 ) * tinc;
  
end


function math.calcBSplineN( i, k, t, tinc )

  if ( k == 1 ) then
  
    if ( ( KNOT( i, tinc ) <= t ) and ( t < KNOT( i + 1, tinc ) ) ) then
    
      return 1;
      
    else
    
      return 0;
      
    end
    
  else
    local ft = ( t - KNOT( i, tinc ) ) * math.calcBSplineN( i, k - 1, t, tinc );
    local fb = KNOT( i + k - 1, tinc ) - KNOT( i, tinc );

    local st = ( KNOT( i + k, tinc ) - t ) * math.calcBSplineN( i + 1, k - 1, t, tinc );
    local sb = KNOT( i + k, tinc ) - KNOT( i + 1, tinc );
    
    local first = 0;
    local second = 0;

    if ( fb > 0 ) then
    
      first = ft / fb;
      
    end
    if ( sb > 0 ) then
    
      second = st / sb;
      
    end

    return first + second;
    
  end
  
end


function math.BSplinePoint( tDiff, tPoints, tMax )
  
  local Q = Vector( 0, 0, 0 );
  local tinc = tMax / ( table.getn( tPoints ) - 3 );
  
  tDiff = tDiff + tinc;
  
  for idx, pt in pairs( tPoints ) do
  
    local n = math.calcBSplineN( idx, 4, tDiff, tinc );
    Q = Q + (n * pt);
    
  end
  
  return Q;
  
end



function math.Round(num, idp)

  local mult = 10^(idp or 0)
  return math.floor(num * mult + 0.5) / mult
  
end


function math.Approach( cur, target, inc )

  inc = math.abs( inc )

  if (cur < target) then
    
    return math.Clamp( cur + inc, cur, target )

  elseif (cur > target) then

    return math.Clamp( cur - inc, target, cur )

  end

  return target
  
end

function math.NormalizeAngle( a )
  return ( a + 180 ) % 360 - 180
end


function math.AngleDifference( a, b )

  local diff = math.NormalizeAngle( a - b )
  
  if ( diff < 180 ) then
    return diff
  end
  
  return diff - 360

end

function math.ApproachAngle( cur, target, inc )

  local diff = math.AngleDifference( target, cur )
  
  return math.Approach( cur, cur + diff, inc )
  
end


function math.TimeFraction( Start, End, Current )
  return ( Current - Start ) / ( End - Start )
end

function math.Remap( value, inMin, inMax, outMin, outMax )
  return outMin + ( ( ( value - inMin ) / ( inMax - inMin ) ) * ( outMax - outMin ) )
end
