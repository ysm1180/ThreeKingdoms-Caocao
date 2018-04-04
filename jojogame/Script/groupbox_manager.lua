require "Script\\control_manager.lua"
require "Script\\groupbox.lua"

GroupBoxManager = ControlManager:Instance 
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Group Box --------")

        local newControl = nil

        if options ~= nil then

            if options._Control ~= nil then
                return GroupBox:Instance(options._Control)
            end
            
            newControl = GroupBox:New(options.Parent)

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

                -- if options.Text.Color ~= nil then
                --     if options.Text.Color.R ~= nil and options.Text.Color.G ~= nil and options.Text.Color.B ~= nil then
                --         newButton:SetNormalTextColor(options.Text.Color)                        
                --         newButton:SetFocusedTextColor(options.Text.Color)
                --         newButton:SetPushedTextColor(options.Text.Color)                    
                --     else
                --         newButton:SetNormalTextColor(options.Text.Color.Normal)
                --         newButton:SetFocusedTextColor(options.Text.Color.Focused)
                --         newButton:SetPushedTextColor(options.Text.Color.Pushed)
                --     end
                -- end

                newControl:SetText(options.Text.Content)
            end

            newControl:SetUserData(options.UserData)

            newControl:Create()

            if options.Show then
                newControl:Show()
            end
        end

        OUTPUT("-------- End Create : Group Box --------")
        return newControl
    end,

    CreateInstance = function(self, control)
        OUTPUT("-------- Create Group Box By Instance --------")
        

        local options = {
            _Control = control,
        }

        OUTPUT("-------- End Create Group Box By Instance --------")        
        
        return self:Create(options)
    end
}
