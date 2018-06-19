require "button.lua"

-- @class ButtonManager
-- @description Button Class 를 관리하는 Manager class
-- @inherit Object
ButtonManager = Object:Instance 
{
    -- @description Button class 를 생성합니다.
    -- @param options ButtonOptions : control 생성 옵션
    -- @return Button : 생성된 Button class
    -- @type ButtonOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
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
            Text table : {
                Font table : {
                    Name string : 폰트 이름
                    Size int : 폰트 크기
                    Bold boolean : 폰트 Bold 여부
                    Underline boolean : 폰트 밑줄 여부
                    Italic boolean : 폰트 기울임 여부
                }
                Color int | table : int 일 땐 해당 색으로 모든 색 통일, table 이면 {
                    Normal int : 기본 색
                    Focused int : 마우스 올라갔을 때 색
                    Pushed int : 눌렸을 때 색
                }
                Content string : 텍스트 내용
            }
            Border table : {
                Width int : 테두리 굵기
                Color int | table : int 일 땐 모든 색 통일, table 일 땐 {
                    Normal : 기본 색
                    Focused : 마우스를 올렸을 때 색
                    Pushed : 눌렸을 때 색
                }
            }
            Background table : {
                Color int | table : int 일 땐 모든 색 통일, table 일 땐 {
                    Normal : 기본 색
                    Focused : 마우스를 올렸을 때 색
                    Pushed : 눌렸을 때 색
                }
                Transparent boolean : 배경 투명화 여부
            }
            UserData string : 임의 데이터
            Show boolean : 생성 후 Show 여부
        }
    ]]
    Create = function(self, options)
        local newControl = nil

        if options ~= nil then

            if options._Control ~= nil then
                return Button:Instance(options._Control)
            end
            
            newControl = Button:New(options.Parent)

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

            -- 텍스트 설정
            if options.Text ~= nil then    
                if options.Text.Font ~= nil then
                    newControl:SetFontName(options.Text.Font.Name)
                    newControl:SetFontSize(options.Text.Font.Size)
                    newControl:SetBold(options.Text.Font.Bold)
                    newControl:SetUnderline(options.Text.Font.Underline)
                    newControl:SetItalic(options.Text.Font.Italic)
                end
                if options.Text.Color ~= nil then
                    if options.Text.Color.R ~= nil and options.Text.Color.G ~= nil and options.Text.Color.B ~= nil then
                        newControl:SetNormalTextColor(options.Text.Color)                        
                        newControl:SetFocusedTextColor(options.Text.Color)
                        newControl:SetPushedTextColor(options.Text.Color)                    
                    else
                        newControl:SetNormalTextColor(options.Text.Color.Normal)
                        newControl:SetFocusedTextColor(options.Text.Color.Focused)
                        newControl:SetPushedTextColor(options.Text.Color.Pushed)
                    end
                end
                newControl:SetText(options.Text.Content)
            end

            -- 테두리 설정
            if options.Border ~= nil then
                newControl:SetBorderWidth(options.Border.Width)
                if options.Border.Color ~= nil then
                    if options.Border.Color.R ~= nil and options.Border.Color.G ~= nil and options.Border.Color.B ~= nil then
                        newControl:SetNormalBorderColor(options.Border.Color)
                        newControl:SetFocusedBorderColor(options.Border.Color)
                        newControl:SetPushedBorderColor(options.Border.Color)
                    else
                        newControl:SetNormalBorderColor(options.Border.Color.Normal)
                        newControl:SetFocusedBorderColor(options.Border.Color.Focused)
                        newControl:SetPushedBorderColor(options.Border.Color.Pushed)
                    end
                end
            end

            -- 배경 설정
            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    if options.Background.Color.R ~= nil and options.Background.Color.G ~= nil and options.Background.Color.B ~= nil then
                        newControl:SetNormalBackgroundColor(options.Background.Color)
                        newControl:SetFocusedBackgroundColor(options.Background.Color)
                        newControl:SetPushedBackgroundColor(options.Background.Color)
                    else
                        newControl:SetNormalBackgroundColor(options.Background.Color.Normal)
                        newControl:SetFocusedBackgroundColor(options.Background.Color.Focused)
                        newControl:SetPushedBackgroundColor(options.Background.Color.Pushed)
                    end
                end
                newControl:SetTransparentBackground(options.Background.Transparent)        
            end

            -- 임의 데이터 설정
            newControl:SetUserData(options.UserData)

            -- 생성
            newControl:Create()

            -- 나타내기
            if options.Show then
                newControl:Show()
            end
        end

        return newControl
    end,

    -- @description 버튼 컨트롤을 Button class 로 wrapping 하여 생성
    -- @param control control : Button control
    -- @return Button : Button control -> Button class
    CreateInstance = function(self, control)
        local options = {
            _Control = control,
        }

        return self:Create(options)
    end
}
