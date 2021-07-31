require "control.lua"
require "text_control.lua"

---@class RadioButton : ChildControl, ExtendFontControl 게임 내의 Radio Button 컨트롤을 Wrapping 하는 클래스
RadioButton = Class(ChildControl, ExtendFontControl)

---기존 컨트롤을 받아와 RadioButton 클래스를 생성합니다.
---@param control any
---@return RadioButton
function RadioButton:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, RadioButton)
    RadioButton.__index = RadioButton

    return newControl
end

---라디오 버튼을 새로 생성합니다.
---@param parent Window @라디오 버튼이 생성될 부모, 보통 Window
---@return RadioButton
function RadioButton:New(parent, isGroupStart)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateRadioButton(parentControl, isGroupStart)
    setmetatable(newControl, RadioButton)
    RadioButton.__index = RadioButton

    return newControl
end

---텍스트를 설정합니다.
---@param text string 
function RadioButton:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---체크 여부를 설정합니다.
---@param value boolean 
function RadioButton:SetChecked(value)
    if type(value) == "boolean" then
        self.control:SetChecked(value)
    end
end

---체크 여부를 반환합니다.
---@return boolean 
function RadioButton:Checked()
    return self.control:IsChecked()
end
