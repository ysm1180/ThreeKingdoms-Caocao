require "Script\\control_manager.lua"
require "Script\\window.lua"

WindowManager = ControlManager:Instance
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Window --------")
        local newWindow = nil
        if options ~= nil then
            newWindow = Window:Instance(options.Parent)

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

            newWindow:SetCreateEvent(options.Create)
            newWindow:SetDestroyEvent(options.Destroy)     
            newWindow:SetActiveEvent(options.Active)
            newWindow:SetCloseEvent(options.Close)
            newWindow:SetMouseLButtonUpEvent(options.MouseLButtonUp)
            newWindow:SetMouseLButtonDownEvent(options.MouseLButtonDown)
            newWindow:SetMouseMoveEvent(options.MouseMove)
            newWindow:SetMouseHoverEvent(options.MouseHover)
            newWindow:SetMouseLeaveEvent(options.MouseLeave)

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