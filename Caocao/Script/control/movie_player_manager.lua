require "movie_player.lua"

---@class MoviePlayerEvent
---@field End function @동영상 끝날 때 실행되는 함수

---@class MoviePlayerCreateOptions
---@field Parent Window @동영상이 재생될 Window class
---@field FileName string @동영상 파일 경로
---@field Center boolean : 가운데 위치 여부
---@field X integer : X 위치
---@field Y integer : Y 위치

---@class MoviePlayerMananger @MoviePlayer object를 관리하는 Manager class
MoviePlayerManager = {}

---@param options MoviePlayerCreateOptions
---@return MoviePlayer
function MoviePlayerManager:Create(options)
    local newControl = nil

    if options ~= nil then
        newControl = MoviePlayer:New(options.Parent, options.FileName)

        -- 이벤트 설정
        if options.Event then
            newControl:SetEndEvent(options.Event.End)
        end

        -- 위치 설정
        newControl:Move(options.X, options.Y)

        -- 생성
        newControl:Create()

        -- 가운데
        if options.Center then
            local movieWidth, movieHeight = newControl:Size()
            if options.Parent then
                local parentWidth, parentHeight = options.Parent:Size()
                newControl:Move(parentWidth / 2 - movieWidth / 2, parentHeight / 2 - movieHeight / 2)
            end
        end
    end

    return newControl
end
