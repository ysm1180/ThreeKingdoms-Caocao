require "Script\\control_manager.lua"
require "Script\\button.lua"

ButtonManager = ControlManager:Instance 
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Button --------")
        local newButton = Button:New(options.Parent)

        newButton:SetSize(options.Width, options.Height)

        if options.Center then
            local x, y = ControlManager.GetCenterPosition(options.Parent, options.Width, options.Height, false)
            if x ~= nil and y ~= nil then
                options.X = x
                options.Y = y
            end
        end
        newButton:Move(options.X, options.Y)

        newButton:SetCreateEvent(options.Create)
        newButton:SetDestroyEvent(options.Destroy)
        newButton:SetMouseLButtonUpEvent(options.MouseLButtonUp)
        newButton:SetMouseLButtonDownEvent(options.MouseLButtonDown)

        if options.Text then    
            if options.Text.Font then
                newButton:SetFontName(options.Text.Font.Name)
                newButton:SetFontSize(options.Text.Font.Size)
            end
            if options.Text.Color then
                newButton:SetNormalTextColor(options.Text.Color.Normal)
                newButton:SetFocusedTextColor(options.Text.Color.Focused)
                newButton:SetPushedTextColor(options.Text.Color.Pushed)
            end
            newButton:SetText(options.Text.Content)
        end

        if options.Border then
            newButton:SetBorderWidth(options.Border.Width)
            if options.Border.Color then
                newButton:SetNormalBorderColor(options.Border.Color.Normal)
                newButton:SetFocusedBorderColor(options.Border.Color.Focused)
                newButton:SetPushedBorderColor(options.Border.Color.Pushed)
            end
        end

        if options.Background then
            if options.Background.Color then
                newButton:SetNormalBackgroundColor(options.Background.Color.Normal)
                newButton:SetFocusedBackgroundColor(options.Background.Color.Focused)
                newButton:SetPushedBackgroundColor(options.Background.Color.Pushed)
            end
            newButton:SetTransparentBackground(options.Background.Transparent)        
        end

        newButton:Create()

        if options.Show then
            newButton:Show()
        end

        OUTPUT("-------- End Create : Button --------")
        return newButton
    end
}
