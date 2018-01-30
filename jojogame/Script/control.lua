require "Script\\object.lua"

Control = Object:Instance
{
    control = nil,

    Create = function(self)
        self.control:Create()
    end,

    Move = function(self, x, y)
        if x ~= nil then
            self.control:SetX(x)
        end
        if y ~= nil then
            self.control:SetY(y)
        end
    end,

    SetSize = function(self, w, h)
        if w ~= nil then
            self.control:SetWidth(w)
        end
        if h ~= nil then
            self.control:SetHeight(h)
        end
    end,
    
    Show = function(self)
        self.control:Show()
    end,

    Position = function(self)
        return self.control:GetX(), self.control:GetY()
    end,

    Size = function(self)
        return self.control:GetWidth(), self.control:GetHeight()
    end,

    SetCreateEvent = function(self, createEventName)
        self.control:SetCreateEvent(createEventName)
    end,

    SetDestroyEvent = function(self, destroyEventName)
        self.control:SetDestroyEvent(destroyEventName)
    end,

    SetClickEvent = function(self, clickEventName)
        self.control:SetClickEvent(clickEventName)
    end
}