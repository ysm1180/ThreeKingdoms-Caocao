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

        OUTPUT("Make Window Instance")        
        
        return newWindow
    end,

    SetBackColor = function(self, color)
        if color ~= nil then
            color.R = color.R or 0
            color.G = color.G or 0
            color.B = color.B or 0
            self.control:SetBackColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT("Set Background Color #" .. string.format('%02x', color.R) .. string.format('%02x', color.G) .. string.format('%02x', color.B) )
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
        end

        OUTPUT("Set Max Button : " .. tostring(value))
    end,

    SetMinButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMinButton(value)
        end

        OUTPUT("Set Min Button : " .. tostring(value))
    end,

    SetControlBox = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetControlBox(value)
        end

        OUTPUT("Set Control Box : " .. tostring(value))
    end,

    SetTitlebar = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetTitlebar(value)
        end

        OUTPUT("Set Title bar : " .. tostring(value))
    end,

    SetMenu = function(self, menu)
        if menu then
            self.control:SetMenu(menu.control)

            OUTPUT("Set Menu")
        end
    end,

    AddLayout = function(self, layout)
        if layout ~= nil then
            self.control:AddLayout(layout.control)
        end
    end,

    Refresh = function(self)
        self.control:Refresh()
    end,

    RefreshRegion = function(self, rect)
        if rect ~= nil then
            self.control:RefreshRegion(rect.Left, rect.Top, rect.Right, rect.Bottom)
        end
    end,
}