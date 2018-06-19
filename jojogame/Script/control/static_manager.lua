require "static.lua"

-- @class StaticMananger
-- @description Static class 를 관리하는 Manager class
-- @inherit Object
StaticManager = Object:Instance
{
    -- @description Static class 를 생성합니다.
    -- @param options StaticOptions : 버튼 생성 옵션
    -- @return RadioButotn : 생성된 Static class
    -- @type StaticOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
            Parent Window : control 이 생성될 부모 Class 지정
            Width int : control 너비
            Height int : control 높이
            AutoSize boolean : 자동 크기 조절 여부
            X int : X 위치
            Y int : Y 위치
            Event table : {
                Create function : 생성 이벤트
                Destroy function : 파괴 이벤트
            }
            Text table : {
                Font table : {
                    Name string : 폰트 이름
                    Size int : 폰트 크기
                    Bold boolean : 폰트 Bold 여부
                    Underline boolean : 폰트 밑줄 여부
                    Italic boolean : 폰트 기울임 여부
                }
                Align int : 텍스트 정렬, Left - 0, Right - 1, Center - 2
                Color int : 텍스트 색
                Content string : 텍스트 내용
            }
            Background table : {
                Color int : 배경 색
                Transparent boolean : 배경 투명화 여부
            }
            Show boolean : 생성 후 Show 여부
        }
    ]]
    Create = function(self, options)
        local newControl = nil
        if options ~= nil then
            if options.Control ~= nil then
                return Static:Instance(options.Control)
            end

            newControl = Static:New(options.Parent)

            -- 크기 설정
            newControl:SetSize(options.Width, options.Height)
            newControl:SetAutoSize(options.AutoSize)

            -- 위치 설정
            newControl:Move(options.X, options.Y)

            -- 텍스트 설정
            if options.Text ~= nil then
                if options.Text.Font ~= nil then
                    newControl:SetFontName(options.Text.Font.Name)
                    newControl:SetFontSize(options.Text.Font.Size)
                    newControl:SetBold(options.Text.Font.Bold)
                    newControl:SetUnderline(options.Text.Font.Underline)
                    newControl:SetItalic(options.Text.Font.Italic)
                end
                newControl:SetAlign(options.Text.Align)
                newControl:SetText(options.Text.Content)
                newControl:SetTextColor(options.Text.Color)
            end

            -- 배경 설정
            if options.Background ~= nil then
                newControl:SetBackgroundColor(options.Background.Color)
                newControl:SetTransparentBackground(options.Background.Transparent)
            end

            -- 이벤트 설정
            if options.Event ~= nil then
                newControl:SetCreateEvent(options.Event.Create)
                newControl:SetDestroyEvent(options.Event.Destroy)     
            end

            -- 생성
            newControl:Create()

            -- Show
            if options.Show then
                newControl:Show()
            end
        end

        return newControl
    end,

    -- @description Static 컨트롤을 Static class 로 wrapping 하여 생성
    -- @param control control : Static control
    -- @return Static : Static control -> Static class
    CreateInstance = function(self, control)
        local options = {
            Control = control,
        }

        return self:Create(options)
    end,
}