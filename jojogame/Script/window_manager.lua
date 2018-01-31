require "Script\\control_manager.lua"
require "Script\\window.lua"

WindowManager = ControlManager:Instance
{
    Create = function(options)
        local newWindow = Window:Instance(options.Parent)

        newWindow:SetSize(options.Width, options.Height)

        if options.Center then
            local x, y = ControlManager.GetCenterPosition(options.Parent, options.Width, options.Height)
            if x ~= nil and y ~= nil then
                options.X = x
                options.Y = y
            end
        end
        newWindow:Move(options.X, options.Y)

        newWindow:SetTitleName(options.TitleName)
        newWindow:SetBackColor(options.BackColor)

        newWindow:SetMaxButton(options.MaxButton)
        newWindow:SetMinButton(options.MinButton)
        if options.ControlBox == nil then
            options.ControlBox = true
        end
        newWindow:SetControlBox(options.ControlBox)

        newWindow:SetCreateEvent(options.Create)
        newWindow:SetDestroyEvent(options.Destroy)     
        newWindow:SetActiveEvent(options.Active)
        newWindow:SetCloseEvent(options.Close)
        newWindow:SetMouseLButtonUpEvent(options.MouseLButtonUp)
        newWindow:SetMouseLButtonDownEvent(options.MouseLButtonDown)

        newWindow:Create()

        return newWindow
    end
}