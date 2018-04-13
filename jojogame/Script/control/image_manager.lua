require "image.lua"

ImageManager = Object:Instance
{
    CreateImage = function(self, options)
        local newControl = nil

        if options ~= nil then
            newControl = Image:New()

            newControl:LoadFromMe5({
                FilePath = options.FilePath,
                Group = {
                    Main = options.Group.Main,
                    Sub = options.Group.Sub,
                },
                MaskColor = options.MaskColor,
            })
        end

        return newControl
    end,
}