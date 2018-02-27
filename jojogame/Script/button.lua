require "Script\\control.lua"
require "Script\\text_control.lua"

Button = Inherit(Control, TextControl)

Button.DEFAULT_BACKGROUND_COLOR = {
    NORMAL = {R = 0xE1, G = 0xE1, B = 0xE1},
    FOCUSED = {R = 0x00, G = 0x78, B = 0xD7},
    PUSHED = {R = 0x00, G = 0x54, B = 0x99}
}
Button.DEFAULT_BORDER_COLOR = {
    NORMAL = {R = 0xAD, G = 0xAD, B = 0xAD},
    FOCUSED = {R = 0x00, G = 0x78, B = 0xD7},
    PUSHED = {R = 0x00, G = 0x54, B = 0x99}
}
Button.DEFAULT_TEXT_COLOR = {
    NORMAL = {R = 0x00, G = 0x00, B = 0x00},
    FOCUSED = {R = 0x00, G = 0x00, B = 0x00},
    PUSHED = {R = 0x00, G = 0x00, B = 0x00}
}

function Button:New(parent)
    local newButton = {}
    local parentControl = nil

    if parent ~= nil then
        parentControl = parent.control
    end
    newButton.control = controlManager:CreateButton(parentControl)
    setmetatable(newButton, Button)
    Button.__index = Button

    OUTPUT("Make Button Instance")

    return newButton
end

function Button:SetText(text)
    self.control:SetText(text)
end

function Button:SetTransparentBackground(value)
    if type(value) == "boolean" then
        self.control:SetTransparentBackground(value)
        OUTPUT("Set Transparent Background : " .. tostring(value))
    end
end

function Button:SetBorderWidth(width)
    self.control:SetBorderWidth(width)
end

function Button:SetNormalBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
        self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Normal Background Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetFocusedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
        self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Focused Background Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetPushedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.PUSHED.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.PUSHED.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.PUSHED.B
        self.control:SetPushedBackgroundColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Pushed Background Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetNormalBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.NORMAL.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.NORMAL.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.NORMAL.B
        self.control:SetBorderColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Normal Border Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetFocusedBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.FOCUSED.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.FOCUSED.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.FOCUSED.B
        self.control:SetFocusedBorderColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Focused Border Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetPushedBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.PUSHED.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.PUSHED.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.PUSHED.B
        self.control:SetPushedBorderColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Pushed Border Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetNormalTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
        self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Normal Text Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.FOCUSED.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.FOCUSED.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.FOCUSED.B
        self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Focused Text Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end

function Button:SetPushedTextColor( color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.PUSHED.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.PUSHED.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.PUSHED.B
        self.control:SetPushedTextColor(gameManager:Color(color.R, color.G, color.B))

        OUTPUT(
            "Set Pushed Text Color #" ..
                string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
        )
    end
end
