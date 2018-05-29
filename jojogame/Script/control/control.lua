require "../base/object.lua"

-- @class ControlObject
-- @inherit Object
ControlObject = Object:Instance
{
    control = nil,
}

-- @class Control : 게임 내의 공통 Control 클래스를 Wrapping 한 클래스
-- @inherit ControlObject
Control = ControlObject:Instance
{
    -- @description 컨트롤 생성
    Create = function(self)
        result = self.control:Create()
        return result
    end,

    -- @description 컨트롤 파괴
    Destroy = function(self)
        self.control:Destroy()
    end,

    -- @description 컨트롤 위치를 이동한다.
    -- @param x int : x 위치
    -- @param y int : y 위치
    Move = function(self, x, y)
        if x ~= nil then
            self.control:SetX(x)
        end
        if y ~= nil then
            self.control:SetY(y)
        end
    end,

    -- @description 컨트롤 사이즈를 설정한다.
    -- @param w int : width
    -- @param h int : height
    SetSize = function(self, w, h)
        if w ~= nil then
            self.control:SetWidth(w)
        end
        if h ~= nil then
            self.control:SetHeight(h)
        end
    end,
    
    -- @description 컨트롤을 나타낸다.
    Show = function(self)
        self.control:Show()
    end,

    -- @description 컨트롤을 숨긴다.
    Hide = function(self)
        self.control:Hide()
    end,

    -- @description 컨트롤의 위치를 반환한다.
    -- @return int, int : X, Y 위치
    Position = function(self)
        return self.control:GetX(), self.control:GetY()
    end,

    -- @description 컨트롤의 너비를 반환한다.
    -- @return int : width
    Width = function(self)
        return self.control:GetWidth()
    end,

    -- @description 컨트롤의 높이를 반환한다.
    -- @return int : height
    Height = function(self)
        return self.control:GetHeight()
    end,

    -- @description 컨트롤의 크기를 반환한다.
    -- @return int, int : width, height
    Size = function(self)
        return self.control:GetWidth(), self.control:GetHeight()
    end,

    -- @description 컨트롤의 데이터를 반환한다.
    -- @return string : user data
    UserData = function(self)
        return self.control:GetUserData()
    end,

    -- @description 컨트롤의 생성 이벤트를 설정한다.
    -- @param event function : 생성 때 실행되는 함수
    SetCreateEvent = function(self, event)
        if event ~= nil then
            self.control:SetCreateEvent(event)
        end
    end,

    -- @description 컨트롤의 파괴 이벤트를 설정한다.
    -- @param event function : 파괴 때 실행되는 함수
    SetDestroyEvent = function(self, event)
        if event ~= nil then            
            self.control:SetDestroyEvent(event)
        end
    end,

    -- @description 컨트롤의 Mouse Left Button Up 이벤트를 설정한다.
    -- @param event function : 마우스 왼쪽 버튼을 눌렀다 땠을 때 실행되는 함수
    SetMouseLButtonUpEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseLButtonUpEvent(event)
        end
    end,

    -- @description 컨트롤의 Mouse Left Button Down 이벤트를 설정한다.
    -- @param event function : 마우스 왼쪽 버튼을 눌렀을 때 실행되는 함수
    SetMouseLButtonDownEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseLButtonDownEvent(event)
        end
    end,

    -- @description 컨트롤의 Mouse Move 이벤트를 설정한다.
    -- @param event function : 컨트롤 내에서 마우스를 움직일 때 실행되는 함수
    SetMouseMoveEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseMoveEvent(event)
        end
    end,

    -- @description 컨트롤의 Mouse Enter 이벤트를 설정한다.
    -- @param event function : 컨트롤 내로 마우스를 이동했을 때 실행되는 함수
    SetMouseEnterEvent = function(self, event)
        if event ~= nil then        
            self.control:SetMouseEnterEvent(event)
        end
    end,

    -- @description 컨트롤의 Mouse Leave 이벤트를 설정한다.
    -- @param event function : 컨트롤 바깥으로 마우스를 이동했을 때 실행되는 함수
    SetMouseLeaveEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseLeaveEvent(event)
        end
    end,

    -- @description 컨트롤의 임의 데이터를 설정한다.
    -- @param data string : 데이터
    SetUserData = function(self, data)
        if data ~= nil then
            self.control:SetUserData(data)
        end
    end,

    -- @description 컨트롤을 다시 그린다.
    Refresh = function(self)
        self.control:Refresh()
    end,

    -- @description 컨트롤의 일부 region 을 다시 그린다.
    -- @param rect table :
    --[[
        {
            Left int : 왼쪽 위 X 위치
            Top int : 왼쪽 위 Y 위치
            Right int : 오른쪽 아래 X 위치
            Bottom int : 오른쪽 아래 Y 위치
        }
    ]]
    RefreshRegion = function(self, rect)
        if rect ~= nil then
            self.control:RefreshRegion(rect.Left, rect.Top, rect.Right, rect.Bottom)
        end
    end,
}