require "../base/object.lua"

ControlObject = Object:Instance
{
    control = nil,
}

Control = ControlObject:Instance
{
    Create = function(self)
        result = self.control:Create()
        return result
    end,

    Destroy = function(self)
        self.control:Destroy()
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

    Hide = function(self)
        self.control:Hide()
    end,

    Position = function(self)
        return self.control:GetX(), self.control:GetY()
    end,

    Width = function(self)
        return self.control:GetWidth()
    end,

    Height = function(self)
        return self.control:GetHeight()
    end,

    Size = function(self)
        return self.control:GetWidth(), self.control:GetHeight()
    end,

    UserData = function(self)
        return self.control:GetUserData()
    end,

    SetCreateEvent = function(self, event)
        if event ~= nil then
            self.control:SetCreateEvent(event)
        end
    end,

    SetDestroyEvent = function(self, event)
        if event ~= nil then            
            self.control:SetDestroyEvent(event)
        end
    end,

    SetMouseLButtonUpEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseLButtonUpEvent(event)
        end
    end,

    SetMouseLButtonDownEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseLButtonDownEvent(event)
        end
    end,

    SetMouseMoveEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseMoveEvent(event)
        end
    end,

    SetMouseEnterEvent = function(self, event)
        if event ~= nil then        
            self.control:SetMouseEnterEvent(event)
        end
    end,

    SetMouseLeaveEvent = function(self, event)
        if event ~= nil then
            self.control:SetMouseLeaveEvent(event)
        end
    end,

    SetUserData = function(self, data)
        if data ~= nil then
            self.control:SetUserData(data)
        end
    end,

    Refresh = function(self)
        self.control:Refresh()
    end,

    RefreshRegion = function(self, rect)
        if rect ~= nil then
            self.control:RefreshRegion(rect.Left, rect.Top, rect.Right, rect.Bottom)
        end
    end,
}