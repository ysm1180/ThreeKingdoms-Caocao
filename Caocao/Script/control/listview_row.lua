require "control.lua"
require "text_control.lua"

---@class ListViewRow : ControlObject @Listview 컨트롤의 row 를 wrapping 한 클래스
ListViewRow = Class(ControlObject)

-- 기본 색
ListViewRow.DEFAULT_BACKGROUND_COLOR = {
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
ListViewRow.DEFAULT_TEXT_COLOR = {
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

---기존 컨트롤을 받아와 ListViewRow 클래스를 생성합니다.
---@param control any
---@return ListViewRow
function ListViewRow:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---ListView Row 을 새로 생성합니다.
---@return ListViewRow
function ListViewRow:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateListViewRow()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---활성화 여부를 반환합니다.
---@return boolean 
function ListViewRow:Enabled()
    return self.control:IsEnabled()
end

---아이템을 추가합니다.
---@param item ListViewItem @추가할 리스트뷰 아이템
function ListViewRow:AddItem(item)
    if item ~= nil then
        self.control:AddItem(item.control)
    end
end

---해당 index 열에 해당하는 아이템을 설정합니다.
---@param index number @아이템이 설정될 column 위치
---@param item ListViewItem @추가할 리스트뷰 아이템
function ListViewRow:SetItem(index, item)
    if item ~= nil then
        self.control:SetItem(index, item.control)
    end
end

---활성화 여부를 설정합니다.
---@param value boolean 
function ListViewRow:SetEnabled(value)
    if type(value) == "boolean" then
        self.control:SetEnabled(value)
    end
end

---활성 이벤트를 설정합니다.
---@param event function
function ListViewRow:SetActiveEvent(event)
    if event ~= nil then
        self.control:SetActiveEvent(event)
    end
end

---기본 배경 색을 설정합니다.
---@param color integer | RGB
function ListViewRow:SetNormalBackgroundColor(color)
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
---@param color integer | RGB
function ListViewRow:SetFocusedBackgroundColor(color)
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
---@param color integer | RGB 
function ListViewRow:SetDisabledBackgroundColor(color)
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
---@param color integer | RGB
function ListViewRow:SetDisableFocusedBackgroundColor(color)
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
        self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---기본 글자 색을 설정합니다.
---@param color integer | RGB
function ListViewRow:SetNormalTextColor(color)
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
---@param color integer | RGB
function ListViewRow:SetFocusedTextColor(color)
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
---@param color integer | RGB 
function ListViewRow:SetDisabledTextColor(color)
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
---@param color integer | RGB
function ListViewRow:SetDisableFocusedTextColor(color)
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
