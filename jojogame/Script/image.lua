require "Script\\control.lua"

Image = ControlObject:Instance 
{
    DEFAULT_MASK_COLOR = {
        R = 0xFF,
        G = 0x00,
        B = 0xF7,
    },
    Instance = function(self)
        local newImage = {}

        newImage.control = controlManager:CreateImage()
        setmetatable(newImage, self)
        self.__index = self

        OUTPUT("Make Image Instance")

        return newImage
    end,

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
            options.MaskColor.R = options.MaskColor.R or self.DEFAULT_MASK_COLOR.R
            options.MaskColor.G = options.MaskColor.G or self.DEFAULT_MASK_COLOR.G
            options.MaskColor.B = options.MaskColor.B or self.DEFAULT_MASK_COLOR.B
            self.control:LoadImageFromMe5File(
                options.FilePath,
                options.Group.Main,
                options.Group.Sub,
                gameManager:Color(options.MaskColor.R, options.MaskColor.G, options.MaskColor.B)
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
