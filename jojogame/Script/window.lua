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

    SetActiveEvent = function(self, evnet)
        self.control:SetActiveEvent(evnet)
    end,

    SetCloseEvent = function(self, evnet)
        self.control:SetCloseEvent(event)
    end,

    SetTitleName = function(self, title)
        self.control:SetTitleName(title)
    end,

    ShowModalWindow = function(self)
        self.control:ShowModalWindow()
    end,

    SetMaxButton = function(self, value)
        self.control:SetMaxButton(value)
    end,

    SetMinButton = function(self, value)
        self.control:SetMinButton(value)
    end,

    SetControlBox = function(self, value)
        self.control:SetControlBox(value)
    end
}