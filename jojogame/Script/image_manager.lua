require "Script\\image.lua"

ImageManager = Object:Instance
{
    CreateImage = function(self, options)
        OUTPUT("-------- Start Create : Image --------")

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

        OUTPUT("-------- End Create : Image --------")
        return newControl
    end,
}