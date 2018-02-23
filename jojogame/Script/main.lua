require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"
require "Script\\button_manager.lua"
require "Script\\menu_manager.lua"

function test()
    FileMenuItem:SetEnabled(false)
end

function quit()
    gameManager:Quit()
end

function main_click()
    openningMovie:Destroy()
end

function main_destroy()
   quit() 
end

function openning_end()
    openningMovie:Destroy()
end

function main()
    local title = "三國志曺操傳"
    local mainSize = {Width = 640, Height = 440}

    FileMenuItem = MenuManager:CreateMenuItem({
        Text = "파일",
        ChildItems = {
            MenuManager:CreateMenuItem({
                Text = "불러오기"
            }),
            MenuManager:CreateMenuItem({
                Text = "저장"
            }),
            MenuManager:CreateMenuItem({
                Text = "-"
            }),
            MenuManager:CreateMenuItem({
                Text = "종료",
                Click = "test"
            })
        }
    })
    mainMenu = MenuManager:CreateMenu({FileMenuItem})

    main = WindowManager:Create({
        Width = mainSize.Width, 
        Height = mainSize.Height, 
        Center = true,
        TitleName = title,
        BackColor = {R = 0, G = 0, B = 0},
        MouseLButtonUp = "main_click",
        Close = "main_close",
        Destroy = "main_destroy",
        Show = true,
        Menu = mainMenu,
    })

    openningMovie = MoviePlayerManager:Create({
        FileName = "Script\\LOGO.avi",
        Parent = main,
        End = "openning_end",
        Center = true,
    })
    --openningMovie:Play()

    mainDialog = WindowManager:Create({
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
        mainDialogButtons[i] = ButtonManager:Create({
            Parent = mainDialog,
            Text = {
                Content = mainDialogButtons.Text[i],
                Color = {
                    Pushed = {R = 0xFF, G = 0x00, B = 0x00}
                }
            },
            Width = 176,
            Height = 30,
            X = 12,
            Y = 11 + (i - 1) * 30,
            Border = {Width = 1},
            Transparent = false,
            Show = true
        })
    end
    --mainDialog:ShowModalWindow()
    
end

main()