require "Script\\control_manager.lua"
require "Script\\button.lua"

ButtonManager = ControlManager:Instance 
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Button --------")

        local newButton = nil

        if options ~= nil then

            if options.Control ~= nil then
                newButton = Button:Instance(options.Control)
                return newButton
            else
                newButton = Button:New(options.Parent)
            end

            newButton:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newButton:Move(options.X, options.Y)

            if options.Event then
                newButton:SetCreateEvent(options.Event.Create)
                newButton:SetDestroyEvent(options.Event.Destroy)
                newButton:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newButton:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newButton:SetMouseMoveEvent(options.Event.MouseMove)
                newButton:SetMouseHoverEvent(options.Event.MouseHover)
                newButton:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            if options.Text then    
                if options.Text.Font then
                    newButton:SetFontName(options.Text.Font.Name)
                    newButton:SetFontSize(options.Text.Font.Size)
                    newButton:SetBold(options.Text.Font.Bold)
                    newButton:SetUnderline(options.Text.Font.Underline)
                    newButton:SetItalic(options.Text.Font.Italic)
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

            newButton:SetUserData(options.UserData)

            newButton:Create()

            if options.Show then
                newButton:Show()
            end
        end

        OUTPUT("-------- End Create : Button --------")
        return newButton
    end,

    CreateInstance = function(self, button)
        OUTPUT("-------- Create Button By Instance --------")
        

        local options = {
            Control = button,
        --     Parent = button:GetParentWindow(),
        --     Width = button:GetWidth(),
        --     Height = button:GetHeight(),
        --     X = button:GetX(),
        --     Y = button:GetY(),
        --     Create = button:GetCreateEvent(),
        --     Destroy = button:GetDestroyEvent(),
        --     MouseLButtonUp = button:GetMouseLButtonUpEvent(),
        --     MouseLButtonDown = button:GetMouseLButtonDownEvent(),
        --     MouseMove = button:GetMouseMoveEvent(),
        --     MouseHover = button:GetMouseHoverEvent(),
        --     MouseLeave = button:GetMouseLeaveEvent(),
        --     Text = {
        --         Content = button:GetText(),
        --         Color = {
        --             Normal = button:GetNormalTextColor(),
        --             Focused = button:GetFocusedTextColor(),
        --             Pushed = button:GetPushedTextColor(),
        --         },
        --         Font = {
        --             Name = button:GetFont():GetFontName(),
        --             Size = button:GetFont():GetFontSize(),
        --             Bold = button:GetFont():IsBold(),
        --             Underline = button:GetFont():IsUnderline(),
        --             Italic = button:GetFont():IsItalic(),
        --         }
        --     },
        --     Border = {
        --         Width = button:GetBorderWidth(),
        --         Color = {
        --             Normal = button:GetNormalBorderColor(),
        --             Focused = button:GetFocusedBorderColor(),
        --             Pushed = button:GetPushedBorderColor(),
        --         }
        --     },
        --     Background = {
        --         Transparent = button:IsTransparentBackground(),
        --         Color = {
        --             Normal = button:GetNormalBackgroundColor(),
        --             Focused = button:GetFocusedBackgroundColor(),
        --             Pushed = button:GetPushedBackgroundColor(),
        --         },
        --     },
        --     Enabled = button:IsEnabled(),
        --     Show = button:IsVisible(),
        --     UserData = button:GetUserData(),
        }

        OUTPUT("-------- End Create Button By Instance --------")        
        
        return self:Create(options)
    end
}
