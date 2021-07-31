---@class CharacterCreateOption
---@field Level integer | nil
---@field Name string
---@field TroopType TroopType
---@field SpecialBattleModelIndex integer
---@field InitHP integer
---@field Power integer
---@field Defense integer

---@class Character
Character = {}

---캐릭터 object를 생성한다.
---@param options CharacterCreateOption
---@return Character
function Character:New(options)
    local newCharacter = {}

    if options.Level == nil then
        options.Level = 1
    end

    newCharacter.Name = options.Name
    newCharacter.Level = options.Level
    newCharacter.TroopType = options.TroopType
    newCharacter.SpecialBattleModelIndex = options.SpecialBattleModelIndex
    newCharacter.InitHP = options.InitHP
    newCharacter.Power = options.Power
    newCharacter.Defense = options.Defense

    setmetatable(newCharacter, self)
    self.__index = self

    return newCharacter
end

---캐릭터의 Model index를 불러온다.
---@return integer
function Character:GetBattleModelIndex()
    if self.SpecialBattleModelIndex ~= nil then
        return self.SpecialBattleModelIndex
    end

    return self.TroopType.BattleModelIndex
end

---캐릭터의 HP를 가져온다.
---@return integer
function Character:GetHP()
    return self.InitHP
end

function Character:GetPower()
    return self.Power
end

function Character:GetDefense()
    return self.Defense
end
