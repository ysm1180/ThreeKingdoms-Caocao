require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"
require "Script\\button_manager.lua"
require "Script\\menu_manager.lua"
require "Script\\toolbar_manager.lua"
require "Script\\image_manager.lua"
require "Script\\layout_manager.lua"

isClosing = false

function quit()
    isClosing = true
    main:Close()
end

function main_click()
    if openningMovie:IsPlaying() then
        openningMovie:Destroy()
    end
    if isClosing then
        gameManager:StopDelay()
    end
end

function main_close()
    exitBackgroundImage =
        ImageManager:CreateImage(
        {
            FilePath = "Script\\logo.me5",
            Group = {
                Main = 0,
                Sub = 1
            },
            MaskColor = 0xF700FF
        }
    )
    exitBackgroundLayout = LayoutManager:CreateLayout({})
    exitBackgroundLayout:AddImage(exitBackgroundImage, 0, toolbar:Height())

    main:DeleteLayout(backgroundLayout)  
    main:AddLayout(exitBackgroundLayout)

    gameManager:Delay(3000)
end

function main_destroy()
    gameManager:Quit()
end

function openning_end()
    openningMovie:Destroy()
end

function buttonLoad_click()
    local loadDialog =
        WindowManager:Create(
        {
            Parent = main,
            Width = 200,
            Height = 142,
            TitleName = "불러오기",
            MinButton = false,
            MaxButton = false,
            BackColor = {R = 0xF0, G = 0xF0, B = 0xF0},
            Center = true,
            Modal = true,
            Show = true,
        }
    )
end

function mainButton_hover(self)
    local button = ButtonManager:CreateInstance(self)
    local index = tonumber(button:UserData())

    backgroundLayout:DeleteImage(mainDialogButtonImages[index * 2 - 1])
    backgroundLayout:AddImage(mainDialogButtonImages[index * 2], 420, 68 + (index - 1) * 70)
    main:RefreshRegion(
        {
            Left = 420,
            Top = 68 + (index - 1) * 70,
            Right = 420 + mainDialogButtonImages[index * 2]:Width(),
            Bottom = 68 + (index - 1) * 70 + mainDialogButtonImages[index * 2]:Height()
        }
    )
end

function mainButton_leave(self)
    local button = ButtonManager:CreateInstance(self)
    local index = tonumber(button:UserData())

    backgroundLayout:DeleteImage(mainDialogButtonImages[index * 2])
    backgroundLayout:AddImage(mainDialogButtonImages[index * 2 - 1], 420, 68 + (index - 1) * 70)
    main:RefreshRegion(
        {
            Left = 420,
            Top = 68 + (index - 1) * 70,
            Right = 420 + mainDialogButtonImages[index * 2 - 1]:Width(),
            Bottom = 68 + (index - 1) * 70 + mainDialogButtonImages[index * 2 - 1]:Height()
        }
    )
end

function main()
    local title = "三國志曺操傳"
    local mainSize = {Width = 640, Height = 400}

    mainMenu =
        MenuManager:CreateMenu(
        {
            MenuManager:CreateMenuItem(
                {
                    Text = {
                        Content = "파일"
                    },
                    ChildItems = {
                        MenuManager:CreateMenuItem(
                            {
                                Text = {
                                    Content = "불러오기"
                                },
                                Enabled = false
                            }
                        ),
                        MenuManager:CreateMenuItem(
                            {
                                Text = {
                                    Content = "저장"
                                }
                            }
                        ),
                        MenuManager:CreateMenuItem(
                            {
                                Seperator = true
                            }
                        ),
                        MenuManager:CreateMenuItem(
                            {
                                Text = {
                                    Content = "종료"
                                },
                                Click = "quit"
                            }
                        )
                    }
                }
            )
        }
    )

    backgroundImage =
        ImageManager:CreateImage(
        {
            FilePath = "Script\\logo.me5",
            Group = {
                Main = 0,
                Sub = 0
            },
            MaskColor = 0xF700FF
        }
    )
    backgroundLayout = LayoutManager:CreateLayout({})

    main =
        WindowManager:Create(
        {
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
            Layouts = {
                backgroundLayout
            }
        }
    )

    exitToolbarButton = ToolbarManager:CreateToolbarButton(
        {
            Text = {
                Tooltip = "종료"
            },
            Click = "quit",
            Image = ImageManager:CreateImage(
                {
                    FilePath = "Script\\logo.me5",
                    Group = {
                        Main = 1,
                        Sub = 0
                    },
                    MaskColor = 0xC0C0C0
                }
            ),
            Enabled = false,
        }
    )

    toolbar =
        ToolbarManager:CreateToolbar(
        {
            Parent = main,
            Image = {
                Width = 24,
                Height = 28
            },
            Buttons = {
                exitToolbarButton,            
                ToolbarManager:CreateToolbarButton(
                    {
                        Text = {
                            Tooltip = "저장"
                        },
                        Click = "",
                        Image = ImageManager:CreateImage(
                            {
                                FilePath = "Script\\logo.me5",
                                Group = {
                                    Main = 1,
                                    Sub = 1
                                },
                                MaskColor = 0xC0C0C0
                            }
                        ),
                        Enabled = false,
                    }
                ),
                ToolbarManager:CreateToolbarButton(
                    {
                        Text = {
                            Tooltip = "불러오기"
                        },
                        Click = "",
                        Image = ImageManager:CreateImage(
                            {
                                FilePath = "Script\\logo.me5",
                                Group = {
                                    Main = 1,
                                    Sub = 2
                                },
                                MaskColor = 0xC0C0C0
                            }
                        ),
                        Enabled = false,
                    }
                ),
            },
            Show = true
        }
    )

    main:SetSize(nil, main:Height() + toolbar:Height())

    openningMovie =
        MoviePlayerManager:Create(
        {
            FileName = "Script\\LOGO.avi",
            Parent = main,
            End = "openning_end",
            Center = true
        }
    )
    openningMovie:Play()

    exitToolbarButton:Enable()
    backgroundLayout:AddImage(backgroundImage, 0, toolbar:Height())
    main:Refresh()

    local dialog = false
    if dialog then
        mainDialog =
            WindowManager:Create(
            {
                Parent = main,
                Width = 200,
                Height = 142,
                TitleName = title,
                ControlBox = false,
                MinButton = false,
                MaxButton = false,
                BackColor = {R = 0xF0, G = 0xF0, B = 0xF0},
                Center = true,
                Modal = true
            }
        )

        local mainDialogButtons = {
            Text = {"새로운 게임을 시작한다", "저장 데이터를 불러온다", "환경 설정", "게임 종료"},
            MouseLButtonUp = {"", "buttonLoad_click", "", "quit"}
        }
        for i = 1, 4 do
            mainDialogButtons[i] =
                ButtonManager:Create(
                {
                    Parent = mainDialog,
                    Text = {
                        Content = mainDialogButtons.Text[i],
                        Color = {
                            Pushed = {R = 0xFF, G = 0x00, B = 0x00}
                        },
                        Font = {}
                    },
                    Width = 176,
                    Height = 30,
                    X = 12,
                    Y = 11 + (i - 1) * 30,
                    Border = {Width = 1},
                    Transparent = false,
                    Show = true,
                    MouseLButtonUp = mainDialogButtons.MouseLButtonUp[i]
                }
            )
        end
        mainDialog:ShowModalWindow()
    else
        mainDialogButtonImages = {}
        for i = 1, 8 do
            mainDialogButtonImages[i] =
                ImageManager:CreateImage(
                {
                    FilePath = "Script\\logo.me5",
                    Group = {
                        Main = 2,
                        Sub = i - 1,
                    },
                    MaskColor = 0xF700FF
                }
            )
        end

        for i = 0, 550, 10 do
            if i <= 220 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[1])
                backgroundLayout:AddImage(mainDialogButtonImages[1], 640 - i, 68)
                main:RefreshRegion(
                    {
                        Left = 640 - i,
                        Top = 68,
                        Right = 640 - i + mainDialogButtonImages[1]:Width() + 10,
                        Bottom = 68 + mainDialogButtonImages[1]:Height()
                    }
                )
            end

            if i > 110 and i <= 330 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[3])
                backgroundLayout:AddImage(mainDialogButtonImages[3], 640 - (i - 110), 138)
                main:RefreshRegion(
                    {
                        Left = 640 - (i - 110),
                        Top = 138,
                        Right = 640 - (i - 110) + mainDialogButtonImages[3]:Width() + 10,
                        Bottom = 138 + mainDialogButtonImages[3]:Height()
                    }
                )
            end

            if i > 220 and i <= 440 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[5])
                backgroundLayout:AddImage(mainDialogButtonImages[5], 640 - (i - 220), 208)
                main:RefreshRegion(
                    {
                        Left = 640 - (i - 220),
                        Top = 208,
                        Right = 640 - (i - 220) + mainDialogButtonImages[5]:Width() + 10,
                        Bottom = 208 + mainDialogButtonImages[5]:Height()
                    }
                )
            end

            if i > 330 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[7])
                backgroundLayout:AddImage(mainDialogButtonImages[7], 640 - (i - 330), 278)
                main:RefreshRegion(
                    {
                        Left = 640 - (i - 330),
                        Top = 278,
                        Right = 640 - (i - 330) + mainDialogButtonImages[7]:Width() + 10,
                        Bottom = 278 + mainDialogButtonImages[7]:Height()
                    }
                )
            end

            gameManager:Delay(5)
        end

        local mainDialogButtons = {
            MouseLButtonUp = {"", "buttonLoad_click", "", "quit"},
            MouseHover = {"mainButton_hover", "mainButton_hover", "mainButton_hover", "mainButton_hover"},
            MouseLeave = {"mainButton_leave", "mainButton_leave", "mainButton_leave", "mainButton_leave"}
        }
        for i = 1, 4 do
            mainDialogButtons[i] =
                ButtonManager:Create(
                {
                    Parent = main,
                    Width = 176,
                    Height = 44,
                    X = 420,
                    Y = 68 + (i - 1) * 70,
                    Border = {Width = 0},
                    Background = {
                        Transparent = true
                    },
                    Show = true,
                    MouseLButtonUp = mainDialogButtons.MouseLButtonUp[i],
                    MouseHover = mainDialogButtons.MouseHover[i],
                    MouseLeave = mainDialogButtons.MouseLeave[i],
                    UserData = tostring(i),
                }
            )
        end
    end
end

main()
