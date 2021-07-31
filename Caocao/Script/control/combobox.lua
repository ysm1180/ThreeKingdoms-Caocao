require "text_control.lua"

---@class Combobox : ChildControl, ExtendFontControl @게임 내의 Combobox 컨트롤을 Wrapping 하는 클래스
Combobox = Class(ChildControl, ExtendFontControl)

---기존 컨트롤을 받아와 Combobox 클래스를 생성합니다.
---@param control any
---@return Combobox
function Combobox:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, Combobox)
    Combobox.__index = Combobox

    return newControl
end

---Combobox를 새로 생성합니다.
---@param parent Window @Combobox가 생성될 부모, 보통 Window
---@return Combobox
function Combobox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateComboBox(parentControl)
    setmetatable(newControl, Combobox)
    Combobox.__index = Combobox

    return newControl
end

---아이템을 추가합니다.
---@param text string
function Combobox:AddItem(itemText)
    if text ~= nil then
        Combobox.control:AddItem(itemText)
    end
end
