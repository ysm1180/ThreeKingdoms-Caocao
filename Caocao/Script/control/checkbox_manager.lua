require "checkbox.lua"

---@class CheckboxEvent
---@field Create function @생성 이벤트
---@field Destroy function @파괴 이벤트
---@field MouseLButtonUp function @마우스 왼쪽 버튼 눌렀다 땠을 때 이벤트
---@field MouseLButtonDown function @마우스 왼쪽 버튼 눌렀을 때 이벤트
---@field MouseMove function @마우스 움직일 때 이벤트
---@field MouseEnter function @마우스 진입 이벤트
---@field MouseLeave function @마우스 벗어났을 때 이벤트

---@class CheckboxCreateOption
---@field _Control any @해당 값이 있으면 모든 옵션은 무시되고 해당 컨트롤로 Checkbox object를 생성한다.
---@field Parent Window 
---@field Width integer
---@field Height integer
---@field Event CheckboxEvent
---@field Font FontOption
---@field Text string
---@field UserData string
---@field Checked boolean
---@field Show boolean

---@class CheckboxManager @Checkbox object를 관리하는 Manager class
CheckboxManager = {}

---Checkbox object를 생성한다.
---@param options CheckboxCreateOption @버튼 생성 옵션
---@return Checkbox
function CheckboxManager:Create(options)
    local newControl = nil

    if options ~= nil then

        if options._Control ~= nil then
            return Checkbox:Instance(options._Control)
        end

        newControl = Checkbox:New(options.Parent)

        -- 크기 설정
        newControl:SetSize(options.Width, options.Height)

        -- 위치 설정
        newControl:Move(options.X, options.Y)

        -- 이벤트 설정
        if options.Event ~= nil then
            newControl:SetCreateEvent(options.Event.Create)
            newControl:SetDestroyEvent(options.Event.Destroy)
            newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
            newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
            newControl:SetMouseMoveEvent(options.Event.MouseMove)
            newControl:SetMouseEnterEvent(options.Event.MouseEnter)
            newControl:SetMouseLeaveEvent(options.Event.MouseLeave)
        end

        -- 폰트 설정
        if options.Font ~= nil then
            newControl:SetFontName(options.Font.Name)
            newControl:SetFontSize(options.Font.Size)
            newControl:SetBold(options.Font.Bold)
            newControl:SetUnderline(options.Font.Underline)
            newControl:SetItalic(options.Font.Italic)
        end

        newControl:SetText(options.Text)

        -- 임의 데이터 설정
        newControl:SetUserData(options.UserData)

        -- 생성
        newControl:Create()

        -- 체크 여부 설정
        newControl:SetChecked(options.Checked)

        -- Show
        if options.Show then
            newControl:Show()
        end
    end
    return newControl
end

---EngineCheckbox object를 기반으로 Checkbox object를 생성한다.
---@param control any @Base EngineCheckbox control object
---@return Checkbox
function CheckboxManager:CreateInstance(control)
    local options = {
        _Control = control
    }

    return CheckboxManager:Create(options)
end

