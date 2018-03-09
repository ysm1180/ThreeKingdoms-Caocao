require "Script\\control.lua"

Layout = ControlObject:Instance
{
    Instance = function(self)
        local newLayout = {}

        newLayout.control = controlManager:CreateLayout()
        setmetatable(newLayout, self)
        self.__index = self

        OUTPUT("Make Layout Instance")        
        
        return newLayout
    end,
    
    SetWidth = function(self, width)
        if width ~= nil then
            self.control:SetWidth(width)
        end
    end,

    SetHeight = function(self, height)
        if height ~= nil then
            self.control:SetHeight(height)
        end
    end,

    SetRatioX = function(self, ratio)
        if ratio ~= nil then
            self.control:SetRatioX(ratio)
        end
    end,

    SetRatioY = function(self, ratio)
        if ratio ~= nil then
            self.control:SetRatioY(ratio)
        end
    end,

    AddImage = function(self, image, x, y)
        if image ~= nil then
            OUTPUT("Layout Add Image")
            return self.control:AddImage(image.control, x, y)
        end
        
        OUTPUT("Fail to add image")
        return -1
    end,
    
    DeleteImage = function(self, image)
        if image ~= nil then
            OUTPUT("LAyout Delete Image")
            self.control:DeleteImage(image.control)
        end
    end,
}