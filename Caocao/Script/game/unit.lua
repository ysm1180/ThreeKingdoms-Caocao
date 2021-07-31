require "../control/image_manager.lua"

Direction = {
    LEFT = 0,
    UP = 1,
    RIGHT = 2,
    DOWN = 3
}

Unit = {
    UnitFilePath = "../unit.me5",

    UnitGroupName = {
        ATK = "ATK",
        MOV = "MOV",
        SPC = "SPC"
    },

    UnitClipingSize = {
        ATK = {64, 64},
        MOV = {48, 48},
        SPC = {48, 48}
    },

    New = function(self, index)
        local newUnit = {}

        newUnit.Direction = Direction.DOWN

        newUnit.AtkImage = ImageManager:CreateImage({
            FilePath = self.UnitFilePath,
            Group = {
                Main = self.UnitGroupName.ATK,
                Sub = index
            },
            IsLoadMirror = true
        })
        newUnit.AtkImage:SetClipingSize(self.UnitClipingSize.ATK[1], self.UnitClipingSize.ATK[2])
        newUnit.AtkImage:SetClipingCoordinate(0, 0)

        newUnit.MovImage = ImageManager:CreateImage({
            FilePath = self.UnitFilePath,
            Group = {
                Main = self.UnitGroupName.MOV,
                Sub = index
            },
            IsLoadMirror = true
        })
        newUnit.MovImage:SetClipingSize(self.UnitClipingSize.MOV[1], self.UnitClipingSize.MOV[2])
        newUnit.MovImage:SetClipingCoordinate(0, 0)

        newUnit.SpcImage = ImageManager:CreateImage({
            FilePath = self.UnitFilePath,
            Group = {
                Main = self.UnitGroupName.SPC,
                Sub = index
            },
            IsLoadMirror = true
        })
        newUnit.SpcImage:SetClipingSize(self.UnitClipingSize.SPC[1], self.UnitClipingSize.SPC[2])
        newUnit.Layout = nil
        newUnit.X = 0
        newUnit.Y = 0

        setmetatable(newUnit, self)
        self.__index = self

        return newUnit
    end,

    Show = function(self, layout, x, y)
        self.X = x
        self.Y = y
        self.Layout = layout
        self.AtkImageIndex = layout:AddImage(self.AtkImage, x, y, false)
        self.MovImageIndex = layout:AddImage(self.MovImage, x, y, false)
        self.SpcImageIndex = layout:AddImage(self.SpcImage, x, y, false)
        self:Stop()
    end,

    Stop = function(self)
        if self.Layout ~= nil then
            self.Layout:HideImage(self.AtkImageIndex)
            self.Layout:HideImage(self.SpcImageIndex)
            if self.Direction == Direction.LEFT then
                self.MovImage:Mirror(false)
                self.MovImage:SetClipingCoordinate(0, 8)
            elseif self.Direction == Direction.UP then
                self.MovImage:Mirror(false)
                self.MovImage:SetClipingCoordinate(0, 7)
            elseif self.Direction == Direction.RIGHT then
                self.MovImage:Mirror(true)
                self.MovImage:SetClipingCoordinate(0, 8)
            elseif self.Direction == Direction.DOWN then
                self.MovImage:Mirror(false)
                self.MovImage:SetClipingCoordinate(0, 6)
            end
            self.Layout:ShowImage(self.MovImageIndex)
        end
    end,

    MoveLeft = function(self)
        self.Direction = Direction.LEFT
        local tick = 0
        if self.X > 0 then
            self.MovImage:Mirror(false)

            for i = 0, 47 do
                self.X = self.X - 1

                if tick == 0 and i % 12 == 0 then
                    self.MovImage:SetClipingCoordinate(0, 5)
                    tick = 1
                elseif tick == 1 and i % 12 == 0 then
                    self.MovImage:SetClipingCoordinate(0, 4)
                    tick = 0
                end
                self.Layout:MoveImage(self.MovImageIndex, self.X, self.Y)
            end
            self:Stop()
        end
    end,

    MoveUp = function(self)
        self.Direction = Direction.UP
        local tick = 0
        if self.Y > 0 then
            self.MovImage:Mirror(false)

            for i = 0, 47 do
                self.Y = self.Y - 1

                if tick == 0 and i % 12 == 0 then
                    self.MovImage:SetClipingCoordinate(0, 3)
                    tick = 1
                elseif tick == 1 and i % 12 == 0 then
                    self.MovImage:SetClipingCoordinate(0, 2)
                    tick = 0
                end
                self.Layout:MoveImage(self.MovImageIndex, self.X, self.Y)
            end
            self:Stop()
        end
    end,

    MoveRight = function(self)
        self.Direction = Direction.RIGHT

        local tick = 0
        self.MovImage:Mirror(true)

        for i = 0, 47 do
            self.X = self.X + 1

            if tick == 0 and i % 12 == 0 then
                self.MovImage:SetClipingCoordinate(0, 5)
                tick = 1
            elseif tick == 1 and i % 12 == 0 then
                self.MovImage:SetClipingCoordinate(0, 4)
                tick = 0
            end
            self.Layout:MoveImage(self.MovImageIndex, self.X, self.Y)
        end
        self:Stop()
    end,

    MoveDown = function(self)
        self.Direction = Direction.DOWN

        local tick = 0
        self.MovImage:Mirror(false)

        for i = 0, 47 do
            self.Y = self.Y + 1

            if tick == 0 and i % 12 == 0 then
                self.MovImage:SetClipingCoordinate(0, 1)
                tick = 1
            elseif tick == 1 and i % 12 == 0 then
                self.MovImage:SetClipingCoordinate(0, 0)
                tick = 0
            end
            self.Layout:MoveImage(self.MovImageIndex, self.X, self.Y)
        end
        self:Stop()
    end,

    Attack = function(self)
        self.Layout:HideImage(self.MovImageIndex)
        self.Layout:MoveImage(self.AtkImageIndex, self.X - 8, self.Y - 8)
        self.Layout:ShowImage(self.AtkImageIndex)
        for i = 0, 3 do
            if self.Direction == Direction.LEFT then
                self.AtkImage:Mirror(false)
                self.AtkImage:SetClipingCoordinate(0, 8 + i)
            elseif self.Direction == Direction.UP then
                self.AtkImage:Mirror(false)
                self.AtkImage:SetClipingCoordinate(0, 4 + i)
            elseif self.Direction == Direction.RIGHT then
                self.AtkImage:Mirror(true)
                self.AtkImage:SetClipingCoordinate(0, 8 + i)
            elseif self.Direction == Direction.DOWN then
                self.AtkImage:Mirror(true)
                self.AtkImage:SetClipingCoordinate(0, i)
            end

            self.Layout:Refresh()
            gameManager:Delay(150)
        end
        self:Stop()
    end
}
