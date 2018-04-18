local clock = gameManager:Clock()

configDialog = WindowManager:Create({
    Parent = main,
    Width = 283,
    Height = 328,
    TitleName = "환경설정",
    MinButton = false,
    MaxButton = false,
    Background = {
        Color = {R = 0xF0, G = 0xF0, B = 0xF0},
    },
    Center = true,
    Modal = true
})

local dialogLayout = LayoutManager:CreateLayout({
    Ratio = {
        X = 1,
        Y = 1,
    }
})
local backImage = ImageManager:CreateImage({
    FilePath = "logo.me5",
    Group = {
        Main = "Background",
        Sub = 0,
    },
    MaskColor = 0xF700FF,
    Brightness = 1.3,
})
for x = 0, configDialog:Width(), backImage:Width() do
    for y = 0, configDialog:Height(), backImage:Height() do
        dialogLayout:AddImage(backImage, x, y)
    end
end
configDialog:AddLayout(dialogLayout)

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
StaticManager:Create({
    Parent = configDialog,
    AutoSize = true,
    X = 13,
    Y = 264,
    Text = {
        Content = "대화 상자의 상태",
    },
    Show = true,
})

configChecks = {
    Text = {
        "게임 중의 BGM을 듣는다",
        "게임 중의 효과음을 듣는다",
        "전투시에 윈도우를 자동적으로 최대화",
        "전투시에 축소 전장도를 자동적으로 표시",
        "클릭할 때까지 대화창을 닫지 않는다",
    },
}
local file = fileManager:LoadFile("config.sav")

for i = 1, 5 do
    local checked = fileManager:ReadByte(file, i - 1)
    configChecks[i] = CheckBoxManager:Create({
        Parent = configDialog,
        X = 13,
        Y = 51 + 20 * (i - 1),
        Width = 255,
        Height = 21,
        Text = {
            Content = configChecks.Text[i],
        },
        Checked = checked == 1,
        Show = true,
    })
end


configRadios = {
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
    configRadios[i] = RadioButtonManager:Create({
        Parent = configDialog,
        X = 43 + 79 * ((i - 1) % 3),
        Y = 184 + 44 * math.floor((i - 1) / 3),
        Width = 48,
        Height = 12,
        Text = {
            Content = configRadios.Text[i],
        },
        Show = true,
    })
end

fileManager:CloseFile(file)

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

ButtonManager:Create({
    Parent = configDialog,
    Text = {
        Content = "OK"
    },
    Event = {
        MouseLButtonUp = function() 
            local file = fileManager:LoadFile("config.sav")
            for i = 1, 5 do
                if configChecks[i]:Checked() then
                    fileManager:WriteByte(file, i - 1, 0x01)
                else
                    fileManager:WriteByte(file, i - 1, 0x00)
                end
            end
            fileManager:CloseFile(file)

            configDialog:Close()
        end,
    },
    Width = 75,
    Height = 21,
    X = 52,
    Y = 294,
    Show = true
})
ButtonManager:Create({
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

ComboBoxManager:Create({
    Parent = configDialog,
    X = 147,
    Y = 261,
    Width = 103,
    Height = 94,
    Items = {
        "배경1",
        "배경2",
    },
    Show = true,
})

OUTPUT("CLOCK : "  .. tostring(gameManager:Clock() - clock))