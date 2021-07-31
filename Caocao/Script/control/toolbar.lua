require "control.lua"

---@class Toolbar : ControlObject @게임 내의 Toolbar 컨트롤을 Wrapping 하는 클래스
Toolbar = Class(ControlObject)

---툴바를 새로 생성합니다.
---@return Toolbar @새로 생성된 Toolbar class
function Toolbar:New()
    local newControl = {
        Parent = nil,
        Visible = false,
    }

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateToolbar()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---툴바 높이를 반환합니다.
---@return number
function Toolbar:Height()
    return self.control:GetHeight()
end

---툴바 버튼을 추가합니다.
---@param button ToolbarButton
function Toolbar:AddButton(button)
    if button ~= nil then
        self.control:AddButton(button.control)
    end
end

---툴바 사이즈를 자동으로 조정합니다.
function Toolbar:AutoSize()
    self.control:AutoSize()
end

---툴바를 생성합니다.
---@param parentWindow Window @툴바가 생성될 Window class
---@param imageWidth number @툴바 버튼의 이미지 너비
---@param imageHeight number @툴바 버튼의 이미지 높이
function Toolbar:Create(parentWindow, imageWidth, imageHeight)
    if parentWindow ~= nil then
        self.Parent = parentWindow
        self.control:Create(parentWindow.control, imageWidth, imageHeight)
    end
end

---툴바를 띄웁니다.
function Toolbar:Show()
    self.Visible = true
    self.control:Show()
end

---툴바를 숨깁니다.
function Toolbar:Hide()
    if self.Parent ~= nil and self.Visible then
        self.Visible = false
    end
    self.Visible = false
    self.control:Hide()
end

