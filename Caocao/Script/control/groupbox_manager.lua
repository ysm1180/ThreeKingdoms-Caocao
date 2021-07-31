require "groupbox.lua"

---@class GroupboxEvent
---@field Create function @생성 이벤트
---@field Destroy function @파괴 이벤트
---@field MouseLButtonUp function @마우스 왼쪽 버튼 눌렀다 땠을 때 이벤트
---@field MouseLButtonDown function @마우스 왼쪽 버튼 눌렀을 때 이벤트
---@field MouseMove function @마우스 움직일 때 이벤트
---@field MouseEnter function @마우스 진입 이벤트
---@field MouseLeave function @마우스 벗어났을 때 이벤트

---@class GroupboxCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Parent Window 
---@field Width integer 
---@field Height integer 
---@field X integer
---@field Y integer
---@field Event GroupboxEvent
---@field Font FontOption
---@field Text string
---@field UserData string
---@field Show boolean

---@class GroupboxManager @GroupBox object를 관리하는 Manager class
GroupboxManager = {}

---@param options GroupboxCreateOptions
---@return Groupbox
function GroupboxManager:Create(options)
    local newControl = nil

    if options ~= nil then
        if options._Control ~= nil then
            return Groupbox:Instance(options._Control)
        end

        newControl = Groupbox:New(options.Parent)

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

        -- 텍스트 설정
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

        -- Show
        if options.Show then
            newControl:Show()
        end
    end

    return newControl
end

---@param control any
---@return Groupbox
function GroupboxManager:CreateInstance(control)
    local options = {
        _Control = control
    }

    return GroupboxManager:Create(options)
end

