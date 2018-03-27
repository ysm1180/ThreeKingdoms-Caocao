require "Script\\control_manager.lua"
require "Script\\button.lua"

ButtonManager = ControlManager:Instance 
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Button --------")

        local newButton = nil

        if options ~= nil then

            if options._Control ~= nil then
                return Button:Instance(options._Control)
            end
            
            newButton = Button:New(options.Parent)

            newButton:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newButton:Move(options.X, options.Y)

            if options.Event ~= nil then
                newButton:SetCreateEvent(options.Event.Create)
                newButton:SetDestroyEvent(options.Event.Destroy)
                newButton:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newButton:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newButton:SetMouseMoveEvent(options.Event.MouseMove)
                newButton:SetMouseEnterEvent(options.Event.MouseEnter)
                newButton:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            if options.Text ~= nil then    
                if options.Text.Font ~= nil then
                    newButton:SetFontName(options.Text.Font.Name)
                    newButton:SetFontSize(options.Text.Font.Size)
                    newButton:SetBold(options.Text.Font.Bold)
                    newButton:SetUnderline(options.Text.Font.Underline)
                    newButton:SetItalic(options.Text.Font.Italic)
                end
                if options.Text.Color ~= nil then
                    if options.Text.Color.R ~= nil and options.Text.Color.G ~= nil and options.Text.Color.B ~= nil then
                        newButton:SetNormalTextColor(options.Text.Color)                        
                        newButton:SetFocusedTextColor(options.Text.Color)
                        newButton:SetPushedTextColor(options.Text.Color)                    
                    else
                        newButton:SetNormalTextColor(options.Text.Color.Normal)
                        newButton:SetFocusedTextColor(options.Text.Color.Focused)
                        newButton:SetPushedTextColor(options.Text.Color.Pushed)
                    end
                end
                newButton:SetText(options.Text.Content)
            end

            if options.Border ~= nil then
                newButton:SetBorderWidth(options.Border.Width)
                if options.Border.Color ~= nil then
                    if options.Border.Color.R ~= nil and options.Border.Color.G ~= nil and options.Border.Color.B ~= nil then
                        newButton:SetNormalBorderColor(options.Border.Color)
                        newButton:SetFocusedBorderColor(options.Border.Color)
                        newButton:SetPushedBorderColor(options.Border.Color)
                    else
                        newButton:SetNormalBorderColor(options.Border.Color.Normal)
                        newButton:SetFocusedBorderColor(options.Border.Color.Focused)
                        newButton:SetPushedBorderColor(options.Border.Color.Pushed)
                    end
                end
            end

            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    if options.Background.Color.R ~= nil and options.Background.Color.G ~= nil and options.Background.Color.B ~= nil then
                        newButton:SetNormalBackgroundColor(options.Background.Color)
                        newButton:SetFocusedBackgroundColor(options.Background.Color)
                        newButton:SetPushedBackgroundColor(options.Background.Color)
                    else
                        newButton:SetNormalBackgroundColor(options.Background.Color.Normal)
                        newButton:SetFocusedBackgroundColor(options.Background.Color.Focused)
                        newButton:SetPushedBackgroundColor(options.Background.Color.Pushed)
                    end
                end
                newButton:SetTransparentBackground(options.Background.Transparent)        
            end

            newButton:SetUserData(options.UserData)

            newButton:Create()

            if options.Show then
                newButton:Show()
            end
        end

        OUTPUT("-------- End Create : Button --------")
        return newButton
    end,

    CreateInstance = function(self, control)
        OUTPUT("-------- Create Button By Instance --------")
        

        local options = {
            _Control = control,
        }

        OUTPUT("-------- End Create Button By Instance --------")        
        
        return self:Create(options)
    end
}
