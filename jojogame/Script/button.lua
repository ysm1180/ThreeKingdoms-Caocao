require "Script\\text_control.lua"

Button = TextControl:Instance
{
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

    SetTransparentBorder = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetTransparentBorder(value)
        else
            self.control:SetTransparentBorder(false)
        end
    end
}