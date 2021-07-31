require "toolbar.lua"
require "toolbar_button.lua"

---@class ToolbarImageSize
---@field Width integer
---@field Height integer

---@class ToolbarCreateOptions
---@field Parent Window @툴바가 생성될 부모 Window
---@field ImageSize ToolbarImageSize
---@field Buttons ToolbarButton[] @추가할 툴바 버튼
---@field Show boolean @생성 후 Show 여부

---@class ToolbarButtonEvent
---@field Click function @클릭 이벤트

---@class ToolbarButtonCreateOptions
---@field Enabled boolean @버튼 활성화 여부
---@field Text string
---@field Tooltip string
---@field Event ToolbarButtonEvent
---@field Image Image @버튼 이미지
---@field Show boolean @생성 후 Show 여부

---@class ToolbarManager @Toolbar object를 관리하는 Manager class
ToolbarManager = {}

---@param options ToolbarCreateOptions
---@return Toolbar
function ToolbarManager:CreateToolbar(options)
    local newControl = nil
    if options ~= nil then
        newControl = Toolbar:New()

        -- 생성
        newControl:Create(options.Parent, options.ImageSize.Width, options.ImageSize.Height)
        options.Parent.Toolbar = newControl

        -- 툴바 버튼 설정
        for i = 1, #options.Buttons do
            newControl:AddButton(options.Buttons[i])
        end

        -- Show
        if options.Show then
            newControl:Show()
        end
    end

    return newControl
end

---@param options ToolbarButtonCreateOptions
---@return ToolbarButton 
function ToolbarManager:CreateToolbarButton(options)
    local newControlButton = ToolbarButton:New()

    ---활성화 설정
    if options.Enabled == false then
        newControlButton:Disable()
    end

    newControlButton:SetText(options.Text)
    newControlButton:SetTooltipText(options.Tooltip)

    -- 이벤트 설정
    if options.Event then
        newControlButton:SetClickEvent(options.Event.Click)
    end

    -- 이미지 설정
    newControlButton:SetImage(options.Image)

    -- 생성
    newControlButton:Create()

    -- Show
    if options.Show then
        newControlButton:Show()
    end

    return newControlButton
end

