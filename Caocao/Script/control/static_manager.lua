require "static.lua"

---@class StaticEvent
---@field Create function @생성 이벤트
---@field Destroy function @파괴 이벤트

---@class StaticCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Parent Window @control 이 생성될 부모 control object 지정
---@field Width integer @control 너비
---@field Height integer @control 높이
---@field AutoSize boolean @자동 크기 조절 여부
---@field X integer @X 위치
---@field Y integer @Y 위치
---@field Event StaticEvent
---@field Font FontOption
---@field Text string 
---@field TextAlign integer @텍스트 정렬, Left - 0, Right - 1, Center - 2
---@field TextColor integer | RGB @텍스트 색
---@field BackgroundColor integer | RGB @배경 색
---@field Transparent boolean @배경 투명화 여부
---@field Show boolean @생성 후 Show 여부

---@class StaticManager @Static object를 관리하는 Manager class
StaticManager = {}

---Static object를 생성합니다.
---@param options StaticCreateOptions
---@return Static
function StaticManager:Create(options)
    local newControl = nil
    if options ~= nil then
        if options.Control ~= nil then
            return Static:Instance(options.Control)
        end

        newControl = Static:New(options.Parent)

        -- 크기 설정
        newControl:SetSize(options.Width, options.Height)
        newControl:SetAutoSize(options.AutoSize)

        -- 위치 설정
        newControl:Move(options.X, options.Y)

        -- 텍스트 설정
        if options.Text ~= nil then
            if options.Text.Font ~= nil then
                newControl:SetFontName(options.Text.Font.Name)
                newControl:SetFontSize(options.Text.Font.Size)
                newControl:SetBold(options.Text.Font.Bold)
                newControl:SetUnderline(options.Text.Font.Underline)
                newControl:SetItalic(options.Text.Font.Italic)
            end
            newControl:SetAlign(options.Text.Align)
            newControl:SetText(options.Text.Content)
            newControl:SetTextColor(options.Text.Color)
        end

        -- 배경 설정
        if options.Background ~= nil then
            newControl:SetBackgroundColor(options.Background.Color)
            newControl:SetTransparentBackground(options.Background.Transparent)
        end

        -- 이벤트 설정
        if options.Event ~= nil then
            newControl:SetCreateEvent(options.Event.Create)
            newControl:SetDestroyEvent(options.Event.Destroy)
        end

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
---@return Static
function StaticManager:CreateInstance(control)
    local options = {
        Control = control
    }

    return self:Create(options)
end

