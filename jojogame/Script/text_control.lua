require "Script\\control.lua"

TextControl = Control:Instance
{
    DEFAULT_FONT_SIZE = 12,
    DEFAULT_FONT_NAME = "굴림",

    FontSize = function(self)
        return self.control:GetFont():GetFontSize()
    end,

    FontName = function(self)
        return self.control:GetFont():GetFontName()
    end,

    Bold = function(self)
        return self.control:GetFont():IsBold()
    end,

    Underline = function(self)
        return self.control:GetFont():IsUnderline()
    end,

    Italic = function(self)
        return self.control:GetFont():IsItalic()
    end,

    SetFontSize = function(self, size)
        if size then
            self.control:GetFont():SetFontSize(size)
        else
            self.control:GetFont():SetFontSize(DEFAULT_FONT_SIZE)
        end

        OUTPUT("Set Font Size : " .. (size or "10"))
    end,

    SetFontName = function(self, name)
        self.control:GetFont():SetFontName(name)
    end,

    SetBold = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetBold(value)
        else
            self.control:GetFont():SetBold(false)
        end
    end,

    SetUnderline = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetUnderline(value)
        else
            self.control:GetFont():SetUnderline(false)
        end
    end,

    SetItalic = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetItalic(value)
        else
            self.control:GetFont():SetItalic(false)
        end
    end,        


}