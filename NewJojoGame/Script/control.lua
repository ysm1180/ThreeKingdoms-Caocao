require "object.lua"

Control = Object:Instance
{
    Create = function(self)
        self.window:Create()
    end,

    Move = function(self, x, y)
        if x ~= nil then
            self.window:SetX(x)
        end
        if y ~= nil then
            self.window:SetY(y)
        end
    end,

    SetSize = function(self, w, h)
        if w ~= nil then
            self.window:SetWidth(w)
        end
        if h ~= nil then
            self.window:SetHeight(h)
        end
    end,
    
    Show = function(self)
        self.window:Show()
    end,
}