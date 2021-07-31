require "image.lua"

---@class ImageCreateOptions
---@field FilePath string @이미지를 불러올 me5 파일 경로
---@field Image Me5Item
---@field MaskColor integer | RGB @투명 색이 될 색
---@field Brightness number @밝기, default : 1
---@field IsLoadMirror boolean

---Image object를 관리하는 Manager class
---@class ImageManager
ImageManager = {}

---@param options ImageCreateOptions
---@return Image 
function ImageManager:CreateImage(options)
    local newControl = nil

    if options ~= nil then
        newControl = Image:New()

        -- Me5 파일에서 이미지 불러오기
        newControl:LoadFromMe5({
            FilePath = options.FilePath,
            Image = {
                Group = options.Image.Group,
                Item = options.Image.Item
            },
            MaskColor = options.MaskColor,
            Brightness = options.Brightness,
            Mirror = options.IsLoadMirror
        })
    end

    return newControl
end
