require "control_manager.lua"
require "window.lua"

WindowManager = ControlManager:Instance
{
    Create = function(self, options)
        local newControl = nil
        if options ~= nil then
            if options.Control ~= nil then
                return Window:Instance(options.Control)
            end

            newControl = Window:New(options.Parent)

            newControl:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, false)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newControl:Move(options.X, options.Y)

            newControl:SetTitleName(options.TitleName)
            if options.Background ~= nil then
                newControl:SetBackgroundColor(options.Background.Color)
            end

            newControl:SetTitlebar(options.Titlebar)
            newControl:SetMaxButton(options.MaxButton)
            newControl:SetMinButton(options.MinButton)
            if options.ControlBox == nil then
                options.ControlBox = true
            end
            newControl:SetControlBox(options.ControlBox)

            if options.Event then
                newControl:SetCreateEvent(options.Event.Create)
                newControl:SetDestroyEvent(options.Event.Destroy)     
                newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newControl:SetMouseMoveEvent(options.Event.MouseMove)
                newControl:SetMouseEnterEvent(options.Event.MouseEnter)
                newControl:SetMouseLeaveEvent(options.Event.MouseLeave)

                newControl:SetActiveEvent(options.Event.Active)
                newControl:SetCloseEvent(options.Event.Close)
                newControl:SetSizeEvent(options.Event.Size)
            end

            if options.Layouts ~= nil then
                for i = 1, #options.Layouts do
                    newControl:AddLayout(options.Layouts[i])
                end
            end

            newControl:SetMenu(options.Menu)

            newControl:Create()

            if options.Show then
                if options.Modal then
                    newControl:ShowModalWindow()
                else
                    newControl:Show()
                end
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