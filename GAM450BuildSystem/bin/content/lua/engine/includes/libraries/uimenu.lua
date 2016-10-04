--[[*****************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: uimenu.lua
 * Author:
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
*****************************************************************]]
if not uimenu then
  uimenu = {}
  uimenu.list = {}
  uimenu.meta = {}
end

function uimenu.Create(vStep, hStep, gamepads)
  -- Initialize button table
  Log(DEBUG, "Created menu")

  local newMenu = {}
  newMenu.buttons = {}

  newMenu.numButtons = 0
  newMenu.selected = 1
  newMenu.verticalStep = vStep or 1
  newMenu.horizontalStep = hStep or 1
  newMenu.gamepads = gamepads or {}
  newMenu.padMoveCD = {0,0,0,0}
  newMenu.padClickCD = {0,0,0,0}
  newMenu.inputCD = 200
  newMenu.active = true
  newMenu.wait = true

  table.Merge(newMenu, uimenu.meta)

  table.insert(uimenu.list, newMenu)

  newMenu.menuID = #uimenu.list

  return newMenu
end

CreateMenu = uimenu.Create

function uimenu.Update()
  for id, realMenu in pairs(uimenu.list) do
    if realMenu.active then
      realMenu:Update()
    end
  end
end

function uimenu.Delete(realMenu)
  local id = realMenu.menuID

  Log(DEBUG, "Deleted menu: "..id)

  local realID = -1



  for k,v in pairs(uimenu.list) do
    if v.menuID == id then
      realID = k
      break
    end
  end

  if realID == -1 then return end

  if uimenu.list[realID] then
    uimenu.list[realID].buttons = {}
    table.remove(uimenu.list, realID)
  end

  --Log(DEBUG, "Current menuss left: ".. #uimenu.list)
  
end

function uimenu.UpdateMeta()
  for k,v in pairs(uimenu.list) do
    table.Merge(v, uimenu.meta)
  end
end



function uimenu.meta:SetHStep(step)
  self.horizontalStep = step
end

function uimenu.meta:SetVStep(step)
  self.verticalStep = step
end

function uimenu.meta:Destroy()
  uimenu.Delete(self)
end

function uimenu.meta:SetActive(active)
  self.active = active
  self.wait = active
end

function uimenu.meta:RegisterButton(btn)
  -- Insert button into menu
  table.insert(self.buttons, btn)

  self.numButtons = self.numButtons + 1

  -- Tell our button about its menu index
  btn.menuBtnID = self.numButtons
  btn.menuID = self.menuID

  -- If that was the first button to be added set it to hovered
  if self.numButtons == 1 then
    self.buttons[1]:SetHovered(true)
  end

  Log(INFO, "Registered button into menu at index: "..#self.buttons)
end
uimenu.meta.Add = uimenu.meta.RegisterButton

function uimenu.meta:InsertButton(btn, index)
  -- Insert our button at the index in the menu
  table.insert(self.buttons, index, btn)

  btn.menuIndex = index

  self.numButtons = self.numButtons + 1

  -- It happened that we inserted a button in the place or before
  -- the currently selected button, we should update that
  if self.selected >= index then
    self.selected = self.selected + 1
  end

  Log(INFO, "Registered button into menu at index: "..index)
end

function uimenu.meta:RemoveButton(btn, index)
  -- Take out the removed button
  table.remove(self.button, index)

  self.numButtons = self.numButtons - 1

  if self.selected == index then
    self.selected = self.selected - 1
    self.button[self.selected]:SetHovered(true)
  elseif self.selected > index then
    self.selected = self.selected - 1
  end

  Log(INFO, "Removed button from menu at index: "..index)
end

function uimenu.meta:FindButtonIndex(btn)
  for index, button in pairs(self.buttons) do
    if button == btn then
      return index
    end
  end

  return -1
end

function uimenu.meta:MoveCooldown(padnum)
  self.padMoveCD[padnum] = CurTime() + self.inputCD
end

function uimenu.meta:ClickCooldown(padnum)
  self.padClickCD[padnum] = CurTime() + self.inputCD
end

function uimenu.meta:Select(newSelection)
    -- Tell our old selection to no longer be selected
    self.buttons[self.selected]:SetHovered(false)

    -- Update our current selection
    self.selected = newSelection
    self.buttons[self.selected]:SetHovered(true)
end

local stickThreshold = 0.5

function uimenu.meta:Update()

  local clicked = false
  local hMove = 0
  local vMove = 0
  local stick, dPad, gamePad

  local cTime = CurTime()

  if self.wait then
    self.wait = false
    return
  end

  --if self.numButtons == 0 then return end

  -- Interpret movement
  for i, pad in pairs(self.gamepads) do
    if cTime > self.padMoveCD[pad + 1] then
      gamePad = GamePad(pad)

      if gamePad:Connected() then

        if not gamePad:InDeadzone(false) then
          stick = gamePad:LeftStick()

          if stick.y > stickThreshold then
            vMove = -1
          elseif stick.y < -stickThreshold then
            vMove = 1
          end

          if stick.x > stickThreshold then
            hMove = -1
          elseif stick.x < -stickThreshold then
            hMove = 1
          end

          if (hMove ~= 0 and self.horizontalStep ~= 0) or
            (vMove ~= 0 and self.verticalStep ~= 0) then
            self:MoveCooldown(pad + 1)
            break
          end
        elseif
          not gamePad:InDeadzone(true) then
          stick = gamePad:RightStick()

          if stick.y > stickThreshold then
            vMove = -1
          elseif stick.y < -stickThreshold then
            vMove = 1
          end

          if stick.x > stickThreshold then
            hMove = -1
          elseif stick.x < -stickThreshold then
            hMove = 1
          end

          if (hMove ~= 0 and self.horizontalStep ~= 0) or
            (vMove ~= 0 and self.verticalStep ~= 0) then
            self:MoveCooldown(pad + 1)
            break
          end
        end

        if gamePad:ButtonDown(GAMEPAD_DPAD_LEFT) or KeyIsPressed(KEY_A) then
          hMove = 1
        end

        if gamePad:ButtonDown(GAMEPAD_DPAD_RIGHT) or KeyIsPressed(KEY_D) then
          hMove = -1
        end

        if gamePad:ButtonDown(GAMEPAD_DPAD_UP) or KeyIsPressed(KEY_W) then
          vMove = -1
        end

        if gamePad:ButtonDown(GAMEPAD_DPAD_DOWN) or KeyIsPressed(KEY_S) then
          vMove = 1
        end

        if (hMove ~= 0 and self.horizontalStep ~= 0) or
          (vMove ~= 0 and self.verticalStep ~= 0) then
          self:MoveCooldown(pad + 1)
          break
        end

      end
    end
  end

  if KeyIsPressed(KEY_A) then
    hMove = 1
  end

  if KeyIsPressed(KEY_D) then
    hMove = -1
  end

  if KeyIsPressed(KEY_W) then
    vMove = -1
  end

  if KeyIsPressed(KEY_S) then
    vMove = 1
  end

  if KeyIsPressed(KEY_ENTER) then
    clicked = true
  end

  -- Interpret clicking
  for i, pad in pairs(self.gamepads) do
    if cTime > self.padClickCD[pad + 1] then
      gamePad = GamePad(pad)

      if gamePad:ButtonPressed(GAMEPAD_A) and gamePad:Connected() then
        clicked = true
        self:ClickCooldown(pad + 1)
      end
    end
  end

  -- Keyboard input



  -- if hMove ~= 0 then
  --   print("hortizontal movement: "..hMove)
  -- end
  -- if vMove ~= 0 then
  --   print("vertical movement: "..vMove)
  -- end

  -- if self.numButtons == 0 then return end

  local newSelection = self.selected

  if hMove ~= 0 and self.horizontalStep ~= 0 then
    --print("hortizontal movement: "..hMove)

    hMove = hMove * self.horizontalStep
    newSelection = newSelection + hMove

    -- Wrap around if we went under 1 or above max buttons
    if newSelection < 1 then
      newSelection = self.numButtons + newSelection
    end

    if newSelection > self.numButtons then
      newSelection = newSelection - self.numButtons
    end

    -- Tell our old selection to no longer be selected
    self.buttons[self.selected]:SetHovered(false)

    -- Update our current selection
    self.selected = newSelection
    self.buttons[self.selected]:SetHovered(true)
  end

  if vMove ~= 0 and self.verticalStep ~= 0 then
    --print("vertical movement: "..vMove)

    vMove = vMove * self.verticalStep
    newSelection = newSelection + vMove

    -- Wrap around if we went under 1 or above max buttons
    if newSelection < 1 then
      newSelection = self.numButtons + newSelection
    end

    if newSelection > self.numButtons then
      newSelection = newSelection - self.numButtons
    end

    -- Tell our old selection to no longer be selected
    self.buttons[self.selected]:SetHovered(false)

    -- Update our current selection
    self.selected = newSelection
    self.buttons[self.selected]:SetHovered(true)
  end

  if clicked then
    self.buttons[self.selected]:Press()
  end

end