require "../character.lua"
require "troop_data.lua"

CharacterData = {Character:New({
    Name = "주인공",
    TroopType = TroopData.Cavalry,
    SpecialBattleModelIndex = 108,
    InitHP = 100,
    Power = 100,
    Defense = 50
}), Character:New({
    Name = "적군_보병",
    TroopType = TroopData.Infantry,
    InitHP = 100,
    Power = 60,
    Defense = 70
}), Character:New({
    Name = "적군_궁수",
    TroopType = TroopData.Archer,
    InitHP = 50,
    Power = 80,
    Defense = 30
})}
