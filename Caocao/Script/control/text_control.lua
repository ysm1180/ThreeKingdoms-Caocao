require "control.lua"

---@class ExtendFontControl @폰트를 가지고 있는 Control
ExtendFontControl = {}

---폰트 크기를 반환합니다.
---@return integer 
function ExtendFontControl:FontSize()
    return ExtendFontControl.control:GetFont():GetFontSize()
end

---폰트 이름을 반환합니다.
---@return string 
function ExtendFontControl:FontName()
    return ExtendFontControl.control:GetFont():GetFontName()
end

---폰트 Bold 여부를 반환합니다.
---@return boolean 
function ExtendFontControl:Bold()
    return ExtendFontControl.control:GetFont():IsBold()
end

---폰트 Underline 여부를 반환합니다.
---@return boolean
function ExtendFontControl:Underline()
    return ExtendFontControl.control:GetFont():IsUnderline()
end

---폰트 Italic 여부를 반환합니다.
---@return boolean
function ExtendFontControl:Italic()
    return ExtendFontControl.control:GetFont():IsItalic()
end

---폰트 크기를 설정합니다.
---@param size integer
function ExtendFontControl:SetFontSize(size)
    if size ~= nil then
        ExtendFontControl.control:GetFont():SetFontSize(size)
    end
end

---폰트 이름을 설정합니다.
---@param name string 
function ExtendFontControl:SetFontName(name)
    if name ~= nil then
        ExtendFontControl.control:GetFont():SetFontName(name)
    end
end

---폰트 Bold 를 설정합니다.
---@param value boolean 
function ExtendFontControl:SetBold(value)
    if type(value) == "boolean" then
        ExtendFontControl.control:GetFont():SetBold(value)
    end
end

---폰트 Underline 을 설정합니다.
---@param value boolean 
function ExtendFontControl:SetUnderline(value)
    if type(value) == "boolean" then
        ExtendFontControl.control:GetFont():SetUnderline(value)
    end
end

---폰트 Itailc 을 설정합니다.
---@param value boolean
function ExtendFontControl:SetItalic(value)
    if type(value) == "boolean" then
        ExtendFontControl.control:GetFont():SetItalic(value)
    end
end

