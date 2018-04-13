require "control_manager.lua"
require "static.lua"

StaticManager = ControlManager:Instance
{
    Create = function(self, options)
        local newControl = nil
        if options ~= nil then
            if options.Control ~= nil then
                return Static:Instance(options.Control)
            end

            newControl = Static:New(options.Parent)

            newControl:SetSize(options.Width, options.Height)
            newControl:SetAutoSize(options.AutoSize)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, false)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newControl:Move(options.X, options.Y)


            if options.Text ~= nil then
                newControl:SetAlign(options.Text.Align)
                newControl:SetText(options.Text.Content)
                newControl:SetTextColor(options.Text.Color)
            end

            if options.Background ~= nil then
                newControl:SetBackgroundColor(options.Background.Color)
                newControl:SetTransparentBackground(options.Background.Transparent)
            end

            if options.Event ~= nil then
                newControl:SetCreateEvent(options.Event.Create)
                newControl:SetDestroyEvent(options.Event.Destroy)     
                newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newControl:SetMouseMoveEvent(options.Event.MouseMove)
                newControl:SetMouseEnterEvent(options.Event.MouseEnter)
                newControl:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            newControl:Create()

            if options.Show then
                newControl:Show()
            end
        end

        return newControl
    end,

    CreateInstance = function(self, control)
        local options = {
            Control = control,
        }

        return self:Create(options)
    end,
}