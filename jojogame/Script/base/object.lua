Object = {}

function Object:Init()
end

-- @description DO NOT USE THIS DIRECTLY
function Object:Instance(instance)
    instance = instance or {}
    setmetatable(instance, self)
    self.__index = self

    return instance
end

function search(k, list)
    for i = 1, #list do
        local v = list[i][k]
        if v then return v end
    end
end

function Inherit(...)
    local c = {}
    local parents = {...}

    setmetatable(c, { __index = function(t, k)
        local v = search(k, parents)
        t[k] = v
        return v
    end})

    c.__parents = parents
    return c
end

