require "control.lua"

---@class Window : Control @게임 내의 Window 컨트롤을 Wrapping 하는 클래스
Window = Class(Control)

---@type Toolbar
Window.Toolbar = nil

---Window 을 새로 생성합니다.
---@parent parent Window : null 가능, 설정되어 있다면 Modal Show 에 쓰이는 부모 Window
---@return Window
function Window:New(parent)
    local newControl = {
        Toolbar = nil
    }
    local parentControl = nil

    if parent ~= nil then
        parentControl = parent.control
    end

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateWindowForm(parentControl)
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

function Window:Height()
    return self.control:GetLuaHeight()
end

---크기 조절 가능한지의 여부를 확인합니다.
---@return boolean
function Window:IsSizable()
    return self.control:IsSizable()
end

---크기 설정
---@param width integer
---@param height integer
function Window:SetSize(width, height)
    if width ~= nil then
        self.control:SetWidth(width)
    end
    if height ~= nil then
        self.control:SetLuaHeight(height)
    end
end

---크기 조절이 가능한지를 설정합니다.
---@param value boolean
function Window:SetSizable(value)
    if type(value) == "boolean" then
        self.control:SetSizable(value)
    end
end

---부모 윈도우를 설정합니다.
function Window:SetParentWindow(window)
    if window ~= nil then
        self.control:SetParentWindow(window.control)
    end
end

---배경 색을 설정합니다.
---@param color integer | RGB @색
function Window:SetBackgroundColor(color)
    if color ~= nil then
        if type(color) == "number" then
            color = {
                R = color % 0x100,
                G = math.floor(color / 0x100) % 0x100,
                B = math.floor(color / 0x10000) % 0x100
            }
        end

        color.R = color.R or 0
        color.G = color.G or 0
        color.B = color.B or 0
        self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    else
        self.control:SetBackgroundColor(gameManager:Color(0, 0, 0))
    end
end

---활성화 이벤트를 설정합니다.
---@param event function 
function Window:SetActiveEvent(event)
    self.control:SetActiveEvent(event)
end

---창 종료 이벤트를 설정합니다.
---@param event function 
function Window:SetCloseEvent(event)
    self.control:SetCloseEvent(event)
end

---크기 조절 이벤트를 설정합니다.
---@param event function 
function Window:SetSizeEvent(event)
    self.control:SetSizeEvent(event)
end

---KEY DOWN 이벤트를 설정합니다.
---@param event function 
function Window:SetKeyDownEvent(event)
    self.control:SetKeyDownEvent(event)
end

---KEY UP 이벤트를 설정합니다.
---@param event function
function Window:SetKeyUpEvent(event)
    self.control:SetKeyUpEvent(event)
end

---타이틀 이름을 설정합니다.
---@param title string 
function Window:SetTitleName(title)
    self.control:SetTitleName(title)
end

---Modal 형태로 창을 띄웁니다.
function Window:ShowModalWindow()
    self.control:ShowModalWindow()
end

---최대화 버튼의 활성화 여부를 설정합니다.
---@param value boolean
function Window:SetMaxButton(value)
    if type(value) == "boolean" then
        self.control:SetMaxButton(value)
    end
end

---최소화 버튼의 활성화 여부를 설정합니다.
---@param value boolean
function Window:SetMinButton(value)
    if type(value) == "boolean" then
        self.control:SetMinButton(value)
    end
end

---컨트롤 박스의 존재 여부를 설정합니다.
---@param value boolean
function Window:SetControlBox(value)
    if type(value) == "boolean" then
        self.control:SetControlBox(value)
    end
end

---타이틀바의 존재 여부를 설정합니다.
---@param value boolean
function Window:SetTitlebar(value)
    if type(value) == "boolean" then
        self.control:SetTitlebar(value)
    end
end

---메뉴를 설정합니다.
---@param menu Menu 
function Window:SetMenu(menu)
    if menu then
        self.control:SetMenu(menu.control)
    end
end

---레이아웃을 추가합니다
---@param layout Layout
function Window:AddLayout(layout)
    if layout ~= nil then
        if self.Toolbar ~= nil and self.Toolbar.Visible then
            layout:Move(nil, layout:Y() + self.Toolbar:Height(), false)
        end
        if isShow == nil then
            isShow = true
        end
        self.control:AddLayout(layout.control, isShow)
    end
end

---레이아웃을 삭제합니다.
---@param layout Layout
function Window:DeleteLayout(layout)
    if layout ~= nil then
        self.control:DeleteLayout(layout.control)
    end
end

---해당 Window 에 추가된 레이아웃을 모두 제거합니다.
function Window:Clear()
    self.control:Clear()
end

---창을 닫습니다.
function Window:Close()
    self.control:Close()
end

---정 가운데로 이동할 위치를 반환한다.
---@param parentControl any @기준 위치를 지정할 class
function Window:Center(parentControl)
    -- parentControl 지정 안했을 시 데스크탑 크기가 기준이 됨
    local parentWidth, parentHeight, parentX, parentY
    if parentControl ~= nil then
        parentWidth, parentHeight = parentControl:Size()
        parentX, parentY = parentControl:Position()
    else
        parentWidth = gameManager:GetDesktopWidth()
        parentHeight = gameManager:GetDesktopHeight()
        parentX, parentY = 0, 0
    end

    local x = parentWidth / 2 - self:Width() / 2 + parentX
    local y = parentHeight / 2 - self:Height() / 2 + parentY

    self:Move(x, y)
end

