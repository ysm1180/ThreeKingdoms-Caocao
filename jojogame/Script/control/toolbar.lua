require "control.lua"

Toolbar = ControlObject:Instance
{
    New = function(self)
        local newControl = {}

        newControl.control = controlManager:CreateToolbar()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    Height = function(self)
        return self.control:GetHeight()
    end,

    AddButton = function(self, button)
        if button ~= nil then
            self.control:AddButton(button.control)
        end
    end,

    AutoSize = function(self)
        self.control:AutoSize()
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
        local newControlButton = {}

        newControlButton.control = controlManager:CreateToolbarButton()
        setmetatable(newControlButton, self)
        self.__index = self

        return newControlButton
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
        else
            self.control:SetImage(nil)
        end
    end,

    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)
        end
    end,

    SetClickEvent = function(self, event)
        if event ~= nil then
            self.control:SetClickEvent(event)
        end
    end,

    SetTooltipText = function(self, tooltip)
        if tooltip ~= nil then
            self.control:SetTooltipText(tooltip)
        end
    end,

    Create = function(self)
        self.control:Create()
    end,
}