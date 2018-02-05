require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"

DEBUG(true)

function main_click()
    if openningMovie:IsPlaying() then
        openningMovie:Destroy()
    end
end

function main_close() 
    if openningMovie:IsPlaying() then
        openningMovie:Destroy()
    end
    gameManager:Quit()
end

function openning_end()
    openningMovie:Destroy()
end

function main()
    local title = "三國志曺操傳"
    main = WindowManager.Create({
        Width = 640, 
        Height = 440, 
        Center = true,
        TitleName = title,
        BackColor = {R = 0, G = 0, B = 0},
        MouseLButtonUp = "main_click",
        Close = "main_close",
        Show = true
    })

    openningMovie = MoviePlayerManager.Create({
        FileName = "Script\\logo.avi",
        Parent = main,
        End = "openning_end"
    })
    openningMovie:Play()
    openningMovie:WaitForPlay()

    
    
end

main()