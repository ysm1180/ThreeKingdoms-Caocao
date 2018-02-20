require "Script\\object.lua"

Control = Object:Instance
{
    control = nil,

    Create = function(self)
        OUTPUT("Try Create")
        result = self.control:Create()
        if result then
            OUTPUT("Success create")
        else
            OUTPUT("Fail to create")
        end
    end,

    Move = function(self, x, y)
        if x ~= nil then
            self.control:SetX(x)
        end
        if y ~= nil then
            self.control:SetY(y)
        end

        OUTPUT("Move (" .. (x or "NOT MOVE") .. ", " .. (y or "NOT MOVE") .. ")")        
    end,

    SetSize = function(self, w, h)
        if w ~= nil then
            self.control:SetWidth(w)
        end
        if h ~= nil then
            self.control:SetHeight(h)
        end

        OUTPUT("SetSize (" .. w .. ", " .. h .. ")")        
    end,
    
    Show = function(self)
        self.control:Show()

        OUTPUT("Show")                
    end,

    Position = function(self)
        OUTPUT("Get Position (" .. self.control:GetX() .. ", " .. self.control:GetY() .. ")")
        
        return self.control:GetX(), self.control:GetY()
    end,

    Size = function(self)
        OUTPUT("Get Size (" .. self.control:GetWidth() .. ", " .. self.control:GetHeight() .. ")")
        return self.control:GetWidth(), self.control:GetHeight()
    end,

    SetCreateEvent = function(self, createEventName)
        self.control:SetCreateEvent(createEventName)

        OUTPUT("Set Create Event : " .. (createEventName or "NULL"))
    end,

    SetDestroyEvent = function(self, destroyEventName)
        self.control:SetDestroyEvent(destroyEventName)

        OUTPUT("Set Destroy Event : " .. (destroyEventName or "NULL"))
    end,

    SetMouseLButtonUpEvent = function(self, mouseLButtonUpEventName)
        self.control:SetMouseLButtonUpEvent(mouseLButtonUpEventName)

        OUTPUT("Set MouseLButtonUp Event : " .. (mouseLButtonUpEventName or "NULL"))
    end,

    SetMouseLButtonDownEvent = function(self, mouseLButtonDownEventName)
        self.control:SetMouseLButtonDownEvent(mouseLButtonDownEventName)
        
        OUTPUT("Set MouseLButtonDown Event : " .. (mouseLButtonDownEventName or "NULL"))
    end,

    SetMouseMoveEvent = function(self, mouseMoveEventName)
        self.control:SetMouseMoveEvent(mouseMoveEventName)

        OUTPUT("Set MouseMove Event : " .. (mouseMoveEventName or "NULL"))
    end,

    SetMouseHoverEvent = function(self, mouseHoverEventName)
        self.control:SetMouseHoverEvent(mouseHoverEventName)

        OUTPUT("Set MouseHover Event : " .. (mouseHoverEventName or "NULL"))
    end,

    SetMouseLeaveEvent = function(self, mouseLeaveEventName)
        self.control:SetMouseLeaveEvent(mouseLeaveEventName)

        OUTPUT("Set MouseLeave Event : " .. (mouseLeaveEventName or "NULL"))
    end
}