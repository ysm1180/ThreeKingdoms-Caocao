require "Script\\control.lua"

Layout = ControlObject:Instance
{
    New = function(self)
        local newLayout = {}

        newLayout.control = controlManager:CreateLayout()
        setmetatable(newLayout, self)
        self.__index = self

        OUTPUT("Make Layout Instance")        
        
        return newLayout
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

        OUTPUT("Move (" .. (x or "NOT MOVE") .. ", " .. (y or "NOT MOVE") .. ")")        
    end,

    SetWidth = function(self, width, isRedraw)
        if width ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetWidth(width)
            OUTPUT("Set layout width : " .. width)        
        end
    end,

    SetHeight = function(self, height, isRedraw)
        if height ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetHeight(height)
            OUTPUT("Set layout height : " .. height)        
        end
    end,

    SetRatioX = function(self, ratio, isRedraw)
        if ratio ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetRatioX(ratio)
            OUTPUT("Set layout ratio X: " .. ratio)        
        end
    end,

    SetRatioY = function(self, ratio, isRedraw)
        if ratio ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetRatioY(ratio)
            OUTPUT("Set layout ratio Y: " .. ratio)        
        end
    end,

    AddImage = function(self, image, x, y, isUpdate)
        if image ~= nil and x ~= nil and y ~= nil then
            OUTPUT("Add layout image (" .. x .. ", " .. y .. ")")
            if isUpdate == nil then
                isUpdate = true
            end
            return self.control:AddImage(image.control, x, y, isUpdate)
        end
        
        OUTPUT("Fail to add image")
        return -1
    end,
    
    DeleteImage = function(self, image, isUpdate)
        if image ~= nil then
            OUTPUT("Delete layout image")
            if isUpdate == nil then
                isUpdate = true
            end
            self.control:DeleteImage(image.control, isUpdate)

        end
    end,
}