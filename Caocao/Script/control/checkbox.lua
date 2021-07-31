require "text_control.lua"

---@class Checkbox : ChildControl, ExtendFontControl @게임 내의 Checkbox 컨트롤을 Wrapping 하는 클래스
Checkbox = Class(ChildControl, ExtendFontControl)

---Checkbox object를 새로 생성합니다.
---@param parent Window @Checkbox가 생성될 부모, 보통 Window
---@return Checkbox
function Checkbox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateCheckbox(parentControl)
    setmetatable(newControl, Checkbox)
    Checkbox.__index = Checkbox

    return newControl
end

---텍스트를 설정합니다.
---@param text string
function Checkbox:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---체크 여부를 설정합니다.
---@param checked boolean
function Checkbox:SetChecked(checked)
    if type(checked) == "boolean" then
        self.control:SetChecked(checked)
    end
end

---체크 여부를 반환합니다.
---@return boolean
function Checkbox:Checked()
    return self.control:IsChecked()
end
