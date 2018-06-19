require "groupbox.lua"

-- @class GroupBoxMananger
-- @description GroupBox class 를 관리하는 Manager class
-- @inherit Object
GroupBoxManager = Object:Instance 
{
    -- @param options GroupBoxOptions : control 생성 옵션
    -- @return GroupBox : 생성된 GroupBox class
    -- @type GroupBoxOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
            Parent Window : control 이 생성될 부모 Class 지정
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
                Content string : 텍스트 내용
            }
            UserData string : 임의 데이터
            Show boolean : 생성 후 Show 여부
        }
    ]]
    Create = function(self, options)
        local newControl = nil

        if options ~= nil then

            if options._Control ~= nil then
                return GroupBox:Instance(options._Control)
            end
            
            newControl = GroupBox:New(options.Parent)

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

                newControl:SetText(options.Text.Content)
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

    -- @description GroupBox 컨트롤을 GroupBox class 로 wrapping 하여 생성
    -- @param control control : GroupBox control
    -- @return GroupBox : GroupBox control -> GroupBox class
    CreateInstance = function(self, control)
        local options = {
            _Control = control,
        }

        return self:Create(options)
    end
}
