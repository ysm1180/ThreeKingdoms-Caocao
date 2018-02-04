require "Script\\control.lua"

MoviePlayer = Object:Instance
{
    Instance = function(self, parent, fileName)
        local newMoviePlayer = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end
        if fileName == nil then
            fileName = ""
        end

        self.control = controlManager:CreateMoviePlayer(parentControl, fileName)
        setmetatable(newMoviePlayer, self)
        self.__index = self
        
        return newMoviePlayer
    end,

    Create = function(self)
        self.control:Create()

        OUTPUT("Movie Created")
    end,
    
    Destroy = function(self)
        self.control:Destroy()

        OUTPUT("Movie Destroy")
    end,

    WaitForPlay = function(self)
        self.control:WaitForPlay()
    end,

    Play = function(self)
        self.control:Play()

        OUTPUT("Movie Play")
    end,

    Stop = function(self)
        self.control:Stop()

        OUTPUT("Movie Stop")
    end,

    IsPlaying = function(self)
        return self.control:IsPlaying()
    end,

    SetEndEvent = function(self, event)
        self.control:SetEndEvent(event)

        OUTPUT("Set End Event : " .. (event or "NULL"))
    end
}