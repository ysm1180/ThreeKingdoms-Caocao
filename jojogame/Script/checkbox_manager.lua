require "Script\\control_manager.lua"
require "Script\\checkbox.lua"

CheckBoxManager = ControlManager:Instance 
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Group Box --------")

        local newCheckBox = nil

        if options ~= nil then

            if options._Control ~= nil then
                return CheckBox:Instance(options._Control)
            end
            
            newCheckBox = CheckBox:New(options.Parent)

            newCheckBox:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newCheckBox:Move(options.X, options.Y)

            if options.Event ~= nil then
                newCheckBox:SetCreateEvent(options.Event.Create)
                newCheckBox:SetDestroyEvent(options.Event.Destroy)
                newCheckBox:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newCheckBox:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newCheckBox:SetMouseMoveEvent(options.Event.MouseMove)
                newCheckBox:SetMouseEnterEvent(options.Event.MouseEnter)
                newCheckBox:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            if options.Text ~= nil then    
                if options.Text.Font ~= nil then
                    newCheckBox:SetFontName(options.Text.Font.Name)
                    newCheckBox:SetFontSize(options.Text.Font.Size)
                    newCheckBox:SetBold(options.Text.Font.Bold)
                    newCheckBox:SetUnderline(options.Text.Font.Underline)
                    newCheckBox:SetItalic(options.Text.Font.Italic)
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

                newCheckBox:SetText(options.Text.Content)
            end
            newCheckBox:SetUserData(options.UserData)

            newCheckBox:Create()

            if options.Show then
                newCheckBox:Show()
            end
        end

        OUTPUT("-------- End Create : Group Box --------")
        return newCheckBox
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
