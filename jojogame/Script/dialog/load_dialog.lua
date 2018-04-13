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

StaticManager:Create({
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

ListViewManager:CreateListView({
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

ButtonManager:Create({
    Parent = loadDialog,
    Text = {
        Content = "취소"
    },
    Event = {
        MouseLButtonUp = function()
            loadDialog:Close()
        end,
    },
    Width = 75,
    Height = 21,
    X = 305,
    Y = 182,
    Show = true
})