require "color.lua"
require "child_control.lua"
require "text_control.lua"

---@class ButtonColor @버튼 색을 표현하는 옵션
---@field Normal RGB @기본 색
---@field Focused RGB @버튼이 강조됐을 때의 색
---@field Pushed RGB @버튼이 눌렸을 때의 색

---@class Button : ChildControl, ExtendFontControl  @게임 내의 Button 컨트롤을 Wrapping 하는 클래스
---@field control EngineButtonControl
Button = Class(ChildControl, ExtendFontControl)

---기본 배경색
---@type ButtonColor
Button.DEFAULT_BACKGROUND_COLOR = {
    Normal = {
        R = 0xE1,
        G = 0xE1,
        B = 0xE1
    },
    Focused = {
        R = 0x00,
        G = 0x78,
        B = 0xD7
    },
    Pushed = {
        R = 0x00,
        G = 0x54,
        B = 0x99
    }
}

---기본 테두리 색
---@type ButtonColor
Button.DEFAULT_BORDER_COLOR = {
    Normal = {
        R = 0xAD,
        G = 0xAD,
        B = 0xAD
    },
    Focused = {
        R = 0x00,
        G = 0x78,
        B = 0xD7
    },
    Pushed = {
        R = 0x00,
        G = 0x54,
        B = 0x99
    }
}

---기본 텍스트 색
---@type ButtonColor
Button.DEFAULT_TEXT_COLOR = {
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
    Pushed = {
        R = 0x00,
        G = 0x00,
        B = 0x00
    }
}

---버튼을 새로 생성합니다.
---@param parent Window @버튼이 생성될 부모, 보통 Window
---@return Button @새로 생성된 Button class
function Button:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    ---@type EngineButtonControl
    newControl.control = controlManager:CreateButton(parentControl)
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---버튼의 텍스트를 설정합니다.
---@param text string
function Button:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---버튼의 배경을 투명화할건지 설정합니다.
---@param value boolean
function Button:SetTransparentBackground(value)
    if type(value) == "boolean" then
        self.control:SetTransparentBackground(value)
    end
end

---버튼의 테두리 선 굵기를 설정합니다.
---@param width number
function Button:SetBorderWidth(width)
    if width ~= nil then
        self.control:SetBorderWidth(width)
    end
end

---버튼의 기본 배경 색을 지정합니다.
---@param color integer | RGB
function Button:SetNormalBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Normal.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Normal.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Normal.B
        self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼에 마우스가 올라가 focus 됐을 때의 배경 색을 지정합니다.
---@param color integer | RGB
function Button:SetFocusedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Focused.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Focused.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Focused.B
        self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼이 눌렸을 때의 배경 색을 지정합니다.
---@param color integer | RGB
function Button:SetPushedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.Pushed.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.Pushed.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.Pushed.B
        self.control:SetPushedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼의 기본 테두리 색을 지정합니다.
---@param color integer | RGB
function Button:SetNormalBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.Normal.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.Normal.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.Normal.B
        self.control:SetBorderColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼에 마우스가 올라가 focus 됐을 때의 테두리 색을 지정합니다.
---@param color integer | RGB
function Button:SetFocusedBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.Focused.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.Focused.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.Focused.B
        self.control:SetFocusedBorderColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼이 눌렸을 때의 테두리 색을 지정합니다.
---@param color integer | RGB
function Button:SetPushedBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.Pushed.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.Pushed.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.Pushed.B
        self.control:SetPushedBorderColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼의 기본 텍스트 색을 지정합니다.
---@param color integer | RGB
function Button:SetNormalTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.Normal.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.Normal.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.Normal.B
        self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼에 마우스가 올라가 focus 됐을 때의 텍스트 색을 지정합니다.
---@param color integer | RGB
function Button:SetFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.Focused.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.Focused.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.Focused.B
        self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

---버튼이 눌렸을 때의 텍스트 색을 지정합니다.
---@param color integer | RGB
function Button:SetPushedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = numberToRGB(color)
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.Pushed.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.Pushed.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.Pushed.B
        self.control:SetPushedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end
