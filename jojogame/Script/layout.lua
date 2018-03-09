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
    
    Move = function(self, x, y)
        if x ~= nil then
            self.control:SetX(x)
        end
        if y ~= nil then
            self.control:SetY(y)
        end

        OUTPUT("Move (" .. (x or "NOT MOVE") .. ", " .. (y or "NOT MOVE") .. ")")        
    end,

    SetWidth = function(self, width)
        if width ~= nil then
            self.control:SetWidth(width)
            OUTPUT("Set layout width : " .. width)        
        end
    end,

    SetHeight = function(self, height)
        if height ~= nil then
            self.control:SetHeight(height)
            OUTPUT("Set layout height : " .. height)        
        end
    end,

    SetRatioX = function(self, ratio)
        if ratio ~= nil then
            self.control:SetRatioX(ratio)
            OUTPUT("Set layout ratio X: " .. ratio)        
        end
    end,

    SetRatioY = function(self, ratio)
        if ratio ~= nil then
            self.control:SetRatioY(ratio)
            OUTPUT("Set layout ratio Y: " .. ratio)        
        end
    end,

    AddImage = function(self, image, x, y)
        if image ~= nil and x ~= nil and y ~= nil then
            OUTPUT("Add layout image (" .. x .. ", " .. y .. ")")
            return self.control:AddImage(image.control, x, y)
        end
        
        OUTPUT("Fail to add image")
        return -1
    end,
    
    DeleteImage = function(self, image)
        if image ~= nil then
            OUTPUT("Delete layout image")
            self.control:DeleteImage(image.control)
        end
    end,
}