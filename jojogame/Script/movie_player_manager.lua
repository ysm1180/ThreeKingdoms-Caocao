require "Script\\control_manager.lua"
require "Script\\movie_player.lua"

MoviePlayerManager = ControlManager:Instance
{
    Create = function(options)
        OUTPUT("-------- Start Create : Movie Player --------")

        local newMoviePlayer = MoviePlayer:Instance(options.Parent, options.FileName)

        newMoviePlayer:SetEndEvent(options.End)
        newMoviePlayer:Create()

        OUTPUT("-------- End Create : Movie Player --------")

        return newMoviePlayer
    end,
}