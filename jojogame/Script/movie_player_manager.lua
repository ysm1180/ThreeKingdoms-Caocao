require "Script\\control_manager.lua"
require "Script\\movie_player.lua"

MoviePlayerManager = ControlManager:Instance
{
    Create = function(options)
        OUTPUT("-------- Start Create : Movie Player --------")

        local newMoviePlayer = MoviePlayer:Instance(options.Parent, options.FileName)

        newMoviePlayer:SetEndEvent(options.End)
        newMoviePlayer:SetPosition(options.X, options.Y)
        newMoviePlayer:Create()

        if options.Center then
            local movieWidth, movieHeight = newMoviePlayer:Size()
            if options.Parent then
                local parentWidth, parentHeight = options.Parent:Size()
                newMoviePlayer:SetPosition(parentWidth / 2 - movieWidth / 2, parentHeight / 2 - movieHeight / 2)
            end
        end

        OUTPUT("-------- End Create : Movie Player --------")

        return newMoviePlayer
    end,
}