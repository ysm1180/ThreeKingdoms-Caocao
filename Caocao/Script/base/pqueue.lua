function pqueue(cmp, initial)
    cmp = cmp or function(a, b)
        return a < b
    end
    local pq = setmetatable({}, {
        __index = {
            push = function(self, v)
                table.insert(self, v)
                local next = #self
                local prev = math.floor(next / 2)
                while next > 1 and cmp(self[next], self[prev]) do
                    -- swap up
                    self[next], self[prev] = self[prev], self[next]
                    next = prev
                    prev = math.floor(next / 2)
                end
                self[next] = v
                self.size = self.size + 1
            end,
            size = 0,
            try_pop = function(self)
                if #self >= 1 then
                    return self[1]
                else
                    return nil
                end
            end,
            pop = function(self)
                if #self < 2 then
                    return table.remove(self)
                end
                local r = self[1]
                self[1] = table.remove(self)
                local root = 1
                if #self > 1 then
                    local size = #self
                    local v = self[root]
                    while root < size do
                        local child = 2 * root
                        if child <= size then
                            if child + 1 <= size and cmp(self[child + 1], self[child]) then
                                child = child + 1
                            end
                            if cmp(self[child], self[root]) then
                                -- swap down
                                self[root], self[child] = self[child], self[root]
                                root = child
                            else
                                self[root] = v
                                break
                            end
                        else
                            self[root] = v
                            break
                        end
                    end
                end
                self.size = self.size - 1
                return r
            end
        }
    })
    for _, el in ipairs(initial or {}) do
        pq:push(el)
    end
    return pq
end
