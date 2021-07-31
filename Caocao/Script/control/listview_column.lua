require "control.lua"
require "text_control.lua"

---@class ListViewColumn : ControlObject @Listview 컨트롤의 Column 을 Wrapping 한 클래스
ListViewColumn = Class(ControlObject)

-- Align
ListViewColumn.COLUMN_TEXT_ALIGN = {
    LEFT = 0,
    RIGHT = 1,
    CENTER = 2
}

---기존 컨트롤을 받아와 ListViewColumn 클래스를 생성합니다.
---@param control control @기존의 ListViewColumn 컨트롤
---@return ListViewColumn @기존 control 의 wrapping 된 ListViewColumn 클래스
function ListViewColumn:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---ListView Column 을 새로 생성합니다.
---@return ListViewColumn @새로 생성된 ListViewColumn class
function ListViewColumn:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateListViewColumn()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---Column 너비를 반환합니다.
---@return number @Column 너비
function ListViewColumn:Width()
    return self.control:GetWidth()
end

---Column 텍스트를 반환합니다.
---@return string @텍스트
function ListViewColumn:Text()
    return self.control:GetText()
end

---Column 텍스트 정렬을 설정합니다.
---@param align number @정렬, Left - 0, Right - 1, Center - 2
function ListViewColumn:SetAlign(align)
    if align ~= nil and align >= COLUMN_TEXT_ALIGN.LEFT and align <= COLUMN_TEXT_ALIGN.CENTER then
        self.control:SetAlign(align)
    end
end

---Column 너비를 설정합니다.
---@param width number @너비
function ListViewColumn:SetWidth(width)
    if width ~= nil then
        self.control:SetWidth(width)
        self.Width = width
    end
end

---Column 텍스트를 설정합니다.
---@param text string @텍스트
function ListViewColumn:SetText(text)
    if text ~= nil then
        self.control:SetText(text)
    end
end

---아이템에 너비 자동 맞춤 여부를 설정합니다.
---@param value boolean @자동 맞춤 여부
function ListViewColumn:SetAutoSizeFitItem(value)
    if type(value) == "boolean" then
        self.control:SetAutoSizeFitItem(value)
    end
end

---Column Text에 너비 자동 맞춤 여부를 설정합니다.
---@param value boolean @자동 맞춤 여부
function ListViewColumn:SetAutoSizeFitHeader(value)
    if type(value) == "boolean" then
        self.control:SetAutoSizeFitHeader(value)
    end
end
