require "Script\\control_manager.lua"
require "Script\\window.lua"

WindowManager = ControlManager:Instance
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Window --------")
        local newWindow = nil
        if options ~= nil then
            newWindow = Window:New(options.Parent)

            newWindow:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, false)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newWindow:Move(options.X, options.Y)

            newWindow:SetTitleName(options.TitleName)
            newWindow:SetBackColor(options.BackColor)

            newWindow:SetTitlebar(options.Titlebar)
            newWindow:SetMaxButton(options.MaxButton)
            newWindow:SetMinButton(options.MinButton)
            if options.ControlBox == nil then
                options.ControlBox = true
            end
            newWindow:SetControlBox(options.ControlBox)

            if options.Event then
                newWindow:SetCreateEvent(options.Event.Create)
                newWindow:SetDestroyEvent(options.Event.Destroy)     
                newWindow:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newWindow:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newWindow:SetMouseMoveEvent(options.Event.MouseMove)
                newWindow:SetMouseHoverEvent(options.Event.MouseHover)
                newWindow:SetMouseLeaveEvent(options.Event.MouseLeave)

                newWindow:SetActiveEvent(options.Event.Active)
                newWindow:SetCloseEvent(options.Event.Close)
                newWindow:SetSizeEvent(options.Event.Size)
            end

            if options.Layouts ~= nil then
                for i = 1, #options.Layouts do
                    newWindow:AddLayout(options.Layouts[i])
                end
            end

            newWindow:SetMenu(options.Menu)

            newWindow:Create()

            if options.Show then
                if options.Modal then
                    newWindow:ShowModalWindow()
                else
                    newWindow:Show()
                end
            end
        end

        OUTPUT("-------- End Create : Window --------")
        return newWindow
    end
}