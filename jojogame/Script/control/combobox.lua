require "control.lua"
require "text_control.lua"

-- @class ComboBox : 게임 내의 Combo Box 컨트롤을 Wrapping 하는 클래스
-- @inherit Control TextControl
ComboBox = Inherit(Control, TextControl)

-- @description 기존 컨트롤을 받아와 ComboBox 클래스를 생성합니다.
-- @param control control : 기존의 ComboBox 컨트롤
-- @return ComboBox : 기존 control 의 wrapping 된 ComboBox 클래스
function ComboBox:Instance(control)
    local newControl = {}
    
    newControl.control = control
    setmetatable(newControl, ComboBox)
    ComboBox.__index = ComboBox
    
    return newControl
end

-- @description 콤보 박스를 새로 생성합니다.
-- @param parent Window : 콤보 박스가 생성될 부모, 보통 Window
-- @return ComboBox : 새로 생성된 ComboBox class
function ComboBox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateComboBox(parentControl)
    setmetatable(newControl, ComboBox)
    ComboBox.__index = ComboBox
    
    return newControl
end

-- @description 아이템을 추가합니다.
-- @param text string : 아이템 텍스트
function ComboBox:AddItem(text)
    if text ~= nil then
        self.control:AddItem(text)
    end
end