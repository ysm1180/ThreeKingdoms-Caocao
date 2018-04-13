require "layout.lua"

LayoutManager = Object:Instance
{
    CreateLayout = function(self, options)
        local newControl = nil

        if options ~= nil then
            newControl = Layout:New()

            newControl:Move(options.X, options.Y)
            
            newControl:SetWidth(options.Width)
            newControl:SetHeight(options.Height)

            if options.Ratio ~= nil then
                newControl:SetRatioX(options.Ratio.X)
                newControl:SetRatioY(options.Ratio.Y)
            end
        end
        return newControl
    end,
}