require "combobox.lua"

-- @class ComboBoxMananger
-- @description ComboBox class 를 관리하는 Manager class
-- @inherit Object
ComboBoxManager = Object:Instance 
{
    -- @param options ComboBoxOptions : ComboBox 생성 옵션
    -- @return ComboBox : 생성된 ComboBox 클래스
    -- @type ComboBoxOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 ComboBox class 를 생성한다.
            Parent Window : control 이 생성될 부모 class 지정
            Width int : control 너비
            Height int : control 높이
            X int : X 위치
            Y int : Y 위치
            Event table : {
                Create function : 생성 이벤트
                Destroy function : 파괴 이벤트
                MouseLButtonUp function : 마우스 왼쪽 버튼 눌렀다 땠을 때 이벤트
                MouseLButtonDown function : 마우스 왼쪽 버튼 눌렀을 때 이벤트
                MouseMove function : 마우스 움직일 때 이벤트
                MouseEnter function : 마우스 진입 이벤트
                MouseLeave function : 마우스 벗어났을 때 이벤트
            }
            Items string[] : ComboBox 아이템들
            UserData string : 임의 데이터
            Show boolean : 생성 후 Show 여부
        }
    ]]
    Create = function(self, options)
        local newControl = nil

        if options ~= nil then

            if options._Control ~= nil then
                return ComboBox:Instance(options._Control)
            end
            
            newControl = ComboBox:New(options.Parent)

            -- 크기 설정
            newControl:SetSize(options.Width, options.Height)

            -- 위치 설정
            newControl:Move(options.X, options.Y)

            -- 이벤트 설정
            if options.Event ~= nil then
                newControl:SetCreateEvent(options.Event.Create)
                newControl:SetDestroyEvent(options.Event.Destroy)
                newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newControl:SetMouseMoveEvent(options.Event.MouseMove)
                newControl:SetMouseEnterEvent(options.Event.MouseEnter)
                newControl:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            -- 아이템 설정
            if options.Items ~= nil then
                for i = 1, #options.Items do
                    newControl:AddItem(options.Items[i])
                end
            end

            -- 임의 데이터 설정
            newControl:SetUserData(options.UserData)

            -- 생성
            newControl:Create()

            -- Show
            if options.Show then
                newControl:Show()
            end
        end

        return newControl
    end,

    -- @description ComboBox 컨트롤을 ComboBox class 로 wrapping 하여 생성
    -- @param control control : ComboBox control
    -- @return ComboBox : ComboBox control -> ComboBox class
    CreateInstance = function(self, control)
        local options = {
            _Control = control,
        }
        
        return self:Create(options)
    end
}
