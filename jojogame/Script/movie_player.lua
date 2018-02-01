require "Script\\control.lua"

MoviePlayer = Control:Instance
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

    Destroy = function(self)
        self.control:Destroy()
    end,

    WaitForPlay = function(self)
        self.control:WaitForPlay()
    end,

    Play = function(self)
        self.control:Play()
    end,

    Stop = function(self)
        self.control:Stop()
    end,

    IsPlaying = function(self)
        return self.control:IsPlaying()
    end,

    SetEndEvent = function(self, event)
        self.control:SetEndEvent(event)
    end
}