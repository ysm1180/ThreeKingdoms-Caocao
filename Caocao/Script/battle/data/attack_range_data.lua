require "../attack_range.lua"
require "../"

AttackRangeData = {
    NearOneAttackRange = AttackRange:New({
        Range = {
            RelativeCoors = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}
        }
    }),

    ArcherAttackRange = AttackRange:New({
        Range = {
            RelativeCoors = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-2, 0}, {2, 0}, {0, 2}, {0, -2}}
        }
    })
}
