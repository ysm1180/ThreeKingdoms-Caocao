require "control.lua"

-- @class Window : 게임 내의 Window 컨트롤을 Wrapping 하는 클래스
-- @inherit Control
Window = Control:Instance
{
    -- @description Window 을 새로 생성합니다.
    -- @parent parent Window : null 가능, 설정되어 있다면 Modal Show 에 쓰이는 부모 Window
    -- @return Window : 새로 생성된 Window class
    New = function(self, parent)
        local newControl = {}
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
    end,

    -- @description 기존 컨트롤을 받아와 Window 클래스를 생성합니다.
    -- @param control control : 기존의 Window 컨트롤
    -- @return Window : 기존 control 의 wrapping 된 Window 클래스
    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 크기 조절 가능한지의 여부를 확인합니다.
    -- @return boolean : 크기 조절 가능 여부
    IsSizable = function(self)
        return self.control:IsSizable()
    end,

    -- @description 크기 조절이 가능한지를 설정합니다.
    -- @param value boolean : 크기 조절 가능 여부
    SetSizable = function(self, value)
        if type(value) == "boolean" then
            self.control:SetSizable(value)
        end
    end,

    SetParentWindow = function(self, window)
        if window ~= nil then
            self.control:SetParentWindow(window.control)
        end
    end,

    -- @description 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end
            
            color.R = color.R or 0
            color.G = color.G or 0
            color.B = color.B or 0
            self.control:SetBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        else
            self.control:SetBackgroundColor(gameManager:Color(0, 0, 0))
        end
    end,

    -- @description 활성화 이벤트를 설정합니다.
    -- @param event function : 활성화 이벤트
    SetActiveEvent = function(self, event)
        self.control:SetActiveEvent(event)
    end,

    -- @description 창 종료 이벤트를 설정합니다.
    -- @param event function : Close 이벤트
    SetCloseEvent = function(self, event)
        self.control:SetCloseEvent(event)
    end,

    -- @description 크기 조절 이벤트를 설정합니다.
    -- @param event function : Size 이벤트
    SetSizeEvent = function(self, event)
        self.control:SetSizeEvent(event)
    end,

    -- @description 타이틀 이름을 설정합니다.
    -- @param title string : 타이틀
    SetTitleName = function(self, title)
        self.control:SetTitleName(title)
    end,

    -- @description Modal 형태로 창을 띄웁니다.
    ShowModalWindow = function(self)
        self.control:ShowModalWindow()
    end,

    -- @description 최대화 버튼의 활성화 여부를 설정합니다.
    -- @param value boolean : 활성화 여부
    SetMaxButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMaxButton(value)
        end
    end,

    -- @description 최소화 버튼의 활성화 여부를 설정합니다.
    -- @param value boolean : 활성화 여부
    SetMinButton = function(self, value)
        if type(value) == "boolean" then
            self.control:SetMinButton(value)
        end
    end,

    -- @description 컨트롤 박스의 존재 여부를 설정합니다.
    -- @param value boolean : 존재 여부
    SetControlBox = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetControlBox(value)
        end
    end,

    -- @description 타이틀바의 존재 여부를 설정합니다.
    -- @param value boolean : 존재 여부
    SetTitlebar = function(self, value)
        if type(value) == "boolean" then        
            self.control:SetTitlebar(value)
        end
    end,

    -- @description 메뉴를 설정합니다.
    -- @param menu Menu : 메뉴
    SetMenu = function(self, menu)
        if menu then
            self.control:SetMenu(menu.control)
        end
    end,

    -- @description 레이아웃을 추가합니다
    -- @param layout Layout : 레이아웃
    AddLayout = function(self, layout)
        if layout ~= nil then
            self.control:AddLayout(layout.control)
        end
    end,

    -- @description 레이아웃을 삭제합니다.
    -- @param layout Layout : 레이아웃
    DeleteLayout = function(self, layout)
        if layout ~= nil then
            self.control:DeleteLayout(layout.control)
        end
    end,

    -- @description 화면을 Clear 한다.
    Clear = function(self)
        self.control:Clear()
    end,

    -- @description 창을 닫습니다.
    Close = function(self)
        self.control:Close()
    end,
}