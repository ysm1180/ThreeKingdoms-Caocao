require "../base/object.lua"
require "../type/control.lua"

---@class Control : ControlObject @게임 내의 공통 Control 클래스를 Wrapping 한 클래스
Control = Class(ControlObject)

---컨트롤 생성
function Control:Create()
    return self.control:Create()
end

---@param control any
---@return Control
function Control:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---컨트롤 파괴
function Control:Destroy()
    self.control:Destroy()
end

---컨트롤 위치를 이동한다.
---@param x integer @x 위치
---@param y integer @y 위치
function Control:Move(x, y)
    if x ~= nil then
        self.control:SetX(x)
    end
    if y ~= nil then
        self.control:SetY(y)
    end
end

--- 컨트롤 사이즈를 설정한다.
---@param width integer @width
---@param height integer @height
function Control:SetSize(width, height)
    if width ~= nil then
        self.control:SetWidth(width)
    end
    if height ~= nil then
        self.control:SetHeight(height)
    end
end

---컨트롤을 나타낸다.
function Control:Show()
    self.control:Show()
end

---컨트롤을 숨긴다.
function Control:Hide()
    self.control:Hide()
end

---컨트롤의 X 위치를 반환한다.
---@return integer
function Control:X()
    return self.control:GetX()
end

---컨트롤의 Y 좌표를 반환한다.
---@return integer
function Control:Y()
    return self.control:GetY()
end

---컨트롤의 위치를 반환한다.
---@return integer, integer @X, Y 위치
function Control:Position()
    return self.control:GetX(), self.control:GetY()
end

---컨트롤의 너비를 반환한다.
---@return integer @width
function Control:Width()
    return self.control:GetWidth()
end

---컨트롤의 높이를 반환한다.
---@return integer @height
function Control:Height()
    return self.control:GetHeight()
end

---컨트롤의 크기를 반환한다.
---@return integer, integer @width, height
function Control:Size()
    return self.control:GetWidth(), self.control:GetHeight()
end

---컨트롤의 데이터를 반환한다.
---@return string @user data
function Control:UserData()
    return self.control:GetUserData()
end

---컨트롤의 생성 이벤트를 설정한다.
---@param event function @생성 때 실행되는 함수
function Control:SetCreateEvent(event)
    if event ~= nil then
        self.control:SetCreateEvent(event)
    end
end

---컨트롤의 파괴 이벤트를 설정한다.
---@param event function @파괴 때 실행되는 함수
function Control:SetDestroyEvent(event)
    if event ~= nil then
        self.control:SetDestroyEvent(event)
    end
end

---컨트롤의 Mouse Left Button Up 이벤트를 설정한다.
---@param event function @마우스 왼쪽 버튼을 눌렀다 땠을 때 실행되는 함수
function Control:SetMouseLButtonUpEvent(event)
    if event ~= nil then
        self.control:SetMouseLButtonUpEvent(event)
    end
end

---컨트롤의 Mouse Left Button Down 이벤트를 설정한다.
---@param event function @마우스 왼쪽 버튼을 눌렀을 때 실행되는 함수
function Control:SetMouseLButtonDownEvent(event)
    if event ~= nil then
        self.control:SetMouseLButtonDownEvent(event)
    end
end

---컨트롤의 Mouse Move 이벤트를 설정한다.
---@param event function @컨트롤 내에서 마우스를 움직일 때 실행되는 함수
function Control:SetMouseMoveEvent(event)
    if event ~= nil then
        self.control:SetMouseMoveEvent(event)
    end
end

---컨트롤의 Mouse Enter 이벤트를 설정한다.
---@param event function @컨트롤 내로 마우스를 이동했을 때 실행되는 함수
function Control:SetMouseEnterEvent(event)
    if event ~= nil then
        self.control:SetMouseEnterEvent(event)
    end
end

---컨트롤의 Mouse Leave 이벤트를 설정한다.
---@param event function @컨트롤 바깥으로 마우스를 이동했을 때 실행되는 함수
function Control:SetMouseLeaveEvent(event)
    if event ~= nil then
        self.control:SetMouseLeaveEvent(event)
    end
end

---컨트롤의 임의 데이터를 설정한다.
---@param data string @데이터
function Control:SetUserData(data)
    if data ~= nil then
        self.control:SetUserData(data)
    end
end

---컨트롤을 다시 그린다.
function Control:Refresh()
    self.control:Refresh()
end

---컨트롤의 일부 region 을 다시 그린다.
---@param rect Rect
function Control:RefreshRegion(rect)
    if rect ~= nil then
        self.control:RefreshRegion(rect.Left, rect.Top, rect.Right, rect.Bottom)
    end
end

