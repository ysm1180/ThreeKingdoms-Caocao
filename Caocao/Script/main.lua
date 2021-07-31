require "control/window_manager.lua"
require "control/movie_player_manager.lua"
require "control/button_manager.lua"
require "control/menu_manager.lua"
require "control/toolbar_manager.lua"
require "control/image_manager.lua"
require "control/layout_manager.lua"
require "control/listview_manager.lua"
require "control/static_manager.lua"
require "control/groupbox_manager.lua"
require "control/checkbox_manager.lua"
require "control/radiobutton_manager.lua"
require "control/combobox_manager.lua"
require "control/audio_player_manager.lua"
require "control/graphic_text_manager.lua"

-- 게임이 종료되고 있는 중인지 체크
local isClosing = false

-- 시작 방식 설정 - true 면 오리지날 조조전 버전
local dialog = false

DEBUG(true)

function Start()
    local title = "三國志曺操傳"
    local mainSize = {
        Width = 640,
        Height = 400
    }

    mainMenu = MenuManager:CreateMenu({MenuManager:CreateMenuItem({
        Text = "파일",
        ChildItems = {MenuManager:CreateMenuItem({
            Text = "불러오기",
            Enabled = false
        }), MenuManager:CreateMenuItem({
            Text = "저장"
        }), MenuManager:CreateMenuItem({
            Seperator = true
        }), MenuManager:CreateMenuItem({
            Text = "종료",
            Event = {
                Click = function()
                    main:Close()
                end
            }
        })}
    })})

    -- 배경 이미지 생성
    backgroundImage = ImageManager:CreateImage({
        FilePath = "logo.me5",
        Image = {
            Group = "Logo",
            Item = 0
        }
    })

    -- 메인 Window 생성
    main = WindowManager:Create({
        Width = mainSize.Width,
        Height = mainSize.Height,
        Center = true,
        TitleName = title,
        BackgroundColor = {
            R = 0,
            G = 0,
            B = 0
        },
        Event = {
            MouseLButtonUp = function()
                -- 오프닝 동영상이 재생 중이면 멈춤
                if openningMovie:IsPlaying() then
                    openningMovie:Destroy()
                end

                -- 종료 중이면 Delay 없애기
                if isClosing then
                    gameManager:StopDelay()
                end
            end,
            Close = function()
                isClosing = true

                -- 종료 배경 이미지 생성
                exitBackgroundImage = ImageManager:CreateImage({
                    FilePath = "logo.me5",
                    Image = {
                        Group = "Logo",
                        Item = 1
                    }
                })
                -- 레이아웃 생성
                exitBackgroundLayout = LayoutManager:CreateLayout({})
                exitBackgroundLayout:AddImage(exitBackgroundImage, 0, 0)

                -- 버튼 삭제
                for i = 1, #mainDialogButtons do
                    mainDialogButtons[i]:Destroy()
                end

                main:SetSize(exitBackgroundImage:Width(), exitBackgroundImage:Height())
                main:Center()

                -- 배경 이미지 레이아웃 삭제, 종료 배경 이미지 레이아웃 추가
                main:Clear()
                main:AddLayout(exitBackgroundLayout)

                -- 3초 딜레이
                gameManager:Delay(3000)
            end,
            Destroy = function()
                -- 게임 종료
                gameManager:Quit()
            end,
            Size = function(self, w, h)
                if backgroundLayout ~= nil and toolbar ~= nil then
                    -- 크기 조절시 툴바 크기와 레이아웃 비율 변경
                    toolbar:AutoSize()

                    ratioX = w / mainSize.Width
                    ratioY = h / mainSize.Height

                    backgroundLayout:SetRatioX(ratioX)
                    backgroundLayout:SetRatioY(ratioY)

                    if mainDialogButtons ~= nil then
                        for i = 1, #mainDialogButtons do
                            local changedWidth = mainDialogButtons.OriginalWidth * ratioX
                            local changedHeight = mainDialogButtons.OriginalHeight * ratioY
                            local changedX = mainDialogButtons.OriginalX * ratioX
                            local index = tonumber(mainDialogButtons[i]:UserData())
                            local changedY =
                                (mainDialogButtons.OriginalY + (index - 1) * mainDialogButtons.OriginalYGap) * ratioY
                            mainDialogButtons[i]:Move(changedX, changedY)
                            mainDialogButtons[i]:SetSize(changedWidth, changedHeight)
                        end
                    end
                end
            end
        },
        Menu = mainMenu
    })

    -- 종료 툴바 버튼 생성
    exitToolbarButton = ToolbarManager:CreateToolbarButton({
        Tooltip = "종료",
        Event = {
            Click = function()
                main:Close()
            end
        },
        Image = ImageManager:CreateImage({
            FilePath = "logo.me5",
            Image = {
                Group = 1,
                Item = 0
            },
            MaskColor = 0xC0C0C0
        }),
        Enabled = false
    })

    -- 툴바 생성
    toolbar = ToolbarManager:CreateToolbar({
        Parent = main,
        ImageSize = {
            Width = 24,
            Height = 28
        },
        Buttons = {exitToolbarButton, ToolbarManager:CreateToolbarButton({
            Tooltip = "저장",
            Image = ImageManager:CreateImage({
                FilePath = "logo.me5",
                Image = {
                    Group = 1,
                    Item = 1
                },
                MaskColor = 0xC0C0C0
            }),
            Enabled = false
        }), ToolbarManager:CreateToolbarButton({
            Tooltip = "불러오기",
            Image = ImageManager:CreateImage({
                FilePath = "logo.me5",
                Image = {
                    Group = 1,
                    Item = 2
                },
                MaskColor = 0xC0C0C0
            }),
            Enabled = false
        })},
        Show = true
    })

    -- 배경 이미지 레이아웃 생성
    backgroundLayout = LayoutManager:CreateLayout({})

    -- 메인 창 띄우기
    main:Show()
    main:Center()

    -- 오프닝 동영상 생성
    openningMovie = MoviePlayerManager:Create({
        FileName = "LOGO.avi",
        Parent = main,
        Event = {
            End = function()
            end
        },
        Center = true
    })
    -- 재생
    openningMovie:Play()

    -- 재생되는 동안 이 아래는 실행되지 않음 --

    -- 동영상 종료된 후 종료 툴바 버튼 활성화
    exitToolbarButton:Enable()

    -- 레이아웃에 이미지 추가
    backgroundLayout:AddImage(backgroundImage, 0, 0)

    -- 레이아웃 추가
    main:AddLayout(backgroundLayout)

    -- 오리지날 조조전 버전
    if dialog then
        -- dialog 창 생성
        mainDialog = WindowManager:Create({
            Parent = main,
            Width = 202,
            Height = 142,
            TitleName = title,
            ControlBox = false,
            MinButton = false,
            MaxButton = false,
            BackgroundColor = {
                R = 0xF0,
                G = 0xF0,
                B = 0xF0
            },
            Center = true,
            Modal = true
        })

        -- 버튼 생성
        mainDialogButtons = {
            Text = {"새로운 게임을 시작한다", "저장 데이터를 불러온다", "환경 설정",
                    "게임 종료"},
            MouseLButtonUp = {nil, function()
                require "dialog/load_dialog.lua"
            end, function()
                require "dialog/config_dialog.lua"
            end, function()
                mainDialog:Close()
                main:Close()
            end}
        }
        for i = 1, 4 do
            mainDialogButtons[i] = ButtonManager:Create({
                Parent = mainDialog,
                Text = mainDialogButtons.Text[i],
                TextColor = {
                    Pushed = {
                        R = 0xFF,
                        G = 0x00,
                        B = 0x00
                    }
                },
                Width = 176,
                Height = 30,
                X = 12,
                Y = 11 + (i - 1) * 30,
                BorderWidth = 1,
                Transparent = false,
                Show = true,
                Event = {
                    MouseLButtonUp = mainDialogButtons.MouseLButtonUp[i]
                }
            })
        end

        -- Modal 창으로 띄움
        mainDialog:ShowModalWindow()

    else
        -- 버튼 이미지 불러오기
        mainDialogButtonImages = {}
        for i = 1, 8 do
            mainDialogButtonImages[i] = ImageManager:CreateImage({
                FilePath = "logo.me5",
                Image = {
                    Group = "TitleButton",
                    Item = i - 1
                }
            })
        end

        buttonImageIndex = {-1, -1, -1, -1}

        buttonImageIndex[1] = backgroundLayout:AddImage(mainDialogButtonImages[1], 640, 28)
        buttonImageIndex[2] = backgroundLayout:AddImage(mainDialogButtonImages[3], 640, 98)
        buttonImageIndex[3] = backgroundLayout:AddImage(mainDialogButtonImages[5], 640, 168)
        buttonImageIndex[4] = backgroundLayout:AddImage(mainDialogButtonImages[7], 640, 238)

        -- 버튼 이미지 애니메이션
        for i = 0, 550, 10 do
            if i <= 220 then
                backgroundLayout:MoveImage(buttonImageIndex[1], 640 - i, 28)
            end

            if i > 110 and i <= 330 then
                backgroundLayout:MoveImage(buttonImageIndex[2], 640 - (i - 110), 98)
            end

            if i > 220 and i <= 440 then
                backgroundLayout:MoveImage(buttonImageIndex[3], 640 - (i - 220), 168)
            end

            if i > 330 then
                backgroundLayout:MoveImage(buttonImageIndex[4], 640 - (i - 330), 238)
            end
        end

        -- 버튼 이벤트 설정
        mainDialogButtons = {
            MouseLButtonUp = {function()
                -- 버튼 삭제
                for i = 1, #mainDialogButtons do
                    mainDialogButtons[i]:Destroy()
                end
                require "scenario/r00.lua"
            end, function()
                require "dialog/load_dialog.lua"
            end, function()
                require "dialog/config_dialog.lua"
            end, function()
                main:Close()
            end},
            MouseEnter = function(self)
                local button = ButtonManager:CreateInstance(self)
                local index = tonumber(button:UserData())

                backgroundLayout:DeleteImage(buttonImageIndex[index], false)
                buttonImageIndex[index] = backgroundLayout:AddImage(mainDialogButtonImages[index * 2], 420,
                    28 + (index - 1) * 70)
            end,
            MouseLeave = function(self)
                local button = ButtonManager:CreateInstance(self)
                local index = tonumber(button:UserData())

                backgroundLayout:DeleteImage(buttonImageIndex[index], false)
                buttonImageIndex[index] = backgroundLayout:AddImage(mainDialogButtonImages[index * 2 - 1], 420,
                    28 + (index - 1) * 70)
            end,
            OriginalHeight = 44,
            OriginalWidth = 176,
            OriginalX = 420,
            OriginalY = 28,
            OriginalYGap = 70
        }

        -- 버튼 생성
        for i = 1, 4 do
            mainDialogButtons[i] = ButtonManager:Create({
                Parent = main,
                Width = 176,
                Height = 44,
                X = 420,
                Y = 28 + (i - 1) * 70,
                BorderWidth = 0,
                Transparent = true,
                Show = true,
                Event = {
                    MouseLButtonUp = mainDialogButtons.MouseLButtonUp[i],
                    MouseEnter = mainDialogButtons.MouseEnter,
                    MouseLeave = mainDialogButtons.MouseLeave
                },
                UserData = tostring(i)
            })
        end

        backgroupSound = AudioPlayerManager:LoadFromMe5File("sound.me5", {
            Group = "SoundTrack",
            Item = 0
        })
        backgroupSound:Play()

    end
end

Start()
