require "Script\\control_manager.lua"
require "Script\\button.lua"

ButtonManager = ControlManager:Instance
{
    Create = function(options)
        OUTPUT("-------- Start Create : Button --------")
        local newButton = Button:Instance(options.Parent)

        newButton:SetSize(options.Width, options.Height)

        if options.Center then
            local x, y = ControlManager.GetCenterPosition(options.Parent, options.Width, options.Height, false)
            if x ~= nil and y ~= nil then
                options.X = x
                options.Y = y
            end
        end
        newButton:Move(options.X, options.Y)

        newButton:SetText(options.Text)

        newButton:SetCreateEvent(options.Create)
        newButton:SetDestroyEvent(options.Destroy)     
        newButton:SetMouseLButtonUpEvent(options.MouseLButtonUp)
        newButton:SetMouseLButtonDownEvent(options.MouseLButtonDown)
    
        if options.Transparent then
            newButton:SetTransparentBackground(options.Transparent.Background)
            newButton:SetTransparentBorder(options.Transparent.Border)
        end

        newButton:Create()

        if options.Show then
            newButton:Show()
        end

        OUTPUT("-------- End Create : Button --------")
        return newButton
    end
}