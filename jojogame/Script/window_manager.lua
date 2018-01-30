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
        
        newWindow:SetBackColor(options.BackColor)

        newWindow:SetCreateEvent(options.CreateEvent)
        newWindow:SetDestroyEvent(options.DestroyEvent)        
        newWindow:SetClickEvent(options.CloseEvent)

        newWindow:SetActiveEvent(options.ActiveEvent)
        newWindow:SetCloseEvent(options.CloseEvent)
        newWindow:Create()
        
        return newWindow
    end,
}