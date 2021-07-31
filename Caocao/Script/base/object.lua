---@class ControlObject @control을 가진 클래스
---@field control EngineBaseControl
ControlObject = {
    control = nil
}

function search(k, list)
    for i = 1, #list do
        local v = list[i][k]
        if v then
            return v
        end
    end
end

function Class(...)
    local c = {}
    local parents = {...}

    setmetatable(c, {
        __index = function(t, k)
            local v = search(k, parents)
            t[k] = v
            return v
        end
    })

    c.__parents = parents
    return c
end
