require '../control/image_manager.lua'
require '../control/audio_player_manager.lua'
require 'types.lua'

---@class Unit
Unit = {
    UnitFilePath = '../unit.me5',
    Map = nil,
    X = 0,
    Y = 0,
    DeltaX = 0,
    DeltaY = 0,
    Moving = false,
    DeltaMoving = 0,
    MovingDirection = 0,
    AttackAnimation = false,
    AttackEvent = function()
    end,
    HitEvent = function()
    end,
    IsHide = true,
    UnitGroupName = {
        ATK = 'ATK',
        MOV = 'MOV',
        SPC = 'SPC'
    },
    UnitClipingSize = {
        ATK = {64, 64},
        MOV = {48, 48},
        SPC = {48, 48}
    }
}

function Unit:New(map, character, layout, type, direction)
    local newUnit = {}

    if direction == nil then
        newUnit.Direction = UnitDirection.DOWN
    else
        newUnit.Direction = direction
    end

    newUnit.Map = map
    newUnit.Character = character
    newUnit.CurMove = character.TroopType.MoveCount

    local index = character:GetBattleModelIndex()
    if type == UnitType.Enemy then
        index = index + 2
    end

    newUnit.AtkImage = self:SetImage("ATK", index)
    newUnit.MovImage = self:SetImage("MOV", index)
    newUnit.SpcImage = self:SetImage("SPC", index)
    newUnit.Layout = layout

    newUnit.AtkImageIndex = layout:AddImage(newUnit.AtkImage, 0, 0, false)
    newUnit.MovImageIndex = layout:AddImage(newUnit.MovImage, 0, 0, false)
    newUnit.SpcImageIndex = layout:AddImage(newUnit.SpcImage, 0, 0, false)

    newUnit.UnitType = type

    newUnit.MoveSound = AudioPlayerManager:LoadFromMe5File({
        FilePath = '../sound.me5',
        Group = {
            Main = 'Effect',
            Sub = 'Move_Walk'
        }
    })
    newUnit.HitSound = AudioPlayerManager:LoadFromMe5File({
        FilePath = '../sound.me5',
        Group = {
            Main = 'Effect',
            Sub = 'Hit_Knife'
        }
    })

    newUnit.HP = character:GetHP()
    newUnit.CurHP = character:GetHP()
    newUnit.Power = character:GetPower()
    newUnit.Defense = character:GetDefense()

    setmetatable(newUnit, self)
    self.__index = self

    return newUnit
end

function Unit:SetImage(type, index)
    local image = ImageManager:CreateImage({
        FilePath = Unit.UnitFilePath,
        Group = {
            Main = Unit.UnitGroupName[type],
            Sub = index
        },
        IsLoadMirror = true
    })

    image:SetClipingSize(Unit.UnitClipingSize[type][1], Unit.UnitClipingSize[type][2])
    image:SetClipingCoordinate(0, 0)

    return image
end

function Unit:GetMapX()
    return math.floor(self.X / self.Map.TileSize.Width)
end

function Unit:GetMapY()
    return math.floor(self.Y / self.Map.TileSize.Height)
end

function Unit:Show(x, y)
    self.IsHide = false

    self.X = x * self.Map.TileSize.Width
    self.Y = y * self.Map.TileSize.Height

    self.Layout:MoveImage(self.AtkImageIndex, self.X, self.Y, false)
    self.Layout:MoveImage(self.MovImageIndex, self.X, self.Y, false)
    self.Layout:MoveImage(self.SpcImageIndex, self.X, self.Y, false)

    self:Stop()
end

function Unit:Hide()
    self.IsHide = true

    self.Layout:HideImage(self.AtkImageIndex)
    self.Layout:HideImage(self.SpcImageIndex)
    self.Layout:HideImage(self.MovImageIndex)
end

function Unit:Stop()
    if self.Layout ~= nil then
        self.Layout:HideImage(self.AtkImageIndex)
        self.Layout:HideImage(self.SpcImageIndex)
        if self.Direction == UnitDirection.LEFT then
            self.MovImage:Mirror(false)
            self.MovImage:SetClipingCoordinate(0, 8)
        elseif self.Direction == UnitDirection.UP then
            self.MovImage:Mirror(false)
            self.MovImage:SetClipingCoordinate(0, 7)
        elseif self.Direction == UnitDirection.RIGHT then
            self.MovImage:Mirror(true)
            self.MovImage:SetClipingCoordinate(0, 8)
        elseif self.Direction == UnitDirection.DOWN then
            self.MovImage:Mirror(false)
            self.MovImage:SetClipingCoordinate(0, 6)
        end
        self.Layout:ShowImage(self.MovImageIndex)
    end
end

function Unit:ChangeDirection(direction)
    self.Direction = direction
    self:Stop()
end

function Unit:Move()
    self.Layout:MoveImage(self.MovImageIndex, self.X, self.Y)
end

function Unit:MoveLeft()
    if self.Moving == false then
        self.Direction = UnitDirection.LEFT
        if self.X > 0 then
            self.MovImage:Mirror(false)

            self.Moving = true
            self.DeltaMoving = -4
            self.MovingDirection = UnitDirection.LEFT

            return true
        end
    end

    return false
end

function Unit:MoveUp()
    if self.Moving == false then
        self.Direction = UnitDirection.UP
        if self.Y > 0 then
            self.MovImage:Mirror(false)

            self.Moving = true
            self.DeltaMoving = -4
            self.MovingDirection = UnitDirection.UP

            return true
        end
    end

    return false
end

function Unit:MoveRight()
    if self.Moving == false then
        self.Direction = UnitDirection.RIGHT

        self.MovImage:Mirror(true)

        self.Moving = true
        self.DeltaMoving = 4
        self.MovingDirection = UnitDirection.RIGHT

        return true
    end

    return false
end

function Unit:MoveDown()
    if self.Moving == false then
        self.Direction = UnitDirection.DOWN

        self.MovImage:Mirror(false)

        self.Moving = true
        self.DeltaMoving = 4
        self.MovingDirection = UnitDirection.DOWN

        return true
    end

    return false
end

function Unit:Attack()
    self.Layout:HideImage(self.MovImageIndex)
    self.Layout:MoveImage(self.AtkImageIndex, self.X - 8, self.Y - 8)
    self.Layout:ShowImage(self.AtkImageIndex)
    for i = 0, 3 do
        if self.Direction == UnitDirection.LEFT then
            self.AtkImage:Mirror(false)
            self.AtkImage:SetClipingCoordinate(0, 8 + i)
        elseif self.Direction == UnitDirection.UP then
            self.AtkImage:Mirror(false)
            self.AtkImage:SetClipingCoordinate(0, 4 + i)
        elseif self.Direction == UnitDirection.RIGHT then
            self.AtkImage:Mirror(true)
            self.AtkImage:SetClipingCoordinate(0, 8 + i)
        elseif self.Direction == UnitDirection.DOWN then
            self.AtkImage:Mirror(false)
            self.AtkImage:SetClipingCoordinate(0, i)
        end

        self.Layout:Refresh()
    end
    self:Stop()
end

function Unit:Surprise()
    self.HitSound:Play()
    self.Layout:HideImage(self.MovImageIndex)
    self.SpcImage:SetClipingCoordinate(0, 3)
    self.Layout:MoveImage(self.SpcImageIndex, self.X, self.Y)
    self.Layout:ShowImage(self.SpcImageIndex)
    self.Layout:Refresh()
    gameManager:Delay(300)
    self:Stop()
end

function Unit:Exhaust()
    self.MovImage:SetClipingCoordinate(0, 9)
    self.Layout:Refresh()
    gameManager:Delay(300)
end

function Unit:MapPosition()
    return math.floor(self.X / self.Map.TileSize.Width), math.floor(self.Y / self.Map.TileSize.Height)
end

function Unit:SetHitEvent(event)
    self.HitEvent = event
end

function Unit:SetAttackEvent(event)
    self.AttackEvent = event
end

function Unit:ResetMoveCount()
    self.CurMove = self.Character.TroopType.MoveCount
end

Unit.UnitGroupName = {
    ATK = 'ATK',
    MOV = 'MOV',
    SPC = 'SPC'
}
Unit.UnitClipingSize = {
    ATK = {64, 64},
    MOV = {48, 48},
    SPC = {48, 48}
}
Unit.UnitFilePath = '../unit.me5'
