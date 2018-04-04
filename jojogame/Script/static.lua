require "Script\\control.lua"
require "Script\\text_control.lua"

Static = Inherit(Control, TextControl)

function Static:Instance(control)
    local newControl = {}
        
    newControl.control = control
    setmetatable(newControl, Static)
    Static.__index = Static

    OUTPUT("Get Static Instance")

    return newControl
end

function Static:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end
    newControl.control = controlManager:CreateStatic(parentControl)
    setmetatable(newControl, Static)
    Static.__index = Static

    OUTPUT("Make Static Instance")

    return newControl
end

function Static:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

function Static:SetAutoSize(value)
    if type(value) == "boolean" then
        self.control:SetAutoSize(value)
    end
end

function Static:SetAlign(align)
    if align ~= nil then
        self.control:SetAlign(align)
    end
end

function Static:SetTransparentBackground(value)
    if type(value) == "boolean" then
        self.control:SetTransparentBackground(value)
    end
end

function Static:SetBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or 0
        color.G = color.G or 0
        color.B = color.B or 0
        self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Background Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Static:SetTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or 0
        color.G = color.G or 0
        color.B = color.B or 0
        self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Background Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end