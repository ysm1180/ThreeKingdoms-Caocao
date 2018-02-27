require "Script\\object.lua"

Menu = Object:Instance
{
    Instance = function(self)
        local newMenu = {}

        newMenu.control = controlManager:CreateMenu()
        setmetatable(newMenu, self)
        self.__index = self

        OUTPUT("Make Menu Instance")        
        
        return newMenu
    end,

    

    AddMenuItem = function(self, menuItem)
        if menuItem ~= nil then
            self.control:AddMenuItem(menuItem.control)
            OUTPUT("Add Menu item")
        end
    end,
}

MenuItem = TextControl:Instance
{
    DEFAULT_BACKGROUND_COLOR = {
        NORMAL = {R = 0xF2, G = 0xF2, B = 0xF2},
        FOCUSED = {R = 0x91 , G = 0xC9, B = 0xF7},
        DISABLED = {R = 0xF2 , G = 0xF2, B = 0xF2},
        DISABLE_FOCUSED = {R = 0xE6, G = 0xE6, B = 0xE6},
    },
    DEFAULT_TEXT_COLOR = {
        NORMAL = {R = 0x00, G = 0x00, B = 0x00},
        FOCUSED = {R = 0x00, G = 0x00, B = 0x00},
        DISABLED = {R = 0x6D , G = 0x6D, B = 0x6D},
        DISABLE_FOCUSED = {R = 0x6D, G = 0x6D, B = 0x6D},
    },

    Instance = function(self)
        local newMenuItem = {}

        newMenuItem.control = controlManager:CreateMenuItem()
        setmetatable(newMenuItem, self)
        self.__index = self

        OUTPUT("Make Menu Item Instance")        
        
        return newMenuItem
    end,

    Text = function(self)
        return self.control:GetText()
    end,

    SetEnabled = function(self, value)
        if type(value) == "boolean" then
            self.control:SetEnabled(value)
        end
    end,

    SetText = function(self, text)
        self.control:SetText(text)

        OUTPUT("Set Text : " .. (text or ""))
    end,

    SetClickEvent = function(self, event)
        self.control:SetClickEvent(event)

        OUTPUT("Set Click Event : " .. (event or ""))
    end,

    SetChildMenu = function(self, menuControl)
        if menuControl ~= nil then
            self.control:SetChildMenu(menuControl)
        end

        OUTPUT("Set Child Menu")
    end,

    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Normal Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedBackgroundColor = function(self, color)
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
    end,

    SetDisabledBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLED.B
            self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disalbed Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisableFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disalbe Focused Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetNormalTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Normal Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedTextColor = function(self, color)
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
    end,

    SetDisabledTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLED.B
            self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disabled Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisableFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Focused Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,
}