require "control.lua"

Layout = ControlObject:Instance
{
    New = function(self)
        local newControl = {}

        newControl.control = controlManager:CreateLayout()
        setmetatable(newControl, self)
        self.__index = self
        
        return newControl
    end,
    
    Move = function(self, x, y, isRedraw)
        if isRedraw == nil then
            isRedraw = true
        end

        if x ~= nil then
            self.control:SetX(x, isRedraw)
        end
        if y ~= nil then
            self.control:SetY(y)
        end

    end,

    SetWidth = function(self, width, isRedraw)
        if width ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetWidth(width)
        end
    end,

    SetHeight = function(self, height, isRedraw)
        if height ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetHeight(height)
        end
    end,

    SetRatioX = function(self, ratio, isRedraw)
        if ratio ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetRatioX(ratio)
        end
    end,

    SetRatioY = function(self, ratio, isRedraw)
        if ratio ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetRatioY(ratio)
        end
    end,

    AddImage = function(self, image, x, y, isUpdate)
        if image ~= nil and x ~= nil and y ~= nil then
            if isUpdate == nil then
                isUpdate = true
            end
            return self.control:AddImage(image.control, x, y, isUpdate)
        end
        
        return -1
    end,
    
    DeleteImage = function(self, image, isUpdate)
        if image ~= nil then
            if isUpdate == nil then
                isUpdate = true
            end
            self.control:DeleteImage(image.control, isUpdate)

        end
    end,
}