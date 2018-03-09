require "Script\\control_manager.lua"
require "Script\\movie_player.lua"

MoviePlayerManager = ControlManager:Instance
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : Movie Player --------")

        local newMoviePlayer = nil
        
        if options ~= nil then
            newMoviePlayer = MoviePlayer:Instance(options.Parent, options.FileName)

            newMoviePlayer:SetEndEvent(options.End)
            newMoviePlayer:Move(options.X, options.Y)
            newMoviePlayer:Create()

            if options.Center then
                local movieWidth, movieHeight = newMoviePlayer:Size()
                if options.Parent then
                    local parentWidth, parentHeight = options.Parent:Size()
                    newMoviePlayer:Move(parentWidth / 2 - movieWidth / 2, parentHeight / 2 - movieHeight / 2)
                end
            end
        end

        OUTPUT("-------- End Create : Movie Player --------")

        return newMoviePlayer
    end,
}