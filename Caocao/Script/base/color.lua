---@class RGB @R, G, B 값을 나타냅니다.
---@field R number @Red value
---@field G number @Green value
---@field B number @Blue value
RGB = {}

---@param color number @RGB 값
---@return RGB
function numberToRGB(color)
    return {
        R = color % 0x100,
        G = math.floor(color / 0x100) % 0x100,
        B = math.floor(color / 0x10000) % 0x100
    }
end
