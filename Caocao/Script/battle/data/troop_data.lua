require "../troop.lua"
require "attack_range_data.lua"

TroopData = {
    Infantry = TroopType:New({
        BattleModelIndex = 0,
        AttackRange = AttackRangeData.NearOneAttackRange,
        MoveCount = 1
    }),

    Archer = TroopType:New({
        BattleModelIndex = 18,
        AttackRange = AttackRangeData.ArcherAttackRange,
        MoveCount = 1
    }),

    Cavalry = TroopType:New({
        BattleModelIndex = 9,
        AttackRange = AttackRangeData.NearOneAttackRange,
        MoveCount = 3
    })

}
