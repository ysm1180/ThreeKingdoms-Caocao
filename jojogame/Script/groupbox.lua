require "Script\\control.lua"
require "Script\\text_control.lua"

GroupBox = Inherit(Control, TextControl)

function GroupBox:Instance(control)
    local newControl = {}
        
    newControl.control = control
    setmetatable(newControl, GroupBox)
    GroupBox.__index = GroupBox

    OUTPUT("Get Group Box Instance")

    return newControl
end

function GroupBox:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end
    newControl.control = controlManager:CreateGroupBox(parentControl)
    setmetatable(newControl, GroupBox)
    GroupBox.__index = GroupBox

    OUTPUT("Make Group Box Instance")

    return newControl
end

function GroupBox:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end