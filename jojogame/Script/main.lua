require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"

DEBUG(true)

function main_close() 
    if openningMovie:IsPlaying() then
        openningMovie:Destroy()
    end
    gameManager:Quit()
end

function openning_click()
    openningMovie:Destroy()
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
        Close = "main_close",
        Show = true
    })

    openningMovie = MoviePlayerManager.Create({
        Center = true,
        FileName = "Script\\logo.mkv",
        Parent = main,
        MouseLButtonDown = "openning_click",
        End = "openning_end"
    })
    openningMovie:Play()
    openningMovie:WaitForPlay()

    mainMenuDialog = WindowManager.Create({
        Parent = main,
        Width = 200,
        Height = 142,
        Center = true,
        BackColor = {R = 0xF0, G = 0xF0, B = 0xF0},
        Modal = true,
        Show = true
    })
    
end

main()