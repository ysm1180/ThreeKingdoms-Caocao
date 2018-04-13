require "control.lua"
require "text_control.lua"

ComboBox = Inherit(Control, TextControl)

function ComboBox:Instance(control)
    local newControl = {}
    
    newControl.control = control
    setmetatable(newControl, ComboBox)
    ComboBox.__index = ComboBox
    
    return newControl
end

function ComboBox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end
    newControl.control = controlManager:CreateComboBox(parentControl)
    setmetatable(newControl, ComboBox)
    ComboBox.__index = ComboBox
    
    return newControl
end

function ComboBox:AddItem(text)
    if text ~= nil then
        self.control:AddItem(text)
    end
end