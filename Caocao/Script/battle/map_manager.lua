require "../control/layout_manager.lua"
require "unit_manager.lua"
require "map.lua"

---@class MapManager
MapManager = {
    Layout = LayoutManager:CreateLayout({}),
    Units = {},
    Map = nil
}

---맵 object를 생성한다.
---@param index number @Map index
---@return Map
function MapManager:CreateMap(index)
    local map = Map:New(index, self.Layout)
    for x = 0, map:Width() do
        self.Units[x] = {}
        for y = 0, map:Height() do
            self.Units[x][y] = nil
        end
    end
    self.Map = map
    return map
end

function MapManager:CreateUnit(options)
    options.UnitType = UnitType.Mine

    local unit = UnitManager:CreateUnit(options)
    self.Units[options.X][options.Y] = unit
    return unit
end

function MapManager:CreateEnemy(options)
    options.ModelIndex = options.Character:GetBattleModelIndex()
    options.UnitType = UnitType.Enemy

    local unit = UnitManager:CreateUnit(options)
    self.Units[options.X][options.Y] = unit
    return unit
end

function MapManager:Update()
    if self.Map ~= nil then
        for x = 0, self.Map:MapWidth() do
            for y = 0, self.Map:MapHeight() do
                if self.Units[x][y] ~= nil then
                    local unitX, unitY = self.Units[x][y]:MapPosition()
                    if unitX ~= x or unitY ~= y then
                        local unit = self.Units[x][y]
                        self.Units[x][y] = nil
                        self.Units[unitX][unitY] = unit
                    end
                end
            end
        end
    end
end

function MapManager:MoveUnit(unit, direction)
    local x, y = unit:MapPosition()
    local result = false

    if direction == UnitDirection.LEFT then
        if self.Units[x - 1][y] == nil or self.Units[x - 1][y].IsHide then
            result = unit:MoveLeft()
        else
            unit:ChangeDirection(UnitDirection.LEFT)
        end
    elseif direction == UnitDirection.UP then
        if self.Units[x][y - 1] == nil or self.Units[x][y - 1].IsHide then
            result = unit:MoveUp()
        else
            unit:ChangeDirection(UnitDirection.UP)
        end
    elseif direction == UnitDirection.RIGHT then
        if self.Units[x + 1][y] == nil or self.Units[x + 1][y].IsHide then
            result = unit:MoveRight()
        else
            unit:ChangeDirection(UnitDirection.RIGHT)
        end
    elseif direction == UnitDirection.DOWN then
        if self.Units[x][y + 1] == nil or self.Units[x][y + 1].IsHide then
            result = unit:MoveDown()
        else
            unit:ChangeDirection(UnitDirection.DOWN)
        end
    end
    return result
end

function MapManager:GetUnitByCoor(x, y)
    return self.Units[x][y]
end

function MapManager:AttackUnit(unit)
    local x, y = unit:MapPosition()
    local direction = unit.Direction

    unit:Attack()
    unit:AttackEvent()
    local absoluteAttackRange = unit.Character.TroopType.AttackRange:ToAbsoluteRange(x, y)
    for i = 1, #absoluteAttackRange do
        if direction & absoluteAttackRange[i].Direction ~= 0 then
            local hitUnit = self.Units[absoluteAttackRange[i][1]][absoluteAttackRange[i][2]]
            if hitUnit ~= nil and hitUnit.IsHide == false then
                hitUnit:HitEvent(unit)
            end
        end
    end
end
