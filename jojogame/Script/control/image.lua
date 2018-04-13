require "control.lua"

Image = ControlObject:Instance 
{
    DEFAULT_MASK_COLOR = {
        R = 0xFF,
        G = 0x00,
        B = 0xF7,
    },
    
    New = function(self)
        local newControl = {}

        newControl.control = controlManager:CreateImage()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
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
