require "control.lua"
require "text_control.lua"

-- @class Static : 게임 내의 Static(Label) 컨트롤을 Wrapping 하는 클래스. 단순히 텍스트를 표현해주는 클래스.
-- @inherit Control TextControl
Static = Inherit(Control, TextControl)

-- @description 기존 컨트롤을 받아와 Static 클래스를 생성합니다.
-- @param control control : 기존의 Static 컨트롤
-- @return Static : 기존 control 의 wrapping 된 Static 클래스
function Static:Instance(control)
    local newControl = {}
        
    newControl.control = control
    setmetatable(newControl, Static)
    Static.__index = Static

    return newControl
end

-- @description 스태틱 컨트롤을 새로 생성합니다.
-- @param parent Window : 스태틱 컨트롤이 생성될 부모, 보통 Window
-- @return Static : 새로 생성된 Static class
function Static:New(parent)
    local newControl = {}
    local parentControl = nil
    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateStatic(parentControl)
    setmetatable(newControl, Static)
    Static.__index = Static

    return newControl
end

-- @description 텍스트를 설정합니다.
-- @param text string : 텍스트 내용
function Static:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

-- @description 자동 크기 조절 여부를 설정합니다.
-- @param value boolean : 자동 크기 조절 여부
function Static:SetAutoSize(value)
    if type(value) == "boolean" then
        self.control:SetAutoSize(value)
    end
end

-- @description 텍스트 정렬을 설정합니다.
-- @param align int : 정렬, Left - 0, Right - 1, Center - 2
function Static:SetAlign(align)
    if align ~= nil then
        self.control:SetAlign(align)
    end
end


-- @description 배경 투명화 여부를 설정합니다.
-- @param value boolean : 투명화 여부
function Static:SetTransparentBackground(value)
    if type(value) == "boolean" then
        self.control:SetTransparentBackground(value)
    end
end

-- @description 기본 배경 색을 설정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Static:SetBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or 0
        color.G = color.G or 0
        color.B = color.B or 0
        self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    end
end

-- @description 기본 글자 색을 설정합니다.
-- @param color int | table { R int, G int, B int } : 색
function Static:SetTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
        end

        color.R = color.R or 0
        color.G = color.G or 0
        color.B = color.B or 0
        self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end