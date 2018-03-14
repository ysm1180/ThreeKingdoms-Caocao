require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"
require "Script\\button_manager.lua"
require "Script\\menu_manager.lua"
require "Script\\toolbar_manager.lua"
require "Script\\image_manager.lua"
require "Script\\layout_manager.lua"
require "Script\\listview_manager.lua"

isClosing = false
dialog = false

function quit()
    if dialog then
        mainDialog:Close()
    end
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
    isClosing = true

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

    for i = 1, #mainDialogButtons do
        mainDialogButtons[i]:Destroy()
    end

    main:DeleteLayout(backgroundLayout)
    main:AddLayout(exitBackgroundLayout)

    gameManager:Delay(3000)
end

function main_size(self, x, y)
    if backgroundLayout ~= nil and toolbar ~= nil then
        ratioX = x / 640
        ratioY = (y - toolbar:Height()) / 400

        for i = 1, #mainDialogButtons do
            if mainDialogButtons[i] ~= nil then
                mainDialogButtons[i]:Move()
            end
        end

        backgroundLayout:SetRatioX(ratioX)
        backgroundLayout:SetRatioY(ratioY)
    end
end

function main_destroy()
    gameManager:Quit()
end

function openning_end()
    openningMovie:Destroy()
end

function buttonLoad_click()
    backgroundLayout:Move(40, nil)

    -- local loadDialog =
    --     WindowManager:Create(
    --     {
    --         Parent = main,
    --         Width = 386,
    --         Height = 209,
    --         TitleName = "데이터 불러오기",
    --         MinButton = false,
    --         MaxButton = false,
    --         BackColor = {R = 0xF0, G = 0xF0, B = 0xF0},
    --         Center = true,
    --         Modal = true
    --     }
    -- )

    -- local loadListView =
    --     ListViewManager:Create(
    --     {
    --         Parent = loadDialog,
    --         Width = 374,
    --         Height = 153,
    --         X = 6,
    --         Y = 23,
    --         Show = true
    --     }
    -- )

    -- local loadCancelButton =
    --     ButtonManager:Create(
    --     {
    --         Parent = loadDialog,
    --         Text = {
    --             Content = "취소"
    --         },
    --         Width = 75,
    --         Height = 21,
    --         X = 305,
    --         Y = 182,
    --         Show = true
    --     }
    -- )

    -- loadDialog:ShowModalWindow()
end

function mainButton_hover(self)
    local button = ButtonManager:CreateInstance(self)
    local index = tonumber(button:UserData())

    backgroundLayout:DeleteImage(mainDialogButtonImages[index * 2 - 1], false)
    backgroundLayout:AddImage(mainDialogButtonImages[index * 2], 420, 28 + (index - 1) * 70)
end

function mainButton_leave(self)
    local button = ButtonManager:CreateInstance(self)
    local index = tonumber(button:UserData())

    backgroundLayout:DeleteImage(mainDialogButtonImages[index * 2], false)
    backgroundLayout:AddImage(mainDialogButtonImages[index * 2 - 1], 420, 28 + (index - 1) * 70)
end

function main()
    local title = "三國志曺操傳"
    local mainSize = {
        Width = 640, 
        Height = 400,
    }

    mainMenu =
        MenuManager:CreateMenu(
        {
            MenuManager:CreateMenuItem(
                {
                    Text = {
                        Content = "파일",
                    },
                    ChildItems = {
                        MenuManager:CreateMenuItem(
                            {
                                Text = {
                                    Content = "불러오기",
                                },
                                Enabled = false,
                            }
                        ),
                        MenuManager:CreateMenuItem(
                            {
                                Text = {
                                    Content = "저장",
                                }
                            }
                        ),
                        MenuManager:CreateMenuItem(
                            {
                                Seperator = true,
                            }
                        ),
                        MenuManager:CreateMenuItem(
                            {
                                Text = {
                                    Content = "종료",
                                },
                                Event = {
                                    Click = "quit",
                                },
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
                Sub = 0,
            },
            MaskColor = 0xF700FF,
        }
    )

    
    main =
        WindowManager:Create(
        {
            Width = mainSize.Width,
            Height = mainSize.Height,
            Center = true,
            TitleName = title,
            BackColor = {R = 0, G = 0, B = 0},
            Event = {
                MouseLButtonUp = "main_click",
                Close = "main_close",
                Destroy = "main_destroy",
                Size = "main_size",
            },
            Menu = mainMenu,
        }
    )

    exitToolbarButton =
        ToolbarManager:CreateToolbarButton(
        {
            Text = {
                Tooltip = "종료",
            },
            Event = {
                Click = "quit",
            },
            Image = ImageManager:CreateImage(
                {
                    FilePath = "Script\\logo.me5",
                    Group = {
                        Main = 1,
                        Sub = 0,
                    },
                    MaskColor = 0xC0C0C0,
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
                Height = 28,
            },
            Buttons = {
                exitToolbarButton,
                ToolbarManager:CreateToolbarButton(
                    {
                        Text = {
                            Tooltip = "저장"
                        },
                        Image = ImageManager:CreateImage(
                            {
                                FilePath = "Script\\logo.me5",
                                Group = {
                                    Main = 1,
                                    Sub = 1
                                },
                                MaskColor = 0xC0C0C0,
                            }
                        ),
                        Enabled = false,
                    }
                ),
                ToolbarManager:CreateToolbarButton(
                    {
                        Text = {
                            Tooltip = "불러오기",
                        },
                        Image = ImageManager:CreateImage(
                            {
                                FilePath = "Script\\logo.me5",
                                Group = {
                                    Main = 1,
                                    Sub = 2,
                                },
                                MaskColor = 0xC0C0C0,
                            }
                        ),
                        Enabled = false,
                    }
                )
            },
            Show = true,
        }
    )

    backgroundLayout =
        LayoutManager:CreateLayout(
        {
            Y = toolbar:Height(),
            Ratio = {
                X = 1,
                Y = 1,
            }
        }
    )

    main:Show()
    main:SetSize(nil, main:Height() + toolbar:Height())

    openningMovie =
        MoviePlayerManager:Create(
        {
            FileName = "Script\\LOGO.avi",
            Parent = main,
            Event = {
                End = "openning_end",
            },
            Center = true,
        }
    )
    openningMovie:Play()

    exitToolbarButton:Enable()
    backgroundLayout:AddImage(backgroundImage, 0, 0)
    main:AddLayout(backgroundLayout)

    if dialog then
        mainDialog =
            WindowManager:Create(
            {
                Parent = main,
                Width = 202,
                Height = 142,
                TitleName = title,
                ControlBox = false,
                MinButton = false,
                MaxButton = false,
                BackColor = {R = 0xF0, G = 0xF0, B = 0xF0},
                Center = true,
                Modal = true,
            }
        )

        mainDialogButtons = {
            Text = {"새로운 게임을 시작한다", "저장 데이터를 불러온다", "환경 설정", "게임 종료"},
            MouseLButtonUp = {"", "buttonLoad_click", "", "quit"},
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
                    },
                    Width = 176,
                    Height = 30,
                    X = 12,
                    Y = 11 + (i - 1) * 30,
                    Border = {Width = 1},
                    Transparent = false,
                    Show = true,
                    Event = {
                        MouseLButtonUp = mainDialogButtons.MouseLButtonUp[i],
                    },
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
                        Sub = i - 1
                    },
                    MaskColor = 0xF700FF,
                }
            )
        end

        for i = 0, 550, 10 do
            if i <= 220 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[1], false)
                backgroundLayout:AddImage(mainDialogButtonImages[1], 640 - i, 28)
            end

            if i > 110 and i <= 330 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[3], false)
                backgroundLayout:AddImage(mainDialogButtonImages[3], 640 - (i - 110), 98)
            end

            if i > 220 and i <= 440 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[5], false)
                backgroundLayout:AddImage(mainDialogButtonImages[5], 640 - (i - 220), 168)
            end

            if i > 330 then
                backgroundLayout:DeleteImage(mainDialogButtonImages[7], false)
                backgroundLayout:AddImage(mainDialogButtonImages[7], 640 - (i - 330), 238)
            end

            gameManager:Delay(5)
        end

        mainDialogButtons = {
            MouseLButtonUp = {"", "buttonLoad_click", "", "quit"},
            MouseHover = {"mainButton_hover", "mainButton_hover", "mainButton_hover", "mainButton_hover"},
            MouseLeave = {"mainButton_leave", "mainButton_leave", "mainButton_leave", "mainButton_leave"},
        }
        for i = 1, 4 do
            mainDialogButtons[i] =
                ButtonManager:Create(
                {
                    Parent = main,
                    Width = 176,
                    Height = 44,
                    X = 420,
                    Y = 28 + (i - 1) * 70 + toolbar:Height(),
                    Border = {Width = 0},
                    Background = {
                        Transparent = true
                    },
                    Show = true,
                    Event = {
                        MouseLButtonUp = mainDialogButtons.MouseLButtonUp[i],
                        MouseHover = mainDialogButtons.MouseHover[i],
                        MouseLeave = mainDialogButtons.MouseLeave[i],
                    },
                    UserData = tostring(i),
                }
            )
        end
    end
end

main()
