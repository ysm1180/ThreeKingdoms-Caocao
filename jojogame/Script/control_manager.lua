require "Script\\object.lua"

ControlManager = Object:Instance 
{
    GetCenterPosition = function(parentControl, w, h)
        if w == nil or h == nil then
            return nil, nil
        end

        if parentControl ~= nil then
            parentWidth, parentHeight = parentControl:Size() 
        else
            parentWidth = gameManager:GetDesktopWidth()
            parentHeight = gameManager:GetDesktopHeight()
        end

        local x = parentWidth / 2 - w / 2
        local y = parentHeight / 2 - h / 2 
    
        return x, y
    end
}
