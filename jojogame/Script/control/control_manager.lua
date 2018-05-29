require "../base/object.lua"

-- @class ControlManager
-- @description 공통 Control 을 관리하는 Manager class, 직접적으로 사용할 일은 없는 클래스
-- @inherit Object
ControlManager = Object:Instance 
{
    -- @description 정 가운데로 이동할 위치를 반환한다.
    -- @param parantControl class : 기준 위치를 지정할 class
    -- @param w int : 현재 컨트롤의 너비
    -- @param h int : 현재 컨트롤의 높이
    -- @param isChildControl boolean : 자식 class 인지 여부
    -- @return int, int : X, Y 위치
    GetCenterPosition = function(self, parentControl, w, h, isChiildControl)
        if w == nil or h == nil then
            return nil, nil
        end

        -- @description isChildControl Dafault : true
        if isChiildControl == nil then
            isChiildControl = true
        end

        -- @description parentControl 지정 안했을 시 데스크탑 크기가 기준이 됨
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
