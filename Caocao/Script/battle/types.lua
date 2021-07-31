---@class UnitDirection
---@field LEFT integer 1 << 0
---@field UP integer 1 << 1
---@field RIGHT integer 1 << 2
---@field DOWN integer 1 << 3
UnitDirection = {LEFT = 1, UP = 2, RIGHT = 4, DOWN = 8}

---@class UnitType
---@field Player integer
---@field Ally integer
---@field Enemy integer
UnitType = {Player = 0, Ally = 1, Enemy = 2}
