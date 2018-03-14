require "Script\\control.lua"

Toolbar = ControlObject:Instance
{
    New = function(self)
        local newToolbar = {}

        newToolbar.control = controlManager:CreateToolbar()
        setmetatable(newToolbar, self)
        self.__index = self

        OUTPUT("Make Toolbar Instance")        
        
        return newToolbar
    end,

    Height = function(self)
        return self.control:GetHeight()
    end,

    AddButton = function(self, button)
        if button ~= nil then
            self.control:AddButton(button.control)
            OUTPUT("Add Toolbar button")
        end
    end,

    Create = function(self, parentWindow, imageWidth, imageHeight)
        if parentWindow ~= nil then
            self.control:Create(parentWindow.control, imageWidth, imageHeight)
        end
    end,

    Show = function(self)
        self.control:Show()
    end,

    Hide = function(self)
        self.control:Hide()
    end,
}

ToolbarButton = ControlObject:Instance
{
    New = function(self)
        local newToolbarButton = {}

        newToolbarButton.control = controlManager:CreateToolbarButton()
        setmetatable(newToolbarButton, self)
        self.__index = self

        OUTPUT("Make Toolbar Button Instance")        
        
        return newToolbarButton
    end,

    Text = function(self)
        return self.control:GetText()
    end,

    Enable = function(self)
        self.control:SetEnabled(true)
    end,

    Disable = function(self)
        self.control:SetEnabled(false)
    end,
    
    SetImage = function(self, image)
        if image ~= nil then
            self.control:SetImage(image.control)

            OUTPUT("Set Image")
        else
            self.control:SetImage(nil)
        end
    end,

    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)

            OUTPUT("Set Text : " .. text)
        end
    end,

    SetClickEvent = function(self, event)
        if event ~= nil then
            self.control:SetClickEvent(event)

            OUTPUT("Set Click Event : " .. event)
        end
    end,

    SetTooltipText = function(self, tooltip)
        if tooltip ~= nil then
            self.control:SetTooltipText(tooltip)

            OUTPUT("Set Tooltip Text : " .. tooltip)
        end
    end,

    Create = function(self)
        self.control:Create()
    end,
}