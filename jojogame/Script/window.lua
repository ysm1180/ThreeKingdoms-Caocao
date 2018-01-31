require "Script\\control.lua"

Window = Control:Instance
{
    Instance = function(self, parent)
        local newWindow = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        self.control = controlManager:CreateWindowForm(parentControl)
        setmetatable(newWindow, self)
        self.__index = self
        
        return newWindow
    end,

    SetBackColor = function(self, color)
        self.control:SetBackColor(gameManager:Color(color.R, color.G, color.B))
    end,

    SetActiveEvent = function(self, event)
        self.control:SetActiveEvent(event)
    end,

    SetCloseEvent = function(self, event)
        self.control:SetCloseEvent(event)
    end,

    SetTitleName = function(self, title)
        self.control:SetTitleName(title)
    end,

    ShowModalWindow = function(self)
        self.control:ShowModalWindow()
    end,

    SetMaxButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMaxButton(value)
        else
            self.control:SetMaxButton(false)
        end
    end,

    SetMinButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMinButton(value)
        else
            self.control:SetMinButton(false)
        end
    end,

    SetControlBox = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetControlBox(value)
        else
            self.control:SetControlBox(false)
        end
    end
}