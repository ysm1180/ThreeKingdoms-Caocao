require "control.lua"

Window = Control:Instance
{
    Instance = function(self, parent)
        local newWindow = {}

        self.window = controlManager:CreateWindowForm(parent)
        setmetatable(newWindow, self)
        self.__index = self
        
        return newWindow
    end,

    SetBackColor = function(self, color)
        self.window:SetBackColor(gameManager:Color(color.R, color.G, color.B))
    end,

}