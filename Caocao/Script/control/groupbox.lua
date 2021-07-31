require "text_control.lua"

---@class Groupbox : ChildControl @게임 내의 Group Box 컨트롤을 Wrapping 하는 클래스
Groupbox = Class(ChildControl, ExtendFontControl)

---기존 컨트롤을 받아와 GroupBox 클래스를 생성합니다.
---@param control any
---@return Groupbox
function Groupbox:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, Groupbox)
    Groupbox.__index = Groupbox

    return newControl
end

---그룹 박스를 새로 생성합니다.
---@param parent Window @그룹 박스가 생성될 부모, 보통 Window
---@return Groupbox
function Groupbox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateGroupBox(parentControl)
    setmetatable(newControl, Groupbox)
    Groupbox.__index = Groupbox

    return newControl
end

---텍스트를 설정합니다.
---@param text string
function Groupbox:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end
