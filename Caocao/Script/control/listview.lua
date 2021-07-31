require "control.lua"
require "text_control.lua"

---@class ListView : ChildControl @게임 내의 ListView 컨트롤을 Wrapping 하는 클래스
ListView = Class(ChildControl)

---기존 컨트롤을 받아와 ListView 클래스를 생성합니다.
---@param control any
---@return ListView 
function ListView:Instance(control)
    local newControl = {}

    newControl.control = control
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---ListView 을 새로 생성합니다.
---@param parent Window @ListView 가 생성될 부모 Window
---@return ListView 
function ListView:New(parent)
    local newControl = {}
    local parentControl = nil

    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateListView(parentControl)
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---생성
function ListView:Create()
    self.control:Create()
end

---파괴
function ListView:Destroy()
    self.control:Destroy()
end

---해당 위치의 Column 을 반환합니다.
---반환된 값을 사용하려면 ListViewColumn class 로 wrapping 한 다음 사용! (ListViewManager:CreateColumnInstance)
---@param columnIndex integer @Column 위치, 1부터 시작
---@return any
function ListView:Column(columnIndex)
    return self.control:GetColumn(columnIndex)
end

---해당 위치의 Row 을 반환합니다.
---반환된 값을 사용하려면 ListViewRow class 로 wrapping 한 다음 사용! (ListViewManager:CreateRowInstance)
---@param rowIndex integer @Row 위치, 1부터 시작
---@return any
function ListView:Row(rowIndex)
    return self.control:GetRow(rowIndex)
end

---테두리를 보여줄지 여부를 설정합니다.
---@param value boolean
function ListView:SetShowBorder(value)
    if type(value) == "boolean" then
        self.control:SetShowBorder(value)
    end
end

---테두리를 보여줄지 여부를 설정합니다.
---@param value boolean
function ListView:SetShowColumn(value)
    if type(value) == "boolean" then
        self.control:SetShowColumn(value)
    end
end

---Column을 보여줄지 여부를 설정합니다.
---@param value boolean
function ListView:SetSortClickedColumn(value)
    if type(value) == "boolean" then
        self.control:SetSortClickedColumn(value)
    end
end

---한번 클릭 시 활성화 여부를 설정합니다.
---@param value boolean 
function ListView:SetOneClickItemActivated(value)
    if type(value) == "boolean" then
        self.control:SetOneClickItemActivated(value)
    end
end

---자동 선택 여부를 설정합니다.
---@param value boolean 
function ListView:SetTrackingSelect(value)
    if type(value) == "boolean" then
        self.control:SetTrackingSelect(value)
    end
end

---배경 투명화 여부를 설정합니다.
---@param value boolean
function ListView:SetTransparentBackground(value)
    if type(value) == "boolean" then
        self.control:SetTransparentBackground(value)
    end
end

---Row 높이를 설정합니다.
---@param height integer
function ListView:SetRowHeight(height)
    if height ~= nil then
        self.control:SetRowHeight(height)
    end
end

---열을 추가합니다.
---@param column ListViewColumn 
function ListView:AddColumn(column)
    if column ~= nil then
        self.control:AddColumn(column.control)
    end
end

---행을 추가합니다.
---@param row ListViewRow
function ListView:AddRow(row)
    if row ~= nil then
        self.control:AddRow(row.control)
    end
end

