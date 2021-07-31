require "unit.lua"

UnitManager = {
    Layout = LayoutManager:CreateLayout({}),

    Units = {}
}

function UnitManager:CreateUnit(index, x, y)
    local unit = Unit:New(index)
    unit:Show(self.Layout, x, y)
    return unit
end
