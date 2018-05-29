require "control_manager.lua"
require "radiobutton.lua"

-- @class RadioButtonMananger
-- @description RadioButton class 를 관리하는 Manager class
-- @inherit ControlManager
RadioButtonManager = ControlManager:Instance 
{
    -- @description RadioButton class 를 생성합니다.
    -- @param options RadioButtonOptions : 버튼 생성 옵션
    -- @return RadioButotn : 생성된 RadioButton class
    -- @type RadioButtonOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
            Parent Window : control 이 생성될 부모 Class 지정
            Width int : control 너비
            Height int : control 높이
            Center boolean : true 면 부모의 가운데로 이동
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
            Text table : {
                Font table : {
                    Name string : 폰트 이름
                    Size int : 폰트 크기
                    Bold boolean : 폰트 Bold 여부
                    Underline boolean : 폰트 밑줄 여부
                    Italic boolean : 폰트 기울임 여부
                }
                Content string : 텍스트 내용
            }
            UserData string : 임의 데이터
            Checked boolean : 체크 여부
            Show boolean : 생성 후 Show 여부
        }
    ]]
    Create = function(self, options)
        local newControl = nil

        if options ~= nil then
            if options._Control ~= nil then
                return RadioButton:Instance(options._Control)
            end
            
            newControl = RadioButton:New(options.Parent)

            -- @description 크기 설정
            newControl:SetSize(options.Width, options.Height)

            -- @description 가운데
            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end

            -- @description 위치 설정
            newControl:Move(options.X, options.Y)

            -- @description 이벤트 설정
            if options.Event ~= nil then
                newControl:SetCreateEvent(options.Event.Create)
                newControl:SetDestroyEvent(options.Event.Destroy)
                newControl:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newControl:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newControl:SetMouseMoveEvent(options.Event.MouseMove)
                newControl:SetMouseEnterEvent(options.Event.MouseEnter)
                newControl:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            -- @description 텍스트 설정
            if options.Text ~= nil then    
                if options.Text.Font ~= nil then
                    newControl:SetFontName(options.Text.Font.Name)
                    newControl:SetFontSize(options.Text.Font.Size)
                    newControl:SetBold(options.Text.Font.Bold)
                    newControl:SetUnderline(options.Text.Font.Underline)
                    newControl:SetItalic(options.Text.Font.Italic)
                end

                -- @description if options.Text.Color ~= nil then
                -- @description     if options.Text.Color.R ~= nil and options.Text.Color.G ~= nil and options.Text.Color.B ~= nil then
                -- @description         newButton:SetNormalTextColor(options.Text.Color)                        
                -- @description         newButton:SetFocusedTextColor(options.Text.Color)
                -- @description         newButton:SetPushedTextColor(options.Text.Color)                    
                -- @description     else
                -- @description         newButton:SetNormalTextColor(options.Text.Color.Normal)
                -- @description         newButton:SetFocusedTextColor(options.Text.Color.Focused)
                -- @description         newButton:SetPushedTextColor(options.Text.Color.Pushed)
                -- @description     end
                -- @description end

                newControl:SetText(options.Text.Content)
            end

            -- @description 임의 데이터 설정
            newControl:SetUserData(options.UserData)

            -- @description 생성
            newControl:Create()

            -- @description 체크 여부 설정
            newControl:SetChecked(options.Checked)

            -- @description Show
            if options.Show then
                newControl:Show()
            end
        end
        return newControl
    end,

    -- @description RadioButton 컨트롤을 RadioButton class 로 wrapping 하여 생성
    -- @param control control : RadioButton control
    -- @return RadioButton : RadioButton control -> RadioButton class
    CreateInstance = function(self, control)
        local options = {
            _Control = control,
        }

        return self:Create(options)
    end
}
