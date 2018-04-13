require "../base/object.lua"

ControlManager = Object:Instance 
{
    GetCenterPosition = function(self, parentControl, w, h, isChiildControl)
        if w == nil or h == nil then
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
        else
            parentWidth = gameManager:GetDesktopWidth()
            parentHeight = gameManager:GetDesktopHeight()
            parentX, parentY = 0, 0
        end

        local x = parentWidth / 2 - w / 2 + parentX
        local y = parentHeight / 2 - h / 2  + parentY
        
        return x, y
    end
}
