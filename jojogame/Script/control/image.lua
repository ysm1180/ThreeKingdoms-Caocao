require "control.lua"

-- @class Image : 게임 내의 이미지를 Wrapping 한 클래스
-- @inherit ControlObject
Image = ControlObject:Instance 
{
    DEFAULT_MASK_COLOR = {
        R = 0xFF,
        G = 0x00,
        B = 0xF7,
    },
    
    -- @description 이미지를 새로 생성합니다.
    -- @return Image : 새로 생성된 Image class
    New = function(self)
        local newControl = {}

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateImage()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 이미지를 Me5 파일에서 불러옵니다.
    -- @param options ImageLoadOptions : 이미지 불러오기 옵션
    -- @type ImageLoadOptions 
    --[[
        {
            FilePath string : 불러올 me5 파일 경로
            MaskColor number | MaskColor table : 투명화할 색 {
                R int : Red 값
                G int : Green 값
                B int : Blue 값
            }
            Brightness double : 밝기
            Group table : 불러올 이미지 위치 {
                Main int | Main string : 그룹 index or 이름
                Sub int | Sub string : 아이템 index or 이름
            }
        }
    ]]
    LoadFromMe5 = function(self, options)
        if options.FilePath ~= nil and options.Group ~= nil then
            if type(options.MaskColor) == "number" then
                options.MaskColor = {
                    R = options.MaskColor % 0x100,
                    G = math.floor(options.MaskColor / 0x100) % 0x100,
                    B = math.floor(options.MaskColor / 0x10000) % 0x100
                }
            end
            if options.MaskColor == nil then
                options.MaskColor = {
                    R = nil, G = nil, B = nil,
                }
            end

            if options.Brightness == nil then
                options.Brightness = 1.0
            end

            options.MaskColor.R = options.MaskColor.R or self.DEFAULT_MASK_COLOR.R
            options.MaskColor.G = options.MaskColor.G or self.DEFAULT_MASK_COLOR.G
            options.MaskColor.B = options.MaskColor.B or self.DEFAULT_MASK_COLOR.B

            local file = gameManager:OpenFile(options.FilePath)
            if type(options.Group.Main) == "string" then
                local groupIndex = file:FindGroupIndexByName(options.Group.Main)
                if groupIndex == -1 then
                    return
                end
                options.Group.Main = groupIndex
            end

            if type(options.Group.Sub) == "string" then
                local itemIndex = file:FindItemIndexByName(options.Group.Main, opions.Group.Sub)
                if itemIndex == -1 then
                    return
                end
                options.Group.Sub = itemIndex - file:GetGroupStartItemIndex(options.Group.Main)
            end
            gameManager:CloseFile(file)

            self.control:LoadImageFromMe5FileByIndex(
                options.FilePath,
                options.Group.Main,
                options.Group.Sub,
                gameManager:Color(options.MaskColor.R, options.MaskColor.G, options.MaskColor.B),
                options.Brightness
            )
        end
    end,

    Width = function(self)
        return self.control:GetWidth()
    end,

    Height = function(self)
        return self.control:GetHeight()
    end,
}
