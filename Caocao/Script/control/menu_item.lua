require "text_control.lua"

---@class MenuItem : Control, ExtendFontControl @메뉴의 아이템을 Wrapping 하는 클래스
MenuItem = Class(Control, ExtendFontControl)

-- 기본 색
MenuItem.DEFAULT_BACKGROUND_COLOR = {
    Normal = {
        R = 0xF2,
        G = 0xF2,
        B = 0xF2
    },
    Focused = {
        R = 0x91,
        G = 0xC9,
        B = 0xF7
    },
    Disabled = {
        R = 0xF2,
        G = 0xF2,
        B = 0xF2
    },
    DisableFocused = {
        R = 0xE6,
        G = 0xE6,
        B = 0xE6
    }
}
MenuItem.DEFAULT_TEXT_COLOR = {
    Normal = {
        R = 0x00,
        G = 0x00,
        B = 0x00
    },
    Focused = {
        R = 0x00,
        G = 0x00,
        B = 0x00
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

---기존 컨트롤을 받아와 MenuItem 클래스를 생성합니다.
---@param control any
---@return MenuItem
function MenuItem:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---Menu Item을 새로 생성합니다.
---@return MenuItem
function MenuItem:New()
    local newControl = {}

    newControl.control = controlManager:CreateMenuItem()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---텍스트를 반환합니다.
---@return string
function MenuItem:Text()
    return self.control:GetText()
end

---활성화 여부를 설정합니다.
---@param value boolean 
function MenuItem:SetEnabled(value)
    if type(value) == "boolean" then
        self.control:SetEnabled(value)
    end
end

---텍스트를 설정합니다.
---@param text string
function MenuItem:SetText(text)
    self.control:SetText(text)
end

---클릭 이벤트를 설정합니다.
---@param event function
function MenuItem:SetClickEvent(event)
    self.control:SetClickEvent(event)
end

---서브 메뉴를 설정합니다
---@param menuControl Menu 
function MenuItem:SetChildMenu(menuControl)
    if menuControl ~= nil then
        self.control:SetChildMenu(menuControl)
    end
end

---메뉴 아이템의 기본 배경 색을 지정합니다.
---@param color integer | RGB 
function MenuItem:SetNormalBackgroundColor(color)
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

---메뉴 아이템에 마우스를 올려 focus 된 배경 색을 지정합니다.
---@param color integer | RGB
function MenuItem:SetFocusedBackgroundColor(color)
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

---메뉴 아이템의 비활성화 된 배경 색을 지정합니다.
---@param color integer | RGB 
function MenuItem:SetDisabledBackgroundColor(color)
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

---비활성화 메뉴 아이템에 마우스를 올려 focus 된 배경 색을 지정합니다.
---@param color integer | RGB
function MenuItem:SetDisableFocusedBackgroundColor(color)
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

---메뉴 아이템의 기본 글자 색을 지정합니다.
---@param color integer | RGB 
function MenuItem:SetNormalTextColor(color)
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

---메뉴 아이템에 마우스를 올려 focus 된 글자 색을 지정합니다.
---@param color integer | RGB
function MenuItem:SetFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.Focused.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.Focused.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.Focused.B
        self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---메뉴 아이템의 비활성화 된 글자 색을 지정합니다.
---@param color integer | RGB 
function MenuItem:SetDisabledTextColor(color)
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

---비활성화 메뉴 아이템에 마우스를 올려 focus 된 글자 색을 지정합니다.
---@param color integer | RGB 
function MenuItem:SetDisableFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.DisableFocused.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.DisableFocused.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.DisableFocused.B
        self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end
