require "control.lua"

-- @class TextControl
-- @description 폰트를 가지고 있는 Control 의 속성을 가지고 있다.
-- @inherit ControlObject
TextControl = ControlObject:Instance
{
    -- @description 폰트 크기를 반환합니다.
    -- @return int : 폰트 크기
    FontSize = function(self)
        return self.control:GetFont():GetFontSize()
    end,

    -- @description 폰트 이름을 반환합니다.
    -- @return string : 폰트 이름
    FontName = function(self)
        return self.control:GetFont():GetFontName()
    end,

    -- @description 폰트 Bold 여부를 반환합니다.
    -- @return boolean : 폰트 bold 여부
    Bold = function(self)
        return self.control:GetFont():IsBold()
    end,

    -- @description 폰트 Underline 여부를 반환합니다.
    -- @return boolean : 폰트 Underline 여부
    Underline = function(self)
        return self.control:GetFont():IsUnderline()
    end,

    -- @description 폰트 Italic 여부를 반환합니다.
    -- @return boolean : 폰트 Italic 여부
    Italic = function(self)
        return self.control:GetFont():IsItalic()
    end,

    -- @description 폰트 크기를 설정합니다.
    -- @param size int : 폰트 크기
    SetFontSize = function(self, size)
        if size ~= nil then
            self.control:GetFont():SetFontSize(size)
        end        
    end,

    -- @description 폰트 이름을 설정합니다.
    -- @param name string : 폰트 이름
    SetFontName = function(self, name)
        if name ~= nil then
            self.control:GetFont():SetFontName(name)
        end
    end,

    -- @description 폰트 Bold 를 설정합니다.
    -- @param value boolean : 폰트 Bold 여부
    SetBold = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetBold(value)
        end
    end,

    -- @description 폰트 Underline 을 설정합니다.
    -- @param value boolean : 폰트 Underline 여부
    SetUnderline = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetUnderline(value)
        end
    end,

    -- @description 폰트 Itailc 을 설정합니다.
    -- @param value boolean : 폰트 Itailc 여부
    SetItalic = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetItalic(value)
        end
    end,        


}