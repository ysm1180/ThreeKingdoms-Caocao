require "Script\\object.lua"

ControlManager = Object:Instance 
{
    GetCenterPosition = function(self, parentControl, w, h, isChiildControl)
        OUTPUT("--- Start Get Center Position ---")

        if w == nil or h == nil then
            OUTPUT("Return Null")
            
            return nil, nil
        end

        if isChiildControl == nil then
            isChiildControl = true
        end

        if parentControl ~= nil then
            parentWidth, parentHeight = parentControl:Size() 
            if isChiildControl then
                parentX, parentY = 0, 0
            else
                parentX, parentY = parentControl:Position()
            end

            OUTPUT("Get Parent Size (" .. parentWidth .. ", " .. parentHeight .. ")")       
            OUTPUT("Get Parent Position (" .. parentX .. ", " .. parentY .. ")")                             
        else
            parentWidth = gameManager:GetDesktopWidth()
            parentHeight = gameManager:GetDesktopHeight()
            parentX, parentY = 0, 0

            OUTPUT("Get Desktop Size (" .. parentWidth .. ", " .. parentHeight .. ")")
        end

        local x = parentWidth / 2 - w / 2 + parentX
        local y = parentHeight / 2 - h / 2  + parentY
        
        OUTPUT("--- Return Get Center Position (" .. x .. ", " .. y .. ") ---")
        return x, y
    end
}
