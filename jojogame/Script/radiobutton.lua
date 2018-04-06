require "Script\\control.lua"
require "Script\\text_control.lua"

RadioButton = Inherit(Control, TextControl)

function RadioButton:Instance(control)
    local newControl = {}
        
    newControl.control = control
    setmetatable(newControl, RadioButton)
    RadioButton.__index = RadioButton

    OUTPUT("Get Radio Button Instance")

    return newControl
end

function RadioButton:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end
    newControl.control = controlManager:CreateRadioButton(parentControl)
    setmetatable(newControl, RadioButton)
    RadioButton.__index = RadioButton

    OUTPUT("Make Radio Button Instance")

    return newControl
end

function RadioButton:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

function RadioButton:SetChecked(value)
    if type(value) == "boolean" then
        self.control:SetChecked(value)
    end
end

function RadioButton:Checked()
    return self.control:IsChecked()
end