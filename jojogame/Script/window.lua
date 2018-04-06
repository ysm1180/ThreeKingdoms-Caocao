require "Script\\control.lua"

Window = Control:Instance
{
    New = function(self, parent)
        local newControl = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        newControl.control = controlManager:CreateWindowForm(parentControl)
        setmetatable(newControl, self)
        self.__index = self

        OUTPUT("Make Window Instance")        
        
        return newControl
    end,

    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        OUTPUT("Get Window Instance")

        return newControl
    end,

    SetBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end
            
            color.R = color.R or 0
            color.G = color.G or 0
            color.B = color.B or 0
            self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))

            OUTPUT("Set Background Color #" .. string.format('%02x', color.R) .. string.format('%02x', color.G) .. string.format('%02x', color.B) )
        else
            self.control:SetBackgroundColor(gameManager:Color(0, 0, 0))

            OUTPUT("Set Background Color #000")
        end
    end,

    SetActiveEvent = function(self, event)
        self.control:SetActiveEvent(event)

        OUTPUT("Set Active Event")
    end,

    SetCloseEvent = function(self, event)
        self.control:SetCloseEvent(event)

        OUTPUT("Set Active Event" )
    end,

    SetSizeEvent = function(self, event)
        self.control:SetSizeEvent(event)

        OUTPUT("Set Size Event" )
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

    DeleteLayout = function(self, layout)
        if layout ~= nil then
            self.control:DeleteLayout(layout.control)
        end
    end,

    Close = function(self)
        self.control:Close()
    end
}