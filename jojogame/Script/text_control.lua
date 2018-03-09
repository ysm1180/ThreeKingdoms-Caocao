require "Script\\control.lua"

TextControl = ControlObject:Instance
{
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
        if size ~= nil then
            self.control:GetFont():SetFontSize(size)

            OUTPUT("Set Font Size : " .. size)
        end        
    end,

    SetFontName = function(self, name)
        if name ~= nil then
            self.control:GetFont():SetFontName(name)
    
            OUTPUT("Set Font Name : " .. name)        
        end
    end,

    SetBold = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetBold(value)
        end
    end,

    SetUnderline = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetUnderline(value)
        end
    end,

    SetItalic = function(self, value)
        if type(value) == "boolean" then        
            self.control:GetFont():SetItalic(value)
        end
    end,        


}