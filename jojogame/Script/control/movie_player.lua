require "control.lua"

-- @class MoviePlayer : 게임 내의 동영상 재생 컨트롤을 Wrapping 하는 클래스
-- @inherit ControlObject
MoviePlayer = ControlObject:Instance
{
    -- @description 동영상을 새로 생성합니다.
    -- @param parent Window : 동영상이 재생될 부모 Window class
    -- @param fileName string : 재생될 동영상 파일 경로
    -- @return MoviePlayer : 새로 생성된 MoviePlayer class
    New = function(self, parent, fileName)
        local newControl = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        if fileName == nil then
            fileName = ""
        end

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateMoviePlayer(parentControl, fileName)
        setmetatable(newControl, self)
        self.__index = self
        
        return newControl
    end,

    -- @description 생성
    Create = function(self)
        self.control:Create()
    end,
    
    -- @description 파괴
    Destroy = function(self)
        self.control:Destroy()
    end,

    -- @description 재생
    Play = function(self)
        self.control:Play()
    end,

    -- @description 멈춤
    Stop = function(self)
        self.control:Stop()
    end,

    -- @description 재생 중인지 반환
    -- @return boolean : 재생 중이라면 true
    IsPlaying = function(self)
        return self.control:IsPlaying()
    end,
    
    -- @description 크기 반환
    -- @return int, int : Width, Height
    Size = function(self)
        return self.control:GetWidth(), self.control:GetHeight()
    end,

    -- @description 재생 종료 이벤트
    -- @param event function : 재생 종료 이벤트
    SetEndEvent = function(self, event)
        self.control:SetEndEvent(event)
    end,

    -- @description 위치 이동
    -- @param x int : X 좌표
    -- @param y int : Y 좌표
    Move = function(self, x, y)
        if x ~= nil then
            self.control:SetX(x)
        end

        if y ~= nil then
            self.control:SetY(y)
        end
    end,
    
}