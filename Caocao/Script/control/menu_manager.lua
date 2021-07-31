require "menu.lua"
require "menu_item.lua"

---@class MenuItemColor
---@field Normal integer | RGB @기본 텍스트 색
---@field Focused integer | RGB @마우스를 올려 포커스된 텍스트 색
---@field Disabled integer | RGB @비활성화 기본 텍스트 색
---@field DisableFocused integer | RGB @마우스를 올려 포커스된 비활성화 텍스트 색

---@class MenuItemEvent
---@field Click function @메뉴 아이템 클릭시 실행되는 함수 

---@class MenuItemCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 MenuItem object를 생성한다.
---@field Seperator boolean @옵션 값이 true면 강제적으로 - 로 지정된다.
---@field Enabled boolean 
---@field Font FontOption
---@field TextColor MenuItemColor
---@field Text string
---@field BackgroundColor MenuItemColor
---@field Child Menu | ChildItems MenuItem[] @Sub menu

---@class MenuMananger @Menu class, MenuItem object를 관리하는 Manager class
MenuManager = {}

---@param items MenuItem[] @추가할 메뉴 아이템
---@return Menu @생성된 메뉴
function MenuManager:CreateMenu(items)
    local newControl = Menu:New()

    for i = 1, #items do
        newControl:AddMenuItem(items[i])
    end

    return newControl
end

---메뉴 아이템을 생성한다. 클릭 액션을 가질 수 있는 메뉴를 메뉴 아이템이라 정의한다.
---@param options MenuItemCreateOptions
---@return MenuItem
function MenuManager:CreateMenuItem(options)
    local newControl = nil

    if options ~= nil then
        if options._Control ~= nil then
            newControl = MenuItem:Instance(options._Control)
            return newControl
        else
            newControl = MenuItem:New()
        end

        -- 구분자 설정
        if options.Seperator == true then
            options.Text = {
                Content = "-",
                Color = {
                    Normal = {
                        R = 0xD7,
                        G = 0xD7,
                        B = 0xD7
                    }
                }
            }
        end

        -- 활성화 설정
        newControl:SetEnabled(options.Enabled)

        -- 이벤트 설정
        if options.Event then
            newControl:SetClickEvent(options.Event.Click)
        end

        -- 폰트 설정
        if options.Font ~= nil then
            newControl:SetFontName(options.Font.Name)
            newControl:SetFontSize(options.Font.Size)
            newControl:SetBold(options.Font.Bold)
            newControl:SetUnderline(options.Font.Underline)
            newControl:SetItalic(options.Font.Italic)
        end

        if options.TextColor ~= nil then
            newControl:SetNormalTextColor(options.TextColor.Normal)
            newControl:SetFocusedTextColor(options.TextColor.Focused)
            newControl:SetDisabledTextColor(options.TextColor.Disabled)
            newControl:SetDisableFocusedTextColor(options.TextColor.DisableFocused)
        end
        newControl:SetText(options.Text)

        -- 배경 설정
        if options.BackgroundColor ~= nil then
            newControl:SetNormalBackgroundColor(options.BackgroundColor.Normal)
            newControl:SetFocusedBackgroundColor(options.BackgroundColor.Focused)
            newControl:SetDisabledBackgroundColor(options.BackgroundColor.Disabled)
            newControl:SetDisableFocusedBackgroundColor(options.BackgroundColor.DisableFocused)
        end

        -- 서브 메뉴 설정
        if options.Child ~= nil then
            newControl:SetChildMenu(options.Child.control)
        elseif options.ChildItems ~= nil then
            newControl:SetChildMenu((MenuManager:CreateMenu(options.ChildItems)).control)
        end
    end

    return newControl
end

---위치 기반으로 MenuItem object를 가져온다.
---@param menu Menu @메뉴 아이템을 가져올 기준 메뉴
---@param index integer @메뉴 아이템 index, 1부터 시작
---@return MenuItem @메뉴의 index 에 해당하는 메뉴 아이템
function MenuManager:GetMenuItemByPosition(menu, index)
    if index < 1 then
        return nil
    end

    local control = menu.control:GetMenuItemByPosition(index - 1)
    local options = {
        _Control = control
    }

    return self:CreateMenuItem(options)
end
