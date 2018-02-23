require "Script\\object.lua"
require "Script\\menu.lua"

MenuManager = Object:Instance
{
    CreateMenu = function(self, items)
        OUTPUT("-------- Start Create : Menu --------")
        local newMenu = Menu:Instance()

        for i = 1, #items do
            OUTPUT(items[i]:Text())
            newMenu:AddMenuItem(items[i])
        end

        OUTPUT("-------- End Create : Menu --------")
        return newMenu
    end,

    CreateMenuItem = function(self, options)
        OUTPUT("-------- Start Create : MenuItem --------")
        local newMenuItem = MenuItem:Instance()

        newMenuItem:SetEnabled(options.Enabled)
        newMenuItem:SetText(options.Text)
        newMenuItem:SetClickEvent(options.Click)
        if options.Child ~= nil then
            newMenuItem:SetChildMenu(options.Child)
        elseif options.ChildItems ~= nil then
            newMenuItem:SetChildMenu(MenuManager:CreateMenu(options.ChildItems))
        end

        OUTPUT("-------- End Create : MenuItem --------")
        return newMenuItem
    end,
}