require "../base/keycode.lua"

battleImage = ImageManager:CreateImage({
    FilePath = "../map.me5",
    Image = {
        Group = "Battle",
        Item = 0
    }
})

battleMapLayout = LayoutManager:CreateLayout({})
battleMapLayout:AddImage(battleImage, 0, toolbar:Height())

mainWindow:AddLayout(battleMapLayout)
mainWindow:SetSize(battleImage:Width(), battleImage:Height() + toolbar:Height())

mainWindow:Center()

unitAtkImage = ImageManager:CreateImage({
    FilePath = "../unit.me5",
    Image = {
        Group = "ATK",
        Item = 0
    }
})
unitAtkImage:SetClipingSize(64, 64)
unitAtkImage:SetClipingCoordinate(0, 0)

unitMoveImage = ImageManager:CreateImage({
    FilePath = "../unit.me5",
    Imag = {
        Group = "MOV",
        Item = 0
    }
})

unitMoveImage:SetClipingSize(48, 48)
unitMoveImage:SetClipingCoordinate(0, 0)
x = 48
y = 48 + toolbar:Height()
unitObjectLayout = LayoutManager:CreateLayout({})
unitMoveImageIndex = unitObjectLayout:AddImage(unitMoveImage, x, y)
unitAtkImageIndex = unitObjectLayout:AddImage(unitAtkImage, x, y)
unitObjectLayout:HideImage(unitAtkImageIndex)
mainWindow:AddLayout(unitObjectLayout)

tick = 0
direction = 0

isKeyDown = false
mainWindow:SetKeyDownEvent(function(self, key)
    if isKeyDown == false then
        isKeyDown = true

        if key == KEY_CODE.UP or key == KEY_CODE.DOWN or key == KEY_CODE.LEFT or key == KEY_CODE.RIGHT then
            for i = 0, 47 do
                if key == KEY_CODE.LEFT then
                    x = x - 1
                    direction = 2
                elseif key == KEY_CODE.UP then
                    y = y - 1
                    direction = 1
                elseif key == KEY_CODE.RIGHT then
                    x = x + 1
                elseif key == KEY_CODE.DOWN then
                    y = y + 1
                    direction = 0
                end

                if tick == 0 and i % 12 == 0 then
                    unitMoveImage:SetClipingRect({
                        Top = direction * 2 * 48 + 48,
                        Left = 0,
                        Right = 47,
                        Bottom = direction * 2 * 48 + 96
                    })
                    tick = 1
                elseif tick == 1 and i % 12 == 0 then
                    unitMoveImage:SetClipingRect({
                        Top = direction * 2 * 48,
                        Left = 0,
                        Right = 47,
                        Bottom = direction * 2 * 48 + 48
                    })
                    tick = 0
                end
                unitObjectLayout:MoveImage(unitMoveImageIndex, x, y)
            end

        elseif key == KEY_CODE.SPACE then
            unitObjectLayout:HideImage(unitMoveImageIndex)
            unitObjectLayout:MoveImage(unitAtkImageIndex, x - 8, y - 8)
            unitObjectLayout:ShowImage(unitAtkImageIndex)
            for i = 0, 3 do
                unitAtkImage:SetClipingCoordinate(0, direction * 4 + i)
                unitObjectLayout:Refresh()
                gameManager:Delay(150)
            end
            unitObjectLayout:HideImage(unitAtkImageIndex)
            unitObjectLayout:ShowImage(unitMoveImageIndex)
        end
        isKeyDown = false
    end
end)
