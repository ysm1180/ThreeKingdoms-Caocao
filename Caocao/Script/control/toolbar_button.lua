require "control.lua"

---@class ToolbarButton : ControlObject
ToolbarButton = Class(ControlObject)

---툴바 버튼을 새로 생성합니다.
---@return ToolbarButton 
function ToolbarButton:New()
    local newControlButton = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControlButton.control = controlManager:CreateToolbarButton()
    setmetatable(newControlButton, self)
    self.__index = self

    return newControlButton
end

---텍스트를 반환합니다.
---@return string
function ToolbarButton:Text()
    return self.control:GetText()
end

---버튼을 활성화합니다.
function ToolbarButton:Enable()
    self.control:SetEnabled(true)
end

---버튼을 비활성화합니다.
function ToolbarButton:Disable()
    self.control:SetEnabled(false)
end

---버튼의 이미지를 설정합니다
---@param image Image 
function ToolbarButton:SetImage(image)
    if image ~= nil then
        self.control:SetImage(image.control)
    else
        self.control:SetImage(nil)
    end
end

---버튼의 텍스트를 설정합니다.
---@param text string 
function ToolbarButton:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---버튼 클릭 이벤트를 설정합니다.
---@param event function
function ToolbarButton:SetClickEvent(event)
    if event ~= nil then
        self.control:SetClickEvent(event)
    end
end

---버튼 툴팁 내용을 설정합니다.
---@param tooltip string 
function ToolbarButton:SetTooltipText(tooltip)
    if tooltip ~= nil then
        self.control:SetTooltipText(tooltip)
    end
end

-- ToolbarButton을 생성합니다.
function ToolbarButton:Create()
    self.control:Create()
end
