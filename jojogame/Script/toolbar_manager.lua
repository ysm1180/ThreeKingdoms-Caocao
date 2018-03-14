require "Script\\toolbar.lua"

ToolbarManager = Object:Instance
{
    CreateToolbar = function(self, options)
        OUTPUT("-------- Start Create : Toolbar --------")

        local newToolbar = nil
        if options ~= nil then
            newToolbar = Toolbar:New()

            newToolbar:Create(options.Parent, options.Image.Width, options.Image.Height)

            for i = 1, #options.Buttons do
                newToolbar:AddButton(options.Buttons[i])
            end

            if options.Show then
                newToolbar:Show()
            end
        end

        OUTPUT("-------- End Create : Toolbar --------")
        return newToolbar
    end,

    CreateToolbarButton = function(self, options)
        OUTPUT("-------- Start Create : Toolbar Button --------")
        local newToolbarButton = ToolbarButton:New()

        if options.Enabled == false then
            newToolbarButton:Disable()
        end

        if options.Text ~= nil then
            newToolbarButton:SetText(options.Text.Content)
            newToolbarButton:SetTooltipText(options.Text.Tooltip)
        end

        if options.Event then
            newToolbarButton:SetClickEvent(options.Event.Click)
        end
        
        newToolbarButton:SetImage(options.Image)

        newToolbarButton:Create()

        if options.Show then
            newToolbarButton:Show()
        end

        OUTPUT("-------- End Create : Toolbar Button --------")
        return newToolbarButton
    end
}