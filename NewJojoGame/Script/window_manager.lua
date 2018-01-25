require "object.lua"

WindowManager = {
    Create = function(options)
        if options.Parent ~= nil then
            newWindow = controlManager:CreateWindowForm(options.Parent)
        else            
            newWindow = controlManager:CreateWindowForm(nil)
        end

        if options.Width ~= nil then
            newWindow:SetWidth(options.Width)
        end
        if options.Height ~= nil then
            newWindow:SetHeight(options.Height)
        end
        if options.X ~= nil then
            newWindow:SetX(options.X)
        end
        if options.Y ~= nil then
            newWindow:SetY(options.Y)
        end
        if options.CloseEvent ~= nil then
            newWindow:SetCloseEvent(options.CloseEvent)
        end
        if options.TitleName ~= nil then
            newWindow:SetTitleName(options.TitleName)
        end
        if options.Color ~= nil then
            newWindow:SetBackColor(gameManager:Color(options.Color))
        end
        newWindow:Create()

        return newWindow
    end
}