require "menu.lua"

MenuManager = Object:Instance
{
    CreateMenu = function(self, items)
        local newControl = Menu:New()

        for i = 1, #items do
            newControl:AddMenuItem(items[i])
        end

        return newControl
    end,

    CreateMenuItem = function(self, options)
        local newControl = nil

        if options ~= nil then
            if options._Control ~= nil then
                newControl = MenuItem:Instance(options._Control)
                return newControl
            else
                newControl = MenuItem:New()            
            end

            if options.Seperator == true then
                options.Text = {
                    Content = "-",
                    Color = {
                        Normal = {R = 0xD7, G = 0xD7, B = 0xD7},
                    },
                }
            end

            newControl:SetEnabled(options.Enabled)

            if options.Event then
                newControl:SetClickEvent(options.Event.Click)
            end

            if options.Text then    
                if options.Text.Font then
                    newControl:SetFontName(options.Text.Font.Name)
                    newControl:SetFontSize(options.Text.Font.Size)
                    newControl:SetBold(options.Text.Font.Bold)
                    newControl:SetUnderline(options.Text.Font.Underline)
                    newControl:SetItalic(options.Text.Font.Italic)
                end
                if options.Text.Color then
                    newControl:SetNormalTextColor(options.Text.Color.Normal)
                    newControl:SetFocusedTextColor(options.Text.Color.Focused)
                    newControl:SetDisabledTextColor(options.Text.Color.Disabled)
                    newControl:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
                newControl:SetText(options.Text.Content)
            end

            if options.Background then
                if options.Background.Color then
                    newControl:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newControl:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newControl:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newControl:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end


            if options.Child ~= nil then
                newControl:SetChildMenu(options.Child.control)
            elseif options.ChildItems ~= nil then
                newControl:SetChildMenu((MenuManager:CreateMenu(options.ChildItems)).control)
            end
        end

        return newControl
    end,

    GetMenuItemByPosition = function(self, menu, index)
        if index < 1 then
            return nil
        end
        
        local control = menu.control:GetMenuItemByPosition(index - 1)
        local options = {
            _Control = control,
        }

        return self:CreateMenuItem(options)
    end,
}