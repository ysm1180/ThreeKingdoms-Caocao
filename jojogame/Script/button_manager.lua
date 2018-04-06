require "Script\\control_manager.lua"
require "Script\\button.lua"

ButtonManager = ControlManager:Instance 
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Button --------")

        local newControl = nil

        if options ~= nil then

            if options._Control ~= nil then
                return Button:Instance(options._Control)
            end
            
            newControl = Button:New(options.Parent)

            newControl:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newControl:Move(options.X, options.Y)

            if options.Event ~= nil then
                newControl:SetCreateEvent(options.Event.Create)
                newControl:SetDestroyEvent(options.Event.Destroy)
                newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newControl:SetMouseMoveEvent(options.Event.MouseMove)
                newControl:SetMouseEnterEvent(options.Event.MouseEnter)
                newControl:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            if options.Text ~= nil then    
                if options.Text.Font ~= nil then
                    newControl:SetFontName(options.Text.Font.Name)
                    newControl:SetFontSize(options.Text.Font.Size)
                    newControl:SetBold(options.Text.Font.Bold)
                    newControl:SetUnderline(options.Text.Font.Underline)
                    newControl:SetItalic(options.Text.Font.Italic)
                end
                if options.Text.Color ~= nil then
                    if options.Text.Color.R ~= nil and options.Text.Color.G ~= nil and options.Text.Color.B ~= nil then
                        newControl:SetNormalTextColor(options.Text.Color)                        
                        newControl:SetFocusedTextColor(options.Text.Color)
                        newControl:SetPushedTextColor(options.Text.Color)                    
                    else
                        newControl:SetNormalTextColor(options.Text.Color.Normal)
                        newControl:SetFocusedTextColor(options.Text.Color.Focused)
                        newControl:SetPushedTextColor(options.Text.Color.Pushed)
                    end
                end
                newControl:SetText(options.Text.Content)
            end

            if options.Border ~= nil then
                newControl:SetBorderWidth(options.Border.Width)
                if options.Border.Color ~= nil then
                    if options.Border.Color.R ~= nil and options.Border.Color.G ~= nil and options.Border.Color.B ~= nil then
                        newControl:SetNormalBorderColor(options.Border.Color)
                        newControl:SetFocusedBorderColor(options.Border.Color)
                        newControl:SetPushedBorderColor(options.Border.Color)
                    else
                        newControl:SetNormalBorderColor(options.Border.Color.Normal)
                        newControl:SetFocusedBorderColor(options.Border.Color.Focused)
                        newControl:SetPushedBorderColor(options.Border.Color.Pushed)
                    end
                end
            end

            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    if options.Background.Color.R ~= nil and options.Background.Color.G ~= nil and options.Background.Color.B ~= nil then
                        newControl:SetNormalBackgroundColor(options.Background.Color)
                        newControl:SetFocusedBackgroundColor(options.Background.Color)
                        newControl:SetPushedBackgroundColor(options.Background.Color)
                    else
                        newControl:SetNormalBackgroundColor(options.Background.Color.Normal)
                        newControl:SetFocusedBackgroundColor(options.Background.Color.Focused)
                        newControl:SetPushedBackgroundColor(options.Background.Color.Pushed)
                    end
                end
                newControl:SetTransparentBackground(options.Background.Transparent)        
            end

            newControl:SetUserData(options.UserData)

            newControl:Create()

            if options.Show then
                newControl:Show()
            end
        end

        OUTPUT("-------- End Create : Button --------")
        return newControl
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
