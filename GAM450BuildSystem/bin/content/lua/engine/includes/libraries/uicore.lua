--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: uicore.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
if PanelFactory == nil then PanelFactory = {} end

local panelMetaTable = GetMeta("Panel")

if gui == nil then gui = {} end
if gui.panels == nil then gui.panels = {} end
if gui.panelcounter == nil then gui.panelcounter = 0 end


function gui.GetPanelMeta(name)
  return PanelFactory[name]
end

local function SortPanels(p1, p2)
  if not p2 then return nil end
  if not p1 then return nil end
  return p1.priority < p2.priority
end

function gui.Create( classname, parent, name, ...)
  local panel = gui.Make(classname, parent, name, ...)

  if parent then
    -- local parentTable = gui.panels[parent.id]
    -- parentTable[#parentTable + 1] = panel
    -- panel.id = -(#parentTable)
    parent.children[#parent.children + 1] = panel
  else
    panel.id = #gui.panels + 1
    gui.panels[#gui.panels + 1] = panel
    table.sort(gui.panels, SortPanels)
    
    --gui.panelcounter = gui.panelcounter + 1
    panel.children = {}
  end

  panel.parent = parent
  
  return panel
end

function gui.Make( classname, parent, name, ...)
  if PanelFactory[classname] ~= nil then

    local metatable = PanelFactory[classname]
    local panel = gui.Make( metatable.basePanelClass, parent, name )
    if ( not panel ) then
      Error( "Tried to create panel with invalid base '"
        ..metatable.basePanelClass.."'\n" );
    end

    table.Merge( panel, metatable )

    panel.basePanelClass = metatable.basePanelClass
    panel.className = classname


    
    -- Call the Init function if we have it
    if ( panel.Init ) then
      panel:Init()  
    end
      
    panel:Prepare()
    
    return panel
  else
    -- Must be the base Panel class
    local panel = setmetatable({}, panelMetaTable)

    if panel.Init then
      panel:Init()
    end

    panel:Prepare()

    return panel
  end
end

function gui.Register( name, mtable, base )

  -- Remove the global
  --PANEL = nil
  mtable.basePanelClass = base or "Panel"

  -- mtable.__oldIndex = mtable.__index
  -- mtable.__index = function (t, k)
  --   print("test")
  --   if PanelFactory[mtable.baseClass] and
  --     PanelFactory[mtable.baseClass][k] then
  --     return PanelFactory[mtable.baseClass][k] end

  --   if k == "super" then
  --     print("Super?")
  --     return PanelFactory[mtable.baseClass]
  --   end

  --   return t.__oldIndex(t, k)
  -- end

  
  PanelFactory[ name ] = mtable

  -- local mt = {}
  -- mt.__index = function( t, k )
  
  --   if ( PanelFactory[ mtable.BasePanel ] && PanelFactory[ mtable.BasePanel ][k] ) then return PanelFactory[ mtable.BasePanel ][k] end
  --   return panel_metatable[k]

  -- end

  -- setmetatable( mtable, mt )

  return mtable

end

function gui.Update()
  local removeList = {}
  local deadChildren = {}


  for k,v in pairs(gui.panels) do
    v:_BaseThink()

    -- Check to see if any of the panels children are dead
    for index, child in pairs(v.children) do
      if not child.valid then
        deadChildren[#deadChildren + 1] = index
      end
    end

    -- Take care of dead children
    for _, child in pairs(deadChildren) do
      v.children[child]:_BaseDelete()
      v.children[child] = nil
    end

    deadChildren = {}

    -- Add the parent to the remove list if it's invalid now
    if not v.valid then
      removeList[#removeList + 1] = k
    end
  end

  for k,panel in pairs(removeList) do
    gui.panels[panel]:_BaseDelete()
    gui.panels[panel] = nil
  end

  if #removeList > 0 then
    table.sort(gui.panels, SortPanels)
  end
end

function gui.Draw()
  --Draw stuff
  for k,v in pairs(gui.panels) do
    --print("Painting "..k)
    if v.visible then
      --print("PAINT")
      v:_BasePaint()
      --print("EPAINT")
    end
  end
  hook.Call("GUIDRAW")
end

local function RefreshMeta(panel)
  local metatable = PanelFactory[panel.className]
  local basetable = PanelFactory[panel.basePanelClass]

  if basetable ~= nil then
    table.Merge( panel, basetable )
  end

  if metatable ~= nil then
    table.Merge( panel, metatable )
  end

  for k,v in pairs(panel.children) do
    RefreshMeta(v)
  end
end

function gui.Reloaded()
  for k, panel in pairs(gui.panels) do
    RefreshMeta(panel)
  end
end
