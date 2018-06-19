require "toolbar.lua"

-- @class ToolbarMananger
-- @description Toolbar class 를 관리하는 Manager class
-- @inherit Object
ToolbarManager = Object:Instance
{
    -- @param options ToolbarOption : control 생성 옵션
    -- @return Toolbar : 생성된 Toolbar class
    -- @type ToolbarOption table
    --[[
        {
            Parent Window : 툴바가 생성될 부모 Window
            Image table : {
                Width int : 툴바 버튼 이미지의 너비
                Height int : 툴바 버튼 이미지의 높이
            }
            Buttons ToolbarButton[] : 추가할 툴바 버튼
            Show : 생성 후 Show 여부
        }
    ]]
    CreateToolbar = function(self, options)
        local newControl = nil
        if options ~= nil then
            newControl = Toolbar:New()

            -- 생성
            newControl:Create(options.Parent, options.Image.Width, options.Image.Height)

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
    end,

    -- @param options ToolbarButtonOptions : 툴바 버튼 생성 옵션
    -- @return ToolbarButton : 생성된 ToolbarButton class
    -- @type ToolbarButtonOptions table
    --[[
        {
            Enabled boolean : 버튼 활성화 여부
            Text table : {
                Content string : 텍스트 내용
                Tooltip string : 툴팁 내용
            }
            Event table : {
                Click function : 클릭 이벤트
            }
            Image Image : 버튼 이미지
            Show boolean : 생성 후 Show 여부
        }
    ]]
    CreateToolbarButton = function(self, options)
        local newControlButton = ToolbarButton:New()

        -- @description 활성화 설정
        if options.Enabled == false then
            newControlButton:Disable()
        end

        -- 텍스트 설정
        if options.Text ~= nil then
            newControlButton:SetText(options.Text.Content)
            newControlButton:SetTooltipText(options.Text.Tooltip)
        end

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
}