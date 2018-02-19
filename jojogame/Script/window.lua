require "Script\\control.lua"

Window = Control:Instance
{
    Instance = function(self, parent)
        local newWindow = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        newWindow.control = controlManager:CreateWindowForm(parentControl)
        setmetatable(newWindow, self)
        self.__index = self

        OUTPUT("Make Window Intance")        
        
        return newWindow
    end,

    SetBackColor = function(self, color)
        if color ~= nil then
            self.control:SetBackColor(gameManager:Color(color.R or 0, color.G or 0, color.B or 0))

            OUTPUT("Set Background Color #" .. string.format('%02x', color.R or 0) .. string.format('%02x', color.G or 0) .. string.format('%02x', color.B or 0) )
        else
            self.control:SetBackColor(gameManager:Color(0, 0, 0))

            OUTPUT("Set Background Color #000")
        end
    end,

    SetActiveEvent = function(self, event)
        self.control:SetActiveEvent(event)

        OUTPUT("Set Active Event : " .. (event or "NULL"))
    end,

    SetCloseEvent = function(self, event)
        self.control:SetCloseEvent(event)

        OUTPUT("Set Active Event : " .. (event or "NULL"))
    end,

    SetTitleName = function(self, title)
        self.control:SetTitleName(title)

        OUTPUT("Set Title Name : " .. (title or ""))        
    end,

    ShowModalWindow = function(self)
        OUTPUT("Show modal window")

        self.control:ShowModalWindow()
    end,

    SetMaxButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMaxButton(value)
        else
            self.control:SetMaxButton(false)
        end

        OUTPUT("Set Max Button : " .. tostring(value))
    end,

    SetMinButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMinButton(value)
        else
            self.control:SetMinButton(false)
        end

        OUTPUT("Set Min Button : " .. tostring(value))
    end,

    SetControlBox = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetControlBox(value)
        else
            self.control:SetControlBox(false)
        end

        OUTPUT("Set Control Box : " .. tostring(value))
    end,

    SetTitlebar = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetTitlebar(value)
        else
            self.control:SetControlBox(false)
        end

        OUTPUT("Set Title bar : " .. tostring(value))
    end,
}