require "text_control.lua"

---@class ChildControl @게임 내의 공통 Child Control 클래스를 Wrapping 한 클래스
ChildControl = Class(Control)

---Y 좌표의 경우 Parent Window의 Toolbar로 인해 좌표가 밀리는 경우가 생겨
---내부적으로 해당 상황에 대해 처리해주는 SetLuaY 함수를 사용한다.  
---@param x integer
---@param y integer
function ChildControl:Move(x, y)
    if x ~= nil then
        self.control:SetX(x)
    end
    if y ~= nil then
        self.control:SetLuaY(y)
    end
end

-- @description 컨트롤의 Y 위치를 반환한다.
-- @return int : Y 위치
function ChildControl:Y()
    return self.control:GetLuaY()
end
