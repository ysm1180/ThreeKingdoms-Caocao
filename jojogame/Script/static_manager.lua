require "Script\\control_manager.lua"
require "Script\\static.lua"

StaticManager = ControlManager:Instance
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Static --------")
        local newStatic = nil
        if options ~= nil then
            if options.Control ~= nil then
                return Static:Instance(options.Control)
            end

            newStatic = Static:New(options.Parent)

            newStatic:SetSize(options.Width, options.Height)
            newStatic:SetAutoSize(options.AutoSize)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, false)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newStatic:Move(options.X, options.Y)


            if options.Text ~= nil then
                newStatic:SetAlign(options.Text.Align)
                newStatic:SetText(options.Text.Content)
                newStatic:SetTextColor(options.Text.Color)
            end

            if options.Background ~= nil then
                newStatic:SetBackgroundColor(options.Background.Color)
            end

            if options.Event ~= nil then
                newStatic:SetCreateEvent(options.Event.Create)
                newStatic:SetDestroyEvent(options.Event.Destroy)     
                newStatic:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newStatic:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newStatic:SetMouseMoveEvent(options.Event.MouseMove)
                newStatic:SetMouseEnterEvent(options.Event.MouseEnter)
                newStatic:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            newStatic:Create()

            if options.Show then
                newStatic:Show()
            end
        end

        OUTPUT("-------- End Create : Static --------")
        return newStatic
    end,

    CreateInstance = function(self, control)
        OUTPUT("-------- Create Static By Instance --------")

        local options = {
            Control = control,
        }

        OUTPUT("-------- End Static By Instance --------")        
        
        return self:Create(options)
    end,
}