require "control.lua"

---@class Menu : ControlObject @윈도우 창의 메뉴를 Wrapping 하는 클래스
Menu = Class(ControlObject)

---Menu 를 새로 생성합니다.
---@return Menu
function Menu:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateMenu()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---메뉴 아이템을 추가합니다.
---@param menuItem MenuItem @추가할 메뉴 아이템
function Menu:AddMenuItem(menuItem)
    if menuItem ~= nil then
        self.control:AddMenuItem(menuItem.control)
    end
end
