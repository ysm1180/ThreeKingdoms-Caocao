require "text_control.lua"

-- @class Menu : 윈도우 창의 메뉴를 Wrapping 하는 클래스
-- @inherit TextControl
Menu = ControlObject:Instance
{
    -- @description Menu 를 새로 생성합니다.
    -- @return Menu : 새로 생성된 Menu class
    New = function(self)
        local newControl = {}

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateMenu()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 메뉴 아이템을 추가합니다.
    -- @param menuItem MenuItem : 추가할 메뉴 아이템
    AddMenuItem = function(self, menuItem)
        if menuItem ~= nil then
            self.control:AddMenuItem(menuItem.control)
        end
    end,
}

-- @class MenuItem : 메뉴의 아이템을 Wrapping 하는 클래스
-- @inherit TextControl
MenuItem = TextControl:Instance
{
    -- @description 기본 색
    DEFAULT_BACKGROUND_COLOR = {
        NORMAL = {R = 0xF2, G = 0xF2, B = 0xF2},
        FOCUSED = {R = 0x91 , G = 0xC9, B = 0xF7},
        DISABLED = {R = 0xF2 , G = 0xF2, B = 0xF2},
        DISABLE_FOCUSED = {R = 0xE6, G = 0xE6, B = 0xE6},
    },
    DEFAULT_TEXT_COLOR = {
        NORMAL = {R = 0x00, G = 0x00, B = 0x00},
        FOCUSED = {R = 0x00, G = 0x00, B = 0x00},
        DISABLED = {R = 0x6D , G = 0x6D, B = 0x6D},
        DISABLE_FOCUSED = {R = 0x6D, G = 0x6D, B = 0x6D},
    },

    -- @description 기존 컨트롤을 받아와 MenuItem 클래스를 생성합니다.
    -- @param control control : 기존의 MenuItem 컨트롤
    -- @return MenuItem : 기존 control 의 wrapping 된 MenuItem 클래스
    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description Menu Item 을 새로 생성합니다.
    -- @return MenuItem : 새로 생성된 MenuItem class
    New = function(self)
        local newControl = {}

        newControl.control = controlManager:CreateMenuItem()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 텍스트를 반환합니다.
    -- @return string : 텍스트 내용
    Text = function(self)
        return self.control:GetText()
    end,

    -- @description 활성화 여부를 설정합니다.
    -- @param value boolean : 활성화 여부
    SetEnabled = function(self, value)
        if type(value) == "boolean" then
            self.control:SetEnabled(value)
        end
    end,

    -- @description 텍스트를 설정합니다.
    -- @param text string : 텍스트 내용
    SetText = function(self, text)
        self.control:SetText(text)
    end,

    -- @description 클릭 이벤트를 설정합니다.
    -- @param event function : 클릭 이벤트
    SetClickEvent = function(self, event)
        self.control:SetClickEvent(event)
    end,

    -- @description 서브 메뉴를 설정합니다
    -- @param menuControl Menu : 서브 메뉴
    SetChildMenu = function(self, menuControl)
        if menuControl ~= nil then
            self.control:SetChildMenu(menuControl)
        end
    end,

    -- @description 메뉴 아이템의 기본 배경 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 메뉴 아이템에 마우스를 올려 focus 된 배경 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 메뉴 아이템의 비활성화 된 배경 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisabledBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLED.B
            self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 비활성화 메뉴 아이템에 마우스를 올려 focus 된 배경 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisableFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 메뉴 아이템의 기본 글자 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetNormalTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 메뉴 아이템에 마우스를 올려 focus 된 글자 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.FOCUSED.B
            self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 메뉴 아이템의 비활성화 된 글자 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisabledTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLED.B
            self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 비활성화 메뉴 아이템에 마우스를 올려 focus 된 글자 색을 지정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisableFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,
}