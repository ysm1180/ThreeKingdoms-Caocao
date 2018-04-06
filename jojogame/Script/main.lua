require "Script\\window_manager.lua"
require "Script\\movie_player_manager.lua"
require "Script\\button_manager.lua"
require "Script\\menu_manager.lua"
require "Script\\toolbar_manager.lua"
require "Script\\image_manager.lua"
require "Script\\layout_manager.lua"
require "Script\\listview_manager.lua"
require "Script\\static_manager.lua"
require "Script\\groupbox_manager.lua"
require "Script\\checkbox_manager.lua"
require "Script\\radiobutton_manager.lua"

isClosing = false
dialog = false

function quit()
    if dialog then
        
        end
end

function close_load_dialog()
    loadDialog:Close()
end

function main_close()
    isClosing = true
    
    exitBackgroundImage = ImageManager:CreateImage({
        FilePath = "Script\\logo.me5",
        Group = {
            Main = 0,
            Sub = 1
        },
        MaskColor = 0xF700FF,
    })
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
        toolbar:AutoSize()
        
        ratioX = x / 640
        ratioY = (y - toolbar:Height()) / 400
        
        backgroundLayout:SetRatioX(ratioX)
        backgroundLayout:SetRatioY(ratioY)
    end
end

function buttonLoad_click()
    loadDialog = WindowManager:Create({
        Parent = main,
        Width = 386,
        Height = 209,
        TitleName = "데이터 불러오기",
        MinButton = false,
        MaxButton = false,
        Background = {
            Color = {R = 0xF0, G = 0xF0, B = 0xF0},
        },
        Center = true,
        Modal = true
    })
    
    local static = StaticManager:Create({
        Parent = loadDialog,
        AutoSize = true,
        Text = {
            Content = "데이터",
        },
        Show = true,
    })
    
    local rows = {}
    for i = 1, 10 do
        rows[i] = ListViewManager:CreateRow({
            Items = {
                ListViewManager:CreateItem({
                    Text = {
                        Font = {
                            Name = "궁서체",
                        },
                        Content = string.format("No.%2d", i),
                    },
                }),
                ListViewManager:CreateItem({
                    Text = {
                        Content = string.format("Lv.%2d", i),
                    },
                }),
                ListViewManager:CreateItem({
                    Text = {
                        Content = "미사용 저장 데이터",
                    },
                }),
            },
        })
    end
    
    local loadListView = ListViewManager:CreateListView({
        Parent = loadDialog,
        Width = 374,
        Height = 152,
        X = 6,
        Y = 23,
        Columns = {
            ListViewManager:CreateColumn({
                AutoSizeFitItem = true,
                Text = {
                    Content = "",
                }
            }),
            ListViewManager:CreateColumn({
                AutoSizeFitItem = true,
                Text = {
                    Content = "",
                }
            }),
            ListViewManager:CreateColumn({
                Width = 292,
                Text = {
                    Content = "",
                }
            })
        },
        Rows = rows,
        RowHeight = 15,
        Option = {
            ShowColumn = false,
            TrackingSelect = true,
            OneClickItemActivated = false,
            SortClickedColumn = false,
        },
        Show = true,
    })
    local loadCancelButton = ButtonManager:Create({
        Parent = loadDialog,
        Text = {
            Content = "취소"
        },
        Event = {
            MouseLButtonUp = close_load_dialog,
        },
        Width = 75,
        Height = 21,
        X = 305,
        Y = 182,
        Show = true
    })
    loadDialog:ShowModalWindow()
end

function buttonConfig_click()
    configDialog = WindowManager:Create({
        Parent = main,
        Width = 283,
        Height = 328,
        TitleName = "환경설정",
        MinButton = false,
        MaxButton = false,
        Background = {
            Color = {R = 0xFF, G = 0xFF, B = 0xFF},
        },
        Center = true,
        Modal = true
    })
    
    StaticManager:Create({
        Parent = configDialog,
        AutoSize = true,
        X = 13,
        Y = 4,
        Text = {
            Content = "항목을 누르고 설정해 주십시오.",
        },
        Show = true,
    })
    StaticManager:Create({
        Parent = configDialog,
        AutoSize = true,
        X = 13,
        Y = 24,
        Text = {
            Content = "설정을 종료한 후 「OK」를 선택해 주십시오.",
        },
        Show = true,
    })
    
    checks = {
        Text = {
            "게임 중의 BGM을 듣는다",
            "게임 중의 효과음을 듣는다",
            "전투시에 윈도우를 자동적으로 최대화",
            "전투시에 축소 전장도를 자동적으로 표시",
            "클릭할 때까지 대화창을 닫지 않는다",
        },
    }
    for i = 1, 5 do
        checks[i] = CheckBoxManager:Create({
            Parent = configDialog,
            X = 13,
            Y = 51 + 20 * (i - 1),
            Width = 255,
            Height = 21,
            Text = {
                Content = checks.Text[i],
            },
            Show = true,
        })
    end
    
    radios = {
        Text = {
            "길다",
            "보통",
            "짧다",
            "빠름",
            "보통",
            "느림",
        }
    }
    for i = 1, 6 do
        radios[i] = RadioButtonManager:Create({
            Parent = configDialog,
            X = 43 + 79 * ((i - 1) % 3),
            Y = 184 + 44 * math.floor((i - 1) / 3),
            Width = 48,
            Height = 12,
            Text = {
                Content = radios.Text[i],
            },
            Show = true
        })
    end
    
    GroupBoxManager:Create({
        Parent = configDialog,
        Width = 255,
        Height = 33,
        X = 13,
        Y = 169,
        Text = {
            Content = "메세지 대기시간",
        },
        Show = true,
    })
    GroupBoxManager:Create({
        Parent = configDialog,
        Width = 255,
        Height = 33,
        X = 13,
        Y = 213,
        Text = {
            Content = "장수 이동 속도",
        },
        Show = true,
    })
    
    local configOKButton = ButtonManager:Create({
        Parent = configDialog,
        Text = {
            Content = "OK"
        },
        Event = {
            MouseLButtonUp = function() end,
        },
        Width = 75,
        Height = 21,
        X = 52,
        Y = 294,
        Show = true
    })
    configCancelButton = ButtonManager:Create({
        Parent = configDialog,
        Text = {
            Content = "취소"
        },
        Event = {
            MouseLButtonUp = function()
                configDialog:Close()
            end,
        },
        Width = 75,
        Height = 21,
        X = 156,
        Y = 294,
        Show = true
    })
    configDialog:ShowModalWindow()
end

function mainButton_enter(self)
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
    
    mainMenu = MenuManager:CreateMenu({
        MenuManager:CreateMenuItem({
            Text = {
                Content = "파일",
            },
            ChildItems = {
                MenuManager:CreateMenuItem({
                    Text = {
                        Content = "불러오기",
                    },
                    Enabled = false,
                }),
                MenuManager:CreateMenuItem({
                    Text = {
                        Content = "저장",
                    }
                }),
                MenuManager:CreateMenuItem({
                    Seperator = true,
                }
                ),
                MenuManager:CreateMenuItem({
                    Text = {
                        Content = "종료",
                    },
                    Event = {
                        Click = function()
                            main:Close()
                        end,
                    },
                })
            },
        })
    })
    
    backgroundImage = ImageManager:CreateImage({
        FilePath = "Script\\logo.me5",
        Group = {
            Main = 0,
            Sub = 0,
        },
        MaskColor = 0xF700FF,
    })
    main = WindowManager:Create({
        Width = mainSize.Width,
        Height = mainSize.Height,
        Center = true,
        TitleName = title,
        Background = {
            Color = {R = 0, G = 0, B = 0},
        },
        Event = {
            MouseLButtonUp = function()
                if openningMovie:IsPlaying() then
                    openningMovie:Destroy()
                end
                if isClosing then
                    gameManager:StopDelay()
                end
            end,
            Close = main_close,
            Destroy = function()
                gameManager:Quit()
            end,
            Size = main_size,
        },
        Menu = mainMenu,
    })
    exitToolbarButton = ToolbarManager:CreateToolbarButton({
        Text = {
            Tooltip = "종료",
        },
        Event = {
            Click = function()
                main:Close()
            end,
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
    })
    toolbar = ToolbarManager:CreateToolbar({
        Parent = main,
        Image = {
            Width = 24,
            Height = 28,
        },
        Buttons = {
            exitToolbarButton,
            ToolbarManager:CreateToolbarButton({
                Text = {
                    Tooltip = "저장"
                },
                Image = ImageManager:CreateImage({
                    FilePath = "Script\\logo.me5",
                    Group = {
                        Main = 1,
                        Sub = 1
                    },
                    MaskColor = 0xC0C0C0,
                }
                ),
                Enabled = false,
            }),
            ToolbarManager:CreateToolbarButton({
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
            })
        },
        Show = true,
    })
    backgroundLayout = LayoutManager:CreateLayout({
        Y = toolbar:Height(),
        Ratio = {
            X = 1,
            Y = 1,
        }
    })
    main:Show()
    main:SetSize(nil, main:Height() + toolbar:Height())
    
    openningMovie = MoviePlayerManager:Create({
        FileName = "Script\\LOGO.avi",
        Parent = main,
        Event = {
            End = function()
                openningMovie:Destroy()
            end,
        },
        Center = true,
    })
    openningMovie:Play()
    
    exitToolbarButton:Enable()
    backgroundLayout:AddImage(backgroundImage, 0, 0)
    main:AddLayout(backgroundLayout)
    
    if dialog then
        mainDialog = WindowManager:Create({
            Parent = main,
            Width = 202,
            Height = 142,
            TitleName = title,
            ControlBox = false,
            MinButton = false,
            MaxButton = false,
            Background = {
                Color = {R = 0xF0, G = 0xF0, B = 0xF0},
            },
            Center = true,
            Modal = true,
        })
        mainDialogButtons = {
            Text = {"새로운 게임을 시작한다", "저장 데이터를 불러온다", "환경 설정", "게임 종료"},
            MouseLButtonUp = {
                nil,
                buttonLoad_click,
                nil,
                function()
                    mainDialog:Close()
                    main:Close()
                end
            },
        }
        for i = 1, 4 do
            mainDialogButtons[i] = ButtonManager:Create({
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
            })
        end
        mainDialog:ShowModalWindow()
    
    else
        mainDialogButtonImages = {}
        for i = 1, 8 do
            mainDialogButtonImages[i] = ImageManager:CreateImage({
                FilePath = "Script\\logo.me5",
                Group = {
                    Main = 2,
                    Sub = i - 1
                },
                MaskColor = 0xF700FF,
            })
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
            MouseLButtonUp = {nil, buttonLoad_click, buttonConfig_click, function()main:Close() end},
            MouseEnter = {mainButton_enter, mainButton_enter, mainButton_enter, mainButton_enter},
            MouseLeave = {mainButton_leave, mainButton_leave, mainButton_leave, mainButton_leave},
        }
        for i = 1, 4 do
            mainDialogButtons[i] = ButtonManager:Create({
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
                    MouseEnter = mainDialogButtons.MouseEnter[i],
                    MouseLeave = mainDialogButtons.MouseLeave[i],
                },
                UserData = tostring(i),
            })
        end
    end
end

main()
