require "control.lua"

---@class MoviePlayer : ControlObject @게임 내의 동영상 재생 컨트롤을 Wrapping 하는 클래스
MoviePlayer = Class(ControlObject)

---동영상을 새로 생성합니다.
---@param parent Window @동영상이 재생될 부모 Window class
---@param fileName string @재생될 동영상 파일 경로
---@return MoviePlayer
function MoviePlayer:New(parent, fileName)
    local newControl = {}
    local parentControl = nil

    if parent ~= nil then
        parentControl = parent.control
    end
    if fileName == nil then
        fileName = ""
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateMoviePlayer(parentControl, fileName)
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---생성
function MoviePlayer:Create()
    self.control:Create()
end

---파괴
function MoviePlayer:Destroy()
    self.control:Destroy()
end

---재생
function MoviePlayer:Play()
    self.control:Play()
end

---멈춤
function MoviePlayer:Stop()
    self.control:Stop()
end

---재생 중인지 반환
---@return boolean @재생 중이라면 true
function MoviePlayer:IsPlaying()
    return self.control:IsPlaying()
end

---크기 반환
---@return integer, integer @Width, Height
function MoviePlayer:Size()
    return self.control:GetWidth(), self.control:GetHeight()
end

---재생 종료 이벤트
---@param event function
function MoviePlayer:SetEndEvent(event)
    self.control:SetEndEvent(event)
end

---위치 이동
---@param x number | nil
---@param y number | nil
function MoviePlayer:Move(x, y)
    if x ~= nil then
        self.control:SetX(x)
    end

    if y ~= nil then
        self.control:SetY(y)
    end
end
