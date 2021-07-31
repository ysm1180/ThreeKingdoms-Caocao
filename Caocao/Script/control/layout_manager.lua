require "layout.lua"

---@class LayoutRatioOption
LayoutRatioOption = {
    X = 1.0,
    Y = 1.0
}

---@class LayoutCreateOption
LayoutCreateOption = {
    Width = 0,
    Height = 0,
    X = 0,
    Y = 0,
    Ratio = LayoutRatioOption
}

---@class LayoutManager @Layout object를 관리하는 Manager class
LayoutManager = {}

---@param options LayoutCreateOption
---@return Layout
function LayoutManager:CreateLayout(options)
    local newControl = nil

    if options ~= nil then
        newControl = Layout:New()

        -- 위치 설정
        newControl:Move(options.X, options.Y)

        -- 크기 설정
        newControl:SetWidth(options.Width)
        newControl:SetHeight(options.Height)

        -- 비율 설정
        if options.Ratio ~= nil then
            newControl:SetRatioX(options.Ratio.X)
            newControl:SetRatioY(options.Ratio.Y)
        end
    end
    return newControl
end

