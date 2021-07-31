require "control.lua"

---@class AudioPlayer : Control  @게임 내의 음악 재생 컨트롤을 Wrapping 하는 클래스
AudioPlayer = Class(Control)

---오디오 재생 Player를 새로 생성합니다.
---@return AudioPlayer @새로 생성된 AudioPlayer class
function AudioPlayer:New()
    local newControl = {}

    if fileName == nil then
        fileName = ""
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateAudioPlayer()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---파일로부터 오디오 불러오기
---@param filePath string @파일 경로
function AudioPlayer:LoadFromFile(filePath)
    self.control:LoadFromFile(filePath)
end

---ME5 파일로부터 오디오 불러오기
---@param filePath string ME5 파일 경로
---@param item Me5Item 불러올 오디오 아이템
function AudioPlayer:LoadFromMe5File(filePath, item)
    local file = gameManager:OpenFile(filePath)
    if type(item.Group) == "string" then
        local groupIndex = file:FindGroupIndexByName(item.Group)
        if groupIndex == -1 then
            return
        end
        item.Group = groupIndex
    end

    if type(item.Item) == "string" then
        local itemIndex = file:FindItemIndexByName(item.Group, item.Item)
        if itemIndex == -1 then
            return
        end
        item.Item = itemIndex - file:GetGroupStartItemIndex(item.Group)
    end
    gameManager:CloseFile(file)

    self.control:LoadFromMe5File(filePath, item.Group, item.Item)
end

---파괴
function AudioPlayer:Destroy()
    self.control:Destroy()
end

---오디오 재생
---@param count integer @플레이 횟수, 0 이면 무한 반복
function AudioPlayer:Play(count)
    if count == nil then
        count = 1
    end
    self.control:Play(count)
end

---오디오 재생 멈춤
function AudioPlayer:Stop()
    self.control:Stop()
end

---오디오 재생 중인지 확인
---@return boolean @재생 중이라면 true
function AudioPlayer:IsPlaying()
    return self.control:IsPlaying()
end
