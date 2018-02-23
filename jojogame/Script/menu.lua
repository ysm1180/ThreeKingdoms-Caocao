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

MenuItem = Object:Instance
{
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

    SetChildMenu = function(self, menu)
        if menu ~= nil then
            self.control:SetChildMenu(menu.control)
        end

        OUTPUT("Set Child Menu")
    end,
}