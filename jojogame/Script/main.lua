require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"
require "Script\\button_manager.lua"


function main_click()
    openningMovie:Destroy()
end

function main_destroy()
    gameManager:Quit()
end

function openning_end()
    openningMovie:Destroy()
end

function main()
    local title = "三國志曺操傳"
    local mainSize = {Width = 640, Height = 440}

    main = WindowManager.Create({
        Width = mainSize.Width, 
        Height = mainSize.Height, 
        Center = true,
        TitleName = title,
        BackColor = {R = 0, G = 0, B = 0},
        MouseLButtonUp = "main_click",
        Close = "main_close",
        Destroy = "main_destroy",
        Show = true
    })

    openningMovie = MoviePlayerManager.Create({
        FileName = "Script\\LOGO.avi",
        Parent = main,
        End = "openning_end",
        Center = true,
    })
    --openningMovie:Play()

    mainDialog = WindowManager.Create({
        Parent = main,
        Width = 200, 
        Height = 142, 
        TitleName = title,
        ControlBox = false,
        MinButton = false,
        MaxButton = false,
        BackColor = {R = 0xF0, G = 0xF0, B = 0xF0},
        Center = true,    
        Modal = true,    
    })

    mainDialogButtons = {Text = {"새로운 게임을 시작한다", "저장 데이터를 불러온다", "환경 설정", "게임 종료"}}
    for i = 1, 4 do
        mainDialogButtons[i] = ButtonManager.Create({
            Parent = mainDialog,
            Text = mainDialogButtons.Text[i],
            Width = 176,
            Height = 30,
            X = 12,
            Y = 11 + (i - 1) * 30,
            Border = {Width = 1},
            Transparent = false,
            Show = true
        })
    end
    mainDialog:ShowModalWindow()
    
end

main()