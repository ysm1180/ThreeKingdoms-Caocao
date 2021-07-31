require "audio_player.lua"

---@class AudioPlayerManager @AudioPlayer object를 관리하는 Manager class
AudioPlayerManager = {}

---오디오 파일로부터 AudioPlayer object 생성
---@param filePath string @오디오 파일 경로
---@return AudioPlayer
function AudioPlayerManager:LoadFromFile(filePath)
    local newControl = nil

    newControl = AudioPlayer:New()
    newControl:LoadFromFile(filePath)

    return newControl
end

---ME5 파일로부터 AudioPlayer object 생성
---@param filePath string @ME5 파일 경로
---@param group Me5Item @불러올 그룹 속성
---@return AudioPlayer
function AudioPlayerManager:LoadFromMe5File(filePath, item)
    local newControl = nil

    newControl = AudioPlayer:New()
    newControl:LoadFromMe5File(filePath, item)

    return newControl
end
