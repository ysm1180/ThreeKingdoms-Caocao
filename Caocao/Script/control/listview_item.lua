require "control.lua"
require "text_control.lua"

---@class ListViewItem : Control, ExtendFontControl @Listview 컨트롤의 아이템을 Wrapping 한 클래스
ListViewItem = Class(Control, ExtendFontControl)
-- 기본 색
ListViewItem.DEFAULT_BACKGROUND_COLOR = {
    Normal = {
        R = 0xFF,
        G = 0xFF,
        B = 0xFF
    },
    Focused = {
        R = 0x00,
        G = 0x78,
        B = 0xD7
    },
    Disabled = {
        R = 0xFF,
        G = 0xFF,
        B = 0xFF
    },
    DisableFocused = {
        R = 0xE6,
        G = 0xE6,
        B = 0xE6
    }
}
ListViewItem.DEFAULT_TEXT_COLOR = {
    Normal = {
        R = 0x00,
        G = 0x00,
        B = 0x00
    },
    Focused = {
        R = 0xFF,
        G = 0xFF,
        B = 0xFF
    },
    Disabled = {
        R = 0x6D,
        G = 0x6D,
        B = 0x6D
    },
    DisableFocused = {
        R = 0x6D,
        G = 0x6D,
        B = 0x6D
    }
}

-- Align
ListViewItem.ITEM_TEXT_ALIGN = {
    LEFT = 0,
    RIGHT = 1,
    CENTER = 2
}

---기존 컨트롤을 받아와 ListViewItem 클래스를 생성합니다.
---@param control control @기존의 ListViewItem 컨트롤
---@return ListViewItem @기존 control 의 wrapping 된 ListViewItem 클래스
function ListViewItem:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---ListView Item 을 새로 생성합니다.
---@return ListViewItem @새로 생성된 ListViewItem class
function ListViewItem:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateListViewItem()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---텍스트를 반환합니다.
---@return string @텍스트
function ListViewItem:Text()
    return self.control:GetText()
end

---텍스트를 설정합니다.
---@param text string @텍스트
function ListViewItem:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---텍스트 정렬을 설정합니다.
---@param align number @정렬, Left - 0, Right - 1, Center - 2
function ListViewItem:SetAlign(align)
    if align ~= nil and align >= ITEM_TEXT_ALIGN.LEFT and align <= ITEM_TEXT_ALIGN.CENTER then
        self.control:SetAlign(align)
    end
end

---기본 배경 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetNormalBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Normal.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Normal.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Normal.B
        self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---마우스로 Focus 된 배경 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetFocusedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Focused.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Focused.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Focused.B
        self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---비활성화된 배경 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetDisabledBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Disabled.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Disabled.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Disabled.B
        self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---마우스로 Focus 된 비활성화 배경 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetDisableFocusedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DisableFocused.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DisableFocused.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DisableFocused.B
        self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---기본 글자 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetNormalTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.Normal.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.Normal.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.Normal.B
        self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---마우스로 Focus 된 글자 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Focused.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Focused.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Focused.B
        self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---비활성화된 글자 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetDisabledTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.Disabled.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.Disabled.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.Disabled.B
        self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---마우스로 Focus 된 비활성화 글자 색을 설정합니다.
---@param color integer | RGB @색
function ListViewItem:SetDisableFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DisableFocused.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DisableFocused.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DisableFocused.B
        self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end
