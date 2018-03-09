require "Script\\layout.lua"

LayoutManager = Object:Instance
{
    CreateLayout = function(self, options)
        OUTPUT("-------- Start Create : Layout --------")

        local newLayout = nil

        if options ~= nil then
            newLayout = Layout:Instance()

            newLayout:SetWidth(options.Width)
            newLayout:SetHeight(options.Height)

            if options.Ratio ~= nil then
                newLayout:SetRatioX(options.Ratio.X)
                newLayout:SetRatioX(options.Ratio.Y)
            end
        end
        
        OUTPUT("-------- End Create : Layout --------")
        return newLayout
    end,
}