require "control_manager.lua"
require "combobox.lua"

ComboBoxManager = ControlManager:Instance 
{
    Create = function(self, options)
        local newControl = nil

        if options ~= nil then

            if options._Control ~= nil then
                return ComboBox:Instance(options._Control)
            end
            
            newControl = ComboBox:New(options.Parent)

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

            if options.Items ~= nil then
                for i = 1, #options.Items do
                    newControl:AddItem(options.Items[i])
                end
            end

            newControl:SetUserData(options.UserData)

            newControl:Create()

            if options.Show then
                newControl:Show()
            end
        end

        return newControl
    end,

    CreateInstance = function(self, control)
        local options = {
            _Control = control,
        }
        
        return self:Create(options)
    end
}
