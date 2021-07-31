require "../base/pqueue.lua"

---@class TimeManager
TimeManager = {
    queue = nil,
    taskId = 0,
    cancelList = {}
}

function TimeManager:Init()
    self.queue = pqueue(function(e1, e2)
        return e1.timestamp < e2.timestamp
    end)
    self.cancelList = {}
end

function TimeManager:GetTime()
    return gameManager:Clock()
end

function TimeManager:GetSecond()
    return self:GetTime() / 1000
end

function TimeManager:AddPeriodicTask(interval, caller, taskFunc)
    local entry = {}

    self.taskId = self.taskId + 1

    entry.timestamp = self:GetSecond()
    entry.taskFunc = taskFunc
    entry.caller = caller
    entry.interval = interval
    entry.taskId = self.taskId

    self.queue:push(entry)
    return entry.taskId
end

function TimeManager:AddTask(afterSec, caller, taskFunc)
    local entry = {}

    self.taskId = self.taskId + 1

    entry.timestamp = self:GetSecond() + afterSec
    entry.taskFunc = taskFunc
    entry.caller = caller
    entry.taskId = self.taskId
    entry.interval = 0

    self.queue:push(entry)
    return entry.taskId
end

function TimeManager:OnTick()
    local top = self.queue:try_pop()
    local now = self:GetSecond()

    if top == nil then
        return
    end

    while top ~= nil do
        if top.timestamp > now then
            return
        end
        top = self.queue:pop()

        if self.cancelList[top.taskId] ~= true then
            -- return이 true인 경우 periodic이 cancel된다
            local ret = top.taskFunc(top.caller) or false

            if top.interval > 0 and ret ~= true then
                top.timestamp = top.timestamp + top.interval

                if top.timestamp < now then
                    top.timestamp = now
                end

                self.queue:push(top)
            end
        else
            self.cancelList[top.taskId] = nil
        end

        top = self.queue:try_pop()
    end
end

