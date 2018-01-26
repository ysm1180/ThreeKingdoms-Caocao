require "window.lua"

WindowManager = {
    Create = function(options)
        local newWindow = Window:Instance(options.Parent)

        newWindow:SetSize(options.Width, options.Height)
        newWindow:Move(options.X, options.Y)
        
        newWindow:SetBackColor(options.BackColor)
        newWindow:Create()
        
        return newWindow
    end
}