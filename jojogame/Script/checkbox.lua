require "Script\\control.lua"
require "Script\\text_control.lua"

CheckBox = Inherit(Control, TextControl)

function CheckBox:Instance(control)
    local newControl = {}
    
    newControl.control = control
    setmetatable(newControl, CheckBox)
    CheckBox.__index = CheckBox
    
    OUTPUT("Get Check Box Instance")
    
    return newControl
end

function CheckBox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end
    newControl.control = controlManager:CreateCheckBox(parentControl)
    setmetatable(newControl, CheckBox)
    CheckBox.__index = CheckBox
    
    OUTPUT("Make Check Box Instance")
    
    return newControl
end

function CheckBox:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

function CheckBox:SetChecked(value)
    if type(value) == "boolean" then
        self.control:SetChecked(value)
    end
end

function CheckBox:Checked()
    return self.control:IsChecked()
end
