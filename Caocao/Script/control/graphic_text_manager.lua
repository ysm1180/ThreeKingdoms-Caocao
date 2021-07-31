require "graphic_text.lua"

---@class GraphicTextOptions
---@field _Control any @해당 값이 있으면 모든 옵션은 무시되고 해당 엔진 컨트롤로 object를 생성한다.
---@field Text string
---@field Font FontOption

---GraphicText object를 관리하는 class
---@class GraphicTextMananger
GraphicTextManager = {}

---GraphicText object를 생성합니다.
---@param options GraphicTextOptions
---@return GraphicText
function GraphicTextManager:Create(options)
    local newControl = nil
    if options ~= nil then
        if options.Control ~= nil then
            return GraphicText:Instance(options.Control)
        end

        newControl = GraphicText:New()

        -- 텍스트 설정
        if options.Font ~= nil then
            newControl:SetFontName(options.Font.Name)
            newControl:SetFontSize(options.Font.Size)
            newControl:SetBold(options.Font.Bold)
            newControl:SetUnderline(options.Font.Underline)
            newControl:SetItalic(options.Font.Italic)
        end
        newControl:SetText(options.Text)
        newControl:SetTextColor(options.Color)
    end

    return newControl
end

---엔진 GraphicText 컨트롤을 기반으로 GraphicText object 생성
---@param control any @Base EngineGraphicText control object
---@return GraphicText
function GraphicTextManager:CreateInstance(control)
    local options = {
        Control = control
    }

    return self:Create(options)
end
