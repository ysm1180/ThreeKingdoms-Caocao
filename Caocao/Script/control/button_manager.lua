require "button.lua"

---@class ButtonCreateOption
---@field _Control any @해당 값이 있으면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Parent Window
---@field Width integer
---@field Height integer
---@field X integer
---@field Y integer
---@field Event ControlEvent
---@field Font FontOption
---@field Text string
---@field TextColor RGB | ButtonColor
---@field BorderWidth number @테두리 굵기
---@field BorderColor  RGB | ButtonColor 
---@field BackgroundColor RGB | ButtonColor
---@field Transparent boolean @배경 투명화 여부
---@field UserData string
---@field Show boolean

---@class ButtonManager @Button object를 관리하는 Manager class
ButtonManager = {}

---Button object를 생성합니다.
---@param options ButtonCreateOption
---@return Button
function ButtonManager:Create(options)
    ---@type Button
    local newClass = nil

    if options ~= nil then
        if options._Control ~= nil then
            return Button:Instance(options._Control)
        end

        newClass = Button:New(options.Parent)

        -- 크기 설정
        newClass:SetSize(options.Width, options.Height)

        -- 위치 설정
        newClass:Move(options.X, options.Y)

        -- 이벤트 설정
        if options.Event ~= nil then
            newClass:SetCreateEvent(options.Event.Create)
            newClass:SetDestroyEvent(options.Event.Destroy)
            newClass:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
            newClass:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
            newClass:SetMouseMoveEvent(options.Event.MouseMove)
            newClass:SetMouseEnterEvent(options.Event.MouseEnter)
            newClass:SetMouseLeaveEvent(options.Event.MouseLeave)
        end

        -- 폰트 설정
        if options.Font ~= nil then
            newClass:SetFontName(options.Font.Name)
            newClass:SetFontSize(options.Font.Size)
            newClass:SetBold(options.Font.Bold)
            newClass:SetUnderline(options.Font.Underline)
            newClass:SetItalic(options.Font.Italic)
        end

        if options.TextColor ~= nil then
            if options.TextColor.R ~= nil and options.TextColor.G ~= nil and options.TextColor.B ~= nil then
                newClass:SetNormalTextColor(options.TextColor)
                newClass:SetFocusedTextColor(options.TextColor)
                newClass:SetPushedTextColor(options.TextColor)
            else
                newClass:SetNormalTextColor(options.TextColor.Normal)
                newClass:SetFocusedTextColor(options.TextColor.Focused)
                newClass:SetPushedTextColor(options.TextColor.Pushed)
            end
        end
        newClass:SetText(options.Text)

        -- 테두리 설정
        newClass:SetBorderWidth(options.BorderWidth)
        if options.BorderColor ~= nil then
            if options.BorderColor.R ~= nil and options.BorderColor.G ~= nil and options.BorderColor.B ~= nil then
                newClass:SetNormalBorderColor(options.BorderColor)
                newClass:SetFocusedBorderColor(options.BorderColor)
                newClass:SetPushedBorderColor(options.BorderColor)
            else
                newClass:SetNormalBorderColor(options.BorderColor.Normal)
                newClass:SetFocusedBorderColor(options.BorderColor.Focused)
                newClass:SetPushedBorderColor(options.BorderColor.Pushed)
            end
        end

        -- 배경 설정
        if options.BackgroundColor ~= nil then
            if options.BackgroundColor.R ~= nil and options.BackgroundColor.G ~= nil and options.BackgroundColor.B ~=
                nil then
                newClass:SetNormalBackgroundColor(options.BackgroundColor)
                newClass:SetFocusedBackgroundColor(options.BackgroundColor)
                newClass:SetPushedBackgroundColor(options.BackgroundColor)
            else
                newClass:SetNormalBackgroundColor(options.BackgroundColor.Normal)
                newClass:SetFocusedBackgroundColor(options.BackgroundColor.Focused)
                newClass:SetPushedBackgroundColor(options.BackgroundColor.Pushed)
            end
        end

        newClass:SetTransparentBackground(options.Transparent)

        -- 임의 데이터 설정
        newClass:SetUserData(options.UserData)

        -- 생성
        newClass:Create()

        -- 나타내기
        if options.Show then
            newClass:Show()
        end
    end

    return newClass
end

---EngineButtonControl object를 기반으로 Button object 생성
---@param control EngineButtonControl
---@return Button
function ButtonManager:CreateInstance(control)
    ---@type ButtonOption
    local options = {
        _Control = control
    }
    return self:Create(options)
end
