require "window.lua"

---@class WindowCreateOption
--[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
            Parent Window : control 이 생성될 부모 control object 지정
            Width number : control 너비
            Height number : control 높이
            Center boolean : true 면 부모의 가운데로 이동
            X number : X 위치
            Y number : Y 위치
            TitleName string : 타이틀 이름
            Titlebar boolean : 타이틀바 존재 여부
            MaxButton boolean : 최대화 버튼 활성화 여부
            MinButton boolean : 최소화 버튼 활성화 여부
            ControlBox boolean : 최소화, 최대화, X 버튼 존재 여부
            Sizable boolean : 크기 조절 가능 여부
            Event table : {
                Create function : 생성 이벤트
                Destroy function : 파괴 이벤트
                MouseLButtonUp function : 마우스 왼쪽 버튼 눌렀다 땠을 때 이벤트
                MouseLButtonDown function : 마우스 왼쪽 버튼 눌렀을 때 이벤트
                MouseMove function : 마우스 움직일 때 이벤트
                MouseEnter function : 마우스 진입 이벤트
                MouseLeave function : 마우스 벗어났을 때 이벤트
                Active function: Window 가 활성화 됐을 때 이벤트
                Close function : 창이 종료될 때 이벤트, 이 이벤트에서 true 를 반환하면 종료 취소
                Size function : 크기를 조절할 때 이벤트
            }
            Layouts Layout[] : 창에 추가할 Layouts
            Menu Menu : 추가할 메뉴
            Background table : {
                Color number : 배경 색
            }
            Show boolean : 생성 후 Show 여부
            Modal boolean : true 시 Show 된 후 Parent class 비활성화 & 종료될 때까지 코드 진행 X
        }
    ]]

---@class WindowManager @Window object를 관리하는 Manager class
WindowManager = {}

---정 가운데로 이동할 위치를 반환한다.
---@param parentControl any @기준 위치를 지정할 class
---@param w integer @현재 컨트롤의 너비
---@param h integer @현재 컨트롤의 높이
---@return integer, integer @X, Y 위치
function WindowManager:GetCenterPosition(parentControl, w, h)
    if w == nil or h == nil then
        return nil, nil
    end

    -- isChildControl Dafault : true
    if isChiildControl == nil then
        isChiildControl = true
    end

    -- parentControl 지정 안했을 시 데스크탑 크기가 기준이 됨
    if parentControl ~= nil then
        parentWidth, parentHeight = parentControl:Size()
        parentX, parentY = parentControl:Position()
    else
        parentWidth = gameManager:GetDesktopWidth()
        parentHeight = gameManager:GetDesktopHeight()
        parentX, parentY = 0, 0
    end

    local x = parentWidth / 2 - w / 2 + parentX
    local y = parentHeight / 2 - h / 2 + parentY

    return x, y
end

---Window object를 생성합니다.
---@param options WindowCreateOption
---@return Window
function WindowManager:Create(options)
    local newControl = nil
    if options ~= nil then
        if options.Control ~= nil then
            return Window:Instance(options._Control)
        end

        newControl = Window:New(options.Parent)

        -- 크기 설정
        newControl:SetSize(options.Width, options.Height)

        -- 가운데
        if options.Center then
            local x, y = self:GetCenterPosition(options.Parent, options.Width, options.Height, false)
            if x ~= nil and y ~= nil then
                options.X = x
                options.Y = y
            end
        end

        -- 위치 설정
        newControl:Move(options.X, options.Y)

        -- 타이틀 설정
        newControl:SetTitleName(options.TitleName)
        newControl:SetTitlebar(options.Titlebar)

        -- 배경 설정
        if options.BackgroundColor ~= nil then
            newControl:SetBackgroundColor(options.BackgroundColor)
        end

        -- 컨트롤바 설정
        newControl:SetMaxButton(options.MaxButton)
        newControl:SetMinButton(options.MinButton)
        if options.ControlBox == nil then
            options.ControlBox = true
        end
        newControl:SetControlBox(options.ControlBox)

        newControl:SetSizable(options.Sizable)

        -- 이벤트 설정
        if options.Event then
            newControl:SetCreateEvent(options.Event.Create)
            newControl:SetDestroyEvent(options.Event.Destroy)
            newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
            newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
            newControl:SetMouseMoveEvent(options.Event.MouseMove)
            newControl:SetMouseEnterEvent(options.Event.MouseEnter)
            newControl:SetMouseLeaveEvent(options.Event.MouseLeave)

            newControl:SetActiveEvent(options.Event.Active)
            newControl:SetCloseEvent(options.Event.Close)
            newControl:SetSizeEvent(options.Event.Size)
        end

        -- 레이아웃 설정
        if options.Layouts ~= nil then
            for i = 1, #options.Layouts do
                newControl:AddLayout(options.Layouts[i])
            end
        end

        -- 메뉴 설정
        newControl:SetMenu(options.Menu)

        -- 생성
        newControl:Create()

        -- Show
        if options.Show then
            if options.Modal then
                newControl:ShowModalWindow()
            else
                newControl:Show()
            end
        end
    end

    return newControl
end

---@return Window
function WindowManager:CreateInstance(control)
    local options = {
        _Control = control
    }
    return self:Create(options)
end

