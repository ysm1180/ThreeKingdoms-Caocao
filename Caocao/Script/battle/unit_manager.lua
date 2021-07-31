require "../control/layout_manager.lua"
require "unit.lua"

---@class UnitManager
UnitManager = {
    Layout = LayoutManager:CreateLayout({}),
    Units = {}
}

function UnitManager:CreateUnit(options)
    local unit = Unit:New(options.Map, options.Character, self.Layout, options.UnitType, options.Direction)
    unit:Show(options.X, options.Y)

    table.insert(self.Units, unit)

    return unit
end

function UnitManager:Update()
    for i = 1, #self.Units do
        local unit = self.Units[i]
        if unit.Moving then
            if unit.MovingDirection == UnitDirection.UP or unit.MovingDirection == UnitDirection.DOWN then
                if math.abs(unit.DeltaY) >= unit.Map.TileSize.Height then
                    unit.Moving = false
                    unit.DeltaMoving = 0
                    unit.DeltaY = 0
                    unit:Stop()
                end

                if unit.DeltaY % 12 == 0 then
                    local tick = math.floor(unit.DeltaY / 12) % 2

                    if unit.MovingDirection == UnitDirection.UP then
                        if tick == 0 then
                            unit.MovImage:SetClipingCoordinate(0, 2)
                        elseif tick == 1 then
                            unit.MovImage:SetClipingCoordinate(0, 3)
                        end
                    elseif unit.MovingDirection == UnitDirection.DOWN then
                        if tick == 0 then
                            unit.MovImage:SetClipingCoordinate(0, 0)
                        elseif tick == 1 then
                            unit.MovImage:SetClipingCoordinate(0, 1)
                        end
                    end
                end

                unit.Y = unit.Y + unit.DeltaMoving
                unit.DeltaY = unit.DeltaY + unit.DeltaMoving
            elseif unit.MovingDirection == UnitDirection.LEFT or unit.MovingDirection == UnitDirection.RIGHT then
                if math.abs(unit.DeltaX) >= unit.Map.TileSize.Width then
                    unit.Moving = false
                    unit.DeltaMoving = 0
                    unit.DeltaX = 0
                    unit:Stop()
                end

                if unit.DeltaX % 12 == 0 then
                    local tick = math.floor(unit.DeltaX / 12) % 2
                    if tick == 0 then
                        unit.MovImage:SetClipingCoordinate(0, 4)
                    elseif tick == 1 then
                        unit.MovImage:SetClipingCoordinate(0, 5)
                    end
                end

                unit.X = unit.X + unit.DeltaMoving
                unit.DeltaX = unit.DeltaX + unit.DeltaMoving
            end

            unit:Move()
        end
    end
end
