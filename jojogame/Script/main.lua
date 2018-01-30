require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"


function main_close() 
    gameManager:Quit()
end

local title = "三國志曺操傳"

main = WindowManager.Create({
    Width = 640, 
    Height = 440, 
    Center = true,
    TitleName = title,
    BackColor = {R = 0, G = 0, B = 0},
    CloseEvent = "main_close"
})
main:Show()

openningMovie = MoviePlayerManager.Create({
    Center = true,
    FileName = "Script\\logo.avi",
    Parent = main
})
openningMovie:Play()
