require "image.lua"

-- @class ImageMananger
-- @description Image class 를 관리하는 Manager class
-- @inherit Object
ImageManager = Object:Instance
{
    -- @param options ImageOptions : control 생성 옵션
    -- @return Image : 생성된 Image class
    -- @type ImageOptions table
    --[[
        {
            FilePath string : 이미지를 불러올 me5 파일 경로
            Group table : {
                Main int | Main string : index 또는 이름으로 그룹 지정
                Sub int | Sub string : index 또는 이름으로 아이템 지정
            }
            MaskColor int : 투명 색이 될 색
            Brightness double : 밝기, default : 1
        }
    ]]
    CreateImage = function(self, options)
        local newControl = nil

        if options ~= nil then
            newControl = Image:New()

            -- @description Me5 파일에서 이미지 불러오기
            newControl:LoadFromMe5({
                FilePath = options.FilePath,
                Group = {
                    Main = options.Group.Main,
                    Sub = options.Group.Sub,
                },
                MaskColor = options.MaskColor,
                Brightness = options.Brightness,
            })
        end

        return newControl
    end,
}