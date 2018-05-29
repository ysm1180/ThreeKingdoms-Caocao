require "menu.lua"

-- @class MenutMananger
-- @description Menu class, MenuItem class 를 관리하는 Manager class
-- @inherit Object
MenuManager = Object:Instance
{
    -- @description 메뉴 아이템을 추가한 메뉴를 생성한다.
    -- @param items MenuItem[] : 추가할 메뉴 아이템
    -- @return Menu : 생성된 메뉴
    CreateMenu = function(self, items)
        local newControl = Menu:New()

        for i = 1, #items do
            newControl:AddMenuItem(items[i])
        end

        return newControl
    end,

    -- @description 메뉴 아이템을 생성한다. 클릭 액션을 가질 수 있는 메뉴를 메뉴 아이템이라 정의한다.
    -- @param options MenuItemOption - 아이템 생성 옵션
    -- @return MenuItem : 생성된 메뉴 아이템
    -- @type MenuItemOption table
    --[[ 
        _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 MenuItem class 를 생성한다.
        Seperator boolean: 옵션 값이 true 면 Text Option 은 강제적으로 - 로 지정된다.
        Enabled boolean : 활성화 여부 설정
        Event table : { 
            Click function : 메뉴 아이템 클릭시 실행되는 함수 
        }
        Text table : { 
            Font table : {
                Name string : 폰트 이름
                Size int : 폰트 크기
                Bold boolean : 폰트 Bold 여부
                Underline boolean : 폰트 밑줄 여부
                Italic boolean : 폰트 기울임 여부
            }
            Color table : {
                Normal int : 기본 텍스트 색
                Focused int : 마우스를 올려 포커스된 텍스트 색
                Disabled int : 비활성화 기본 텍스트 색
                DisableFocused int : 마우스를 올려 포커스된 비활성화 텍스트 색
            }
            Content string : 텍스트 내용
        }
        Background table : {
            Color table : {
                Normal int : 기본 배경 색
                Focused int : 마우스를 올려 포커스된 배경 색
                Disabled int : 비활성화 기본 배경 색
                DisableFocused int : 마우스를 올려 포커스된 비활성화 배경 색
            }
        }
        Child Menu | ChildItems MenuItem[] : Sub menu
    ]]
    CreateMenuItem = function(self, options)
        local newControl = nil

        if options ~= nil then
            if options._Control ~= nil then
                newControl = MenuItem:Instance(options._Control)
                return newControl
            else
                newControl = MenuItem:New()            
            end

            -- @description 구분자 설정
            if options.Seperator == true then
                options.Text = {
                    Content = "-",
                    Color = {
                        Normal = {R = 0xD7, G = 0xD7, B = 0xD7},
                    },
                }
            end

            -- @description 활성화 설정
            newControl:SetEnabled(options.Enabled)

            -- @description 이벤트 설정
            if options.Event then
                newControl:SetClickEvent(options.Event.Click)
            end

            -- @description 텍스트 설정
            if options.Text then    
                if options.Text.Font then
                    newControl:SetFontName(options.Text.Font.Name)
                    newControl:SetFontSize(options.Text.Font.Size)
                    newControl:SetBold(options.Text.Font.Bold)
                    newControl:SetUnderline(options.Text.Font.Underline)
                    newControl:SetItalic(options.Text.Font.Italic)
                end
                if options.Text.Color then
                    newControl:SetNormalTextColor(options.Text.Color.Normal)
                    newControl:SetFocusedTextColor(options.Text.Color.Focused)
                    newControl:SetDisabledTextColor(options.Text.Color.Disabled)
                    newControl:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
                newControl:SetText(options.Text.Content)
            end

            -- @description 배경 설정
            if options.Background then
                if options.Background.Color then
                    newControl:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newControl:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newControl:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newControl:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end

            -- @description 서브 메뉴 설정
            if options.Child ~= nil then
                newControl:SetChildMenu(options.Child.control)
            elseif options.ChildItems ~= nil then
                newControl:SetChildMenu((MenuManager:CreateMenu(options.ChildItems)).control)
            end
        end

        return newControl
    end,

    -- @description 위치 기반으로 MenuItem class 를 가져온다.
    -- @param menu Menu : 메뉴 아이템을 가져올 기준 메뉴
    -- @param index int : 메뉴 아이템 index, 1부터 시작
    -- @return MenuItem : 메뉴의 index 에 해당하는 메뉴 아이템
    GetMenuItemByPosition = function(self, menu, index)
        if index < 1 then
            return nil
        end
        
        local control = menu.control:GetMenuItemByPosition(index - 1)
        local options = {
            _Control = control,
        }

        return self:CreateMenuItem(options)
    end,
}