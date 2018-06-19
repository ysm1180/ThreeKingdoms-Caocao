require "control.lua"
require "text_control.lua"

-- @class Button : 게임 내의 Button 컨트롤을 Wrapping 하는 클래스
-- @inherit Control TextControl
Button = Inherit(Control, TextControl)

-- 기본 색 지정
Button.DEFAULT_BACKGROUND_COLOR = {
    NORMAL = {R = 0xE1, G = 0xE1, B = 0xE1},
    FOCUSED = {R = 0x00, G = 0x78, B = 0xD7},
    PUSHED = {R = 0x00, G = 0x54, B = 0x99}
}
Button.DEFAULT_BORDER_COLOR = {
    NORMAL = {R = 0xAD, G = 0xAD, B = 0xAD},
    FOCUSED = {R = 0x00, G = 0x78, B = 0xD7},
    PUSHED = {R = 0x00, G = 0x54, B = 0x99}
}
Button.DEFAULT_TEXT_COLOR = {
    NORMAL = {R = 0x00, G = 0x00, B = 0x00},
    FOCUSED = {R = 0x00, G = 0x00, B = 0x00},
    PUSHED = {R = 0x00, G = 0x00, B = 0x00}
}

-- @description 기존 컨트롤을 받아와 Button 클래스를 생성합니다.
-- @param control control : 기존의 button 컨트롤
-- @return Button : 기존 control 의 wrapping 된 Button 클래스
function Button:Instance(control)
    local newControl = {}
        
    newControl.control = control
    setmetatable(newControl, Button)
    Button.__index = Button

    return newControl
end

-- @description 버튼을 새로 생성합니다.
-- @param parent Window : 버튼이 생성될 부모, 보통 Window
-- @return Button : 새로 생성된 Button class
function Button:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateButton(parentControl)
    setmetatable(newControl, Button)
    Button.__index = Button

    return newControl
end

-- @description 버튼의 텍스트를 설정합니다.
-- @param text string : 텍스트
function Button:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

-- @description 버튼의 배경을 투명화할건지 설정합니다.
-- @param value boolean : 투명화 여부
function Button:SetTransparentBackground(value)
    if type(value) == "boolean" then
        self.control:SetTransparentBackground(value)
    end
end

-- @description 버튼의 테두리 선 굵기를 설정합니다.
-- @param width int : 테두리 선 굵기
function Button:SetBorderWidth(width)
    if width ~= nil then
        self.control:SetBorderWidth(width)
    end
end


-- @description 버튼의 기본 배경 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetNormalBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
        self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼에 마우스가 올라가 focus 됐을 때의 배경 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetFocusedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
        self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼이 눌렸을 때의 배경 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetPushedBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.PUSHED.R
        color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.PUSHED.G
        color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.PUSHED.B
        self.control:SetPushedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼의 기본 테두리 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetNormalBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.NORMAL.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.NORMAL.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.NORMAL.B
        self.control:SetBorderColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼에 마우스가 올라가 focus 됐을 때의 테두리 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetFocusedBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.FOCUSED.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.FOCUSED.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.FOCUSED.B
        self.control:SetFocusedBorderColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼이 눌렸을 때의 테두리 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetPushedBorderColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_BORDER_COLOR.PUSHED.R
        color.G = color.G or self.DEFAULT_BORDER_COLOR.PUSHED.G
        color.B = color.B or self.DEFAULT_BORDER_COLOR.PUSHED.B
        self.control:SetPushedBorderColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼의 기본 텍스트 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetNormalTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
        self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼에 마우스가 올라가 focus 됐을 때의 텍스트 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetFocusedTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.FOCUSED.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.FOCUSED.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.FOCUSED.B
        self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 버튼이 눌렸을 때의 텍스트 색을 지정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Button:SetPushedTextColor( color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or self.DEFAULT_TEXT_COLOR.PUSHED.R
        color.G = color.G or self.DEFAULT_TEXT_COLOR.PUSHED.G
        color.B = color.B or self.DEFAULT_TEXT_COLOR.PUSHED.B
        self.control:SetPushedTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end
