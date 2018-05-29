require "layout.lua"

-- @class LayoutMananger
-- @description Layout class 를 관리하는 Manager class
-- @inherit Object
LayoutManager = Object:Instance
{
    -- @param options LayoutOptions : control 생성 옵션
    -- @return Layout : 생성된 Layout class
    -- @type LayoutOptions table
    --[[
        {
            Width int : control 너비
            Height int : control 높이
            X int : X 위치
            Y int : Y 위치
            Ratio table : {
                X double : 가로 비율, default: 1.0
                Y double : 세로 비율, default: 1.0
            }
        }
    ]]
    CreateLayout = function(self, options)
        local newControl = nil

        if options ~= nil then
            newControl = Layout:New()

            -- @description 위치 설정
            newControl:Move(options.X, options.Y)
            
            -- @description 크기 설정
            newControl:SetWidth(options.Width)
            newControl:SetHeight(options.Height)

            -- @description 비율 설정
            if options.Ratio ~= nil then
                newControl:SetRatioX(options.Ratio.X)
                newControl:SetRatioY(options.Ratio.Y)
            end
        end
        return newControl
    end,
}