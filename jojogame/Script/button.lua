require "Script\\text_control.lua"

Button = TextControl:Instance {
    DEFAULT_BACKGROUND_COLOR = {
        NORMAL = {R = 0xE1, G = 0xE1, B = 0xE1},
        FOCUSED = {R = 0x00, G = 0x78, B = 0xD7},
        PUSHED = {R = 0x00, G = 0x54, B = 0x99}
    },
    DEFAULT_BORDER_COLOR = {
        NORMAL = {R = 0xAD, G = 0xAD, B = 0xAD},
        FOCUSED = {R = 0x00, G = 0x78, B = 0xD7},
        PUSHED = {R = 0x00, G = 0x54, B = 0x99}
    },
    DEFAULT_TEXT_COLOR = {
        NORMAL = {R = 0x00, G = 0x00, B = 0x00},
        FOCUSED = {R = 0x00, G = 0x00, B = 0x00},
        PUSHED = {R = 0x00, G = 0x00, B = 0x00}
    },

    Instance = function(self, parent)
        local newButton = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        newButton.control = controlManager:CreateButton(parentControl)
        setmetatable(newButton, self)
        self.__index = self

        OUTPUT("Make Button Intance")

        return newButton
    end,

    SetText = function(self, text)
        self.control:SetText(text)
    end,

    SetTransparentBackground = function(self, value)
        if type(value) == "boolean" then
            self.control:SetTransparentBackground(value)
        else
            self.control:SetTransparentBackground(false)
        end
    end,

    SetBorderWidth = function(self, width)
        self.control:SetBorderWidth(width)
    end,

    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Normal Background Color #" ..
                    string.format("%02x", color.R) ..
                        string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Focused Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetPushedBackgroundColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_BACKGROUND_COLOR.PUSHED.R
            color.G = color.G or DEFAULT_BACKGROUND_COLOR.PUSHED.G
            color.B = color.B or DEFAULT_BACKGROUND_COLOR.PUSHED.B
            self.control:SetPushedBackgroundColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Pushed Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetNormalBorderColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_BORDER_COLOR.NORMAL.R
            color.G = color.G or DEFAULT_BORDER_COLOR.NORMAL.G
            color.B = color.B or DEFAULT_BORDER_COLOR.NORMAL.B
            self.control:SetBorderColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Normal Border Color #" ..
                    string.format("%02x", color.R) ..
                        string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedBorderColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_BORDER_COLOR.FOCUSED.R
            color.G = color.G or DEFAULT_BORDER_COLOR.FOCUSED.G
            color.B = color.B or DEFAULT_BORDER_COLOR.FOCUSED.B
            self.control:SetFocusedBorderColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Focused Border Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetPushedBorderColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_BORDER_COLOR.PUSHED.R
            color.G = color.G or DEFAULT_BORDER_COLOR.PUSHED.G
            color.B = color.B or DEFAULT_BORDER_COLOR.PUSHED.B
            self.control:SetPushedBorderColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Pushed Border Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetNormalTextColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Normal Text Color #" ..
                    string.format("%02x", color.R) ..
                        string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedTextColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_TEXT_COLOR.FOCUSED.R
            color.G = color.G or DEFAULT_TEXT_COLOR.FOCUSED.G
            color.B = color.B or DEFAULT_TEXT_COLOR.FOCUSED.B
            self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Focused Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetPushedTextColor = function(self, color)
        if color ~= nil then
            color.R = color.R or DEFAULT_TEXT_COLOR.PUSHED.R
            color.G = color.G or DEFAULT_TEXT_COLOR.PUSHED.G
            color.B = color.B or DEFAULT_TEXT_COLOR.PUSHED.B
            self.control:SetPushedTextColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT(
                "Set Pushed Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end
}
