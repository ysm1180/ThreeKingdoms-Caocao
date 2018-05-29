require "control_manager.lua"
require "movie_player.lua"

-- @class MoviePlayerMananger
-- @description MoviePlayer class 를 관리하는 Manager class
-- @inherit ControlManager
MoviePlayerManager = ControlManager:Instance
{
    -- @param options MoviePlayerOptions : control 생성 옵션
    -- @return MoviePlayer : 생성된 MoviePlayer class
    -- @type MovePlayerOptions table
    --[[
        {
            Parent Window : 동영상이 재생될 Window class
            FileName string : 동영상 파일 경로
            Event table : {
                End function : 동영상 끝날 때 실행되는 함수
            }
            Center boolean : 가운데 위치 여부
            X int : X 위치
            Y int : Y 위치
        }
    ]]
    Create = function(self, options)
        local newControl = nil
        
        if options ~= nil then
            newControl = MoviePlayer:New(options.Parent, options.FileName)

            -- @description 이벤트 설정
            if options.Event then
                newControl:SetEndEvent(options.Event.End)
            end

            -- @description 위치 설정
            newControl:Move(options.X, options.Y)

            -- @description 생성
            newControl:Create()

            -- @description 가운데
            if options.Center then
                local movieWidth, movieHeight = newControl:Size()
                if options.Parent then
                    local parentWidth, parentHeight = options.Parent:Size()
                    newControl:Move(parentWidth / 2 - movieWidth / 2, parentHeight / 2 - movieHeight / 2)
                end
            end
        end

        return newControl
    end,
}