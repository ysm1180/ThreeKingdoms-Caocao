---@class TroopTypeCreateOptions
---@field BattleModelIndex integer
---@field AttackRange AttackRange
---@field MoveCount integer

---@class TroopType
TroopType = {}

---TroopType object를 생성한다.
---@param options TroopTypeCreateOptions
---@return TroopType
function TroopType:New(options)
    local newTroop = {}

    newTroop.BattleModelIndex = options.BattleModelIndex
    newTroop.AttackRange = options.AttackRange
    newTroop.MoveCount = options.MoveCount

    setmetatable(newTroop, self)
    self.__index = self

    return newTroop
end
