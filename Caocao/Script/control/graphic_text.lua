require "control.lua"
require "text_control.lua"
require "color.lua"

---@class GraphicText : Control, ExtendFontControl @게임 내의 GraphicText 컨트롤을 Wrapping 하는 클래스. 텍스트를 레이아웃에 표현해준다.
GraphicText = Class(Control, ExtendFontControl)

---기존 엔진 컨트롤을 기반으로 GraphicText object를 생성합니다.
---@param control any @엔진 GraphicText 컨트롤
---@return GraphicText
function GraphicText:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, GraphicText)
    GraphicText.__index = GraphicText

    return newControl
end

---GrapicText object를 새로 생성합니다.
---@return GraphicText
function GraphicText:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateGraphicText()
    setmetatable(newControl, GraphicText)
    GraphicText.__index = GraphicText

    return newControl
end

---텍스트를 설정합니다.
---@param text string
function GraphicText:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---기본 글자 색을 설정합니다.
---@param color integer | RGB
function GraphicText:SetTextColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or 0
        color.G = color.G or 0
        color.B = color.B or 0
        self.control:SetTextColor(gameManager:Color(color.R, color.G, color.B))
    end
end
