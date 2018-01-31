require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"

function main_close() 
    if openningMovie:IsPlaying() then
        openningMovie:Destroy()
    end
    gameManager:Quit()
end

function openning_click()
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
        Close = "main_close"
    })
    main:Show()

    openningMovie = MoviePlayerManager.Create({
        Center = true,
        FileName = "Script\\logo.avi",
        Parent = main,
        MouseLButtonDown = "openning_click"
    })
    openningMovie:Play()
    openningMovie:WaitForPlay()

    mainMenuDialog = WindowManager.Create({
        Parent = main,
        
    })
end

main()