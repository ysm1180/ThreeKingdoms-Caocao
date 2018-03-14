require "Script\\menu.lua"

MenuManager = Object:Instance
{
    CreateMenu = function(self, items)
        OUTPUT("-------- Start Create : Menu --------")
        local newMenu = Menu:New()

        for i = 1, #items do
            newMenu:AddMenuItem(items[i])
        end

        OUTPUT("-------- End Create : Menu --------")
        return newMenu
    end,

    CreateMenuItem = function(self, options)
        OUTPUT("-------- Start Create : MenuItem --------")

        local newMenuItem = nil

        if options ~= nil then
            if options.Control ~= nil then
                newMenuItem = MenuItem:Instance(options.Control)
                return newMenuItem
            else
                newMenuItem = MenuItem:New()            
            end

            if options.Seperator == true then
                options.Text = {
                    Content = "-",
                    Color = {
                        Normal = {R = 0xD7, G = 0xD7, B = 0xD7},
                    },
                }
            end

            newMenuItem:SetEnabled(options.Enabled)

            if options.Event then
                newMenuItem:SetClickEvent(options.Event.Click)
            end

            if options.Text then    
                if options.Text.Font then
                    newMenuItem:SetFontName(options.Text.Font.Name)
                    newMenuItem:SetFontSize(options.Text.Font.Size)
                    newMenuItem:SetBold(options.Text.Font.Bold)
                    newMenuItem:SetUnderline(options.Text.Font.Underline)
                    newMenuItem:SetItalic(options.Text.Font.Italic)
                end
                if options.Text.Color then
                    newMenuItem:SetNormalTextColor(options.Text.Color.Normal)
                    newMenuItem:SetFocusedTextColor(options.Text.Color.Focused)
                    newMenuItem:SetDisabledTextColor(options.Text.Color.Disabled)
                    newMenuItem:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
                newMenuItem:SetText(options.Text.Content)
            end

            if options.Background then
                if options.Background.Color then
                    newMenuItem:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newMenuItem:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newMenuItem:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newMenuItem:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end


            if options.Child ~= nil then
                newMenuItem:SetChildMenu(options.Child.control)
            elseif options.ChildItems ~= nil then
                newMenuItem:SetChildMenu((MenuManager:CreateMenu(options.ChildItems)).control)
            end
        end

        OUTPUT("-------- End Create : MenuItem --------")
        return newMenuItem
    end,

    GetMenuItemByPosition = function(self, menu, index)
        if index < 1 then
            return nil
        end
        
        OUTPUT("-------- Get MenuItem By Position --------")
        
        local control = menu.control:GetMenuItemByPosition(index - 1)
        local options = {
            Control = control,
            -- Text = {
            --     Content = control:GetText(),
            --     Color = {
            --         Normal = control:GetNormalTextColor(),
            --         Focused = control:GetFocusedTextColor(),
            --         Disabled = control:GetDisabledTextColor(),
            --         DisableFocused = control:GetDisableFocusedTextColor(),
            --     },
            --     Font = {
            --         Name = control:GetFont():GetFontName(),
            --         Size = control:GetFont():GetFontSize(),
            --         Bold = control:GetFont():IsBold(),
            --         Underline = control:GetFont():IsUnderline(),
            --         Italic = control:GetFont():IsItalic(),
            --     }
            -- },
            -- Background = {
            --     Color = {
            --         Normal = control:GetNormalBackgroundColor(),
            --         Focused = control:GetFocusedBackgroundColor(),
            --         Disabled = control:GetDisabledBackgroundColor(),
            --         DisableFocused = control:GetDisableFocusedBackgroundColor(),
            --     }
            -- },
            -- Enabled = control:IsEnabled(),
            -- Click = control:GetClickEvent(),
            -- Child = {control = control:GetChildMenu()}
        }

        OUTPUT("-------- End Get MenuItem By Position --------")        
        
        return self:CreateMenuItem(options)
    end,
}