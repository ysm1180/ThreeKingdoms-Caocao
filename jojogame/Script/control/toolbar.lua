require "control.lua"

-- @class Toolbar : 게임 내의 Toolbar 컨트롤을 Wrapping 하는 클래스
-- @inherit ControlObject
Toolbar = ControlObject:Instance
{
    -- @description 툴바를 새로 생성합니다.
    -- @return Toolbar : 새로 생성된 Toolbar class
    New = function(self)
        local newControl = {}

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateToolbar()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 툴바 높이를 반환합니다.
    -- @return int : 높이
    Height = function(self)
        return self.control:GetHeight()
    end,

    -- @description 툴바 버튼을 추가합니다.
    -- @param button ToolbarButton : 추가할 버튼
    AddButton = function(self, button)
        if button ~= nil then
            self.control:AddButton(button.control)
        end
    end,

    -- @description 툴바 사이즈를 자동으로 조정합니다.
    AutoSize = function(self)
        self.control:AutoSize()
    end,

    -- @description 툴바를 생성합니다.
    -- @param parentWindow Window : 툴바가 생성될 Window class
    -- @param imageWidth int : 툴바 버튼의 이미지 너비
    -- @param imageHeight int : 툴바 버튼의 이미지 높이
    Create = function(self, parentWindow, imageWidth, imageHeight)
        if parentWindow ~= nil then
            self.control:Create(parentWindow.control, imageWidth, imageHeight)
        end
    end,

    -- @description 툴바를 띄웁니다.
    Show = function(self)
        self.control:Show()
    end,

    -- @description 툴바를 숨깁니다.
    Hide = function(self)
        self.control:Hide()
    end,
}

ToolbarButton = ControlObject:Instance
{
    -- @description 툴바 버튼을 새로 생성합니다.
    -- @return ToolbarButton : 새로 생성된 ToolbarButton class
    New = function(self)
        local newControlButton = {}

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControlButton.control = controlManager:CreateToolbarButton()
        setmetatable(newControlButton, self)
        self.__index = self

        return newControlButton
    end,

    -- @description 텍스트를 반환합니다.
    -- @return string : 텍스트
    Text = function(self)
        return self.control:GetText()
    end,

    -- @description 버튼을 활성화합니다.
    Enable = function(self)
        self.control:SetEnabled(true)
    end,

    -- @description 버튼을 비활성화합니다.
    Disable = function(self)
        self.control:SetEnabled(false)
    end,

    -- @description 버튼의 이미지를 설정합니다
    -- @param image Image : 이미지
    SetImage = function(self, image)
        if image ~= nil then
            self.control:SetImage(image.control)
        else
            self.control:SetImage(nil)
        end
    end,

    -- @description 버튼의 텍스트를 설정합니다.
    -- @param text string : 텍스트
    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)
        end
    end,

    -- @description 버튼 클릭 이벤트를 설정합니다.
    -- @param event function : 클릭 이벤트
    SetClickEvent = function(self, event)
        if event ~= nil then
            self.control:SetClickEvent(event)
        end
    end,

    -- @description 버튼 툴팁 내용을 설정합니다.
    -- @param tooltip string : 툴팁
    SetTooltipText = function(self, tooltip)
        if tooltip ~= nil then
            self.control:SetTooltipText(tooltip)
        end
    end,

    -- @description 생성합니다.
    Create = function(self)
        self.control:Create()
    end,
}