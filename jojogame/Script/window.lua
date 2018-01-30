require "Script\\control.lua"

Window = Control:Instance
{
    Instance = function(self, parent)
        local newWindow = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        self.control = controlManager:CreateWindowForm(parent)
        setmetatable(newWindow, self)
        self.__index = self
        
        return newWindow
    end,

    SetBackColor = function(self, color)
        self.control:SetBackColor(gameManager:Color(color.R, color.G, color.B))
    end,

    SetActiveEvent = function(self, activeEventName)
        self.control:SetActiveEvent(activeEventName)
    end,

    SetCloseEvent = function(self, closeEventName)
        self.control:SetCloseEvent(closeEventName)
    end
}