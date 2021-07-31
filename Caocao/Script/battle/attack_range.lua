require "types.lua"

---@class AttackRangeCreateOption
---@field Range table

---@class AttackRange
AttackRange = {}

---AttackRange object를 생성한다.
---@param options AttackRangeCreateOption
---@return AttackRange
function AttackRange:New(options)
    local newAttackRange = {}

    newAttackRange.Range = options.Range

    setmetatable(newAttackRange, self)
    self.__index = self

    return newAttackRange
end

---공격 범위의 절대 좌표를 계산한다.
---@field x integer @Base 절대 좌표 X
---@field y integer @Base 절대 좌표 Y
---@return table
function AttackRange:ToAbsoluteRange(x, y)
    local result = {}
    for i = 1, #self.Range.RelativeCoors do
        local data = {x + self.Range.RelativeCoors[i][1], y + self.Range.RelativeCoors[i][2]}
        data.Direction = 0
        if data[1] > x then
            data.Direction = data.Direction | UnitDirection.RIGHT
        elseif data[1] < x then
            data.Direction = data.Direction | UnitDirection.LEFT
        end
        if data[2] > y then
            data.Direction = data.Direction | UnitDirection.DOWN
        elseif data[2] < y then
            data.Direction = data.Direction | UnitDirection.UP
        end

        table.insert(result, data)
    end

    return result
end
