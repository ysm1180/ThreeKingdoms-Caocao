require "listview.lua"
require "listview_row.lua"
require "listview_column.lua"
require "listview_item.lua"

---@class ListViewEvent
---@

---@class ListViewCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Parent Window
---@field Width integer
---@field Height integer
---@field X integer
---@field Y integer
---@field RowHeight integer
---@field ShowBorder boolean @테두리 Show 여부
---@field ShowColumn boolean @Column Show 여부
---@field SortClickedColumn boolean @Column 클릭시 Row 정렬 여부
---@field OneClickItemActivated boolean @클릭 한번시 Row Active 활성화 여부
---@field TrackingSelect boolean @마우스 이동시 Row 자동 선택 여부
---@field Transparent boolean @배경 투명화 여부
---@field Columns ListViewColumn[] @ListView 의 열
---@field Rows ListViewRow[] @ListView 의 행
---@field UserData string 
---@field Show boolean 

---@class ListViewMananger @ListView object를 관리하는 Manager class
ListViewManager = {}

---ListView object를 생성합니다.
---@param options ListViewCreateOptions
---@return ListView
function ListViewManager:CreateListView(options)
    local newControl = nil

    if options ~= nil then
        if options._Control ~= nil then
            return ListView:Instance(options._Control)
        end

        newControl = ListView:New(options.Parent)

        -- 크기 설정
        newControl:SetSize(options.Width, options.Height)

        -- 위치 설정
        newControl:Move(options.X, options.Y)

        -- Row 높이 설정
        newControl:SetRowHeight(options.RowHeight)

        -- 배경 투명화 설정
        newControl:SetTransparentBackground(options.Transparent)

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

        -- 옵션 설정
        newControl:SetShowBorder(options.ShowBorder)
        newControl:SetShowColumn(options.ShowColumn)
        newControl:SetSortClickedColumn(options.SortClickedColumn)
        newControl:SetOneClickItemActivated(options.OneClickItemActivated)
        newControl:SetTrackingSelect(options.TrackingSelect)

        -- 임의 데이터 설정
        newControl:SetUserData(options.UserData)

        -- 생성
        newControl:Create()

        -- Column 설정
        if options.Columns ~= nil then
            for i = 1, #options.Columns do
                newControl:AddColumn(options.Columns[i])
            end
        end

        -- Row 설정
        if options.Rows ~= nil then
            for i = 1, #options.Rows do
                newControl:AddRow(options.Rows[i])
            end
        end

        -- Show
        if options.Show then
            newControl:Show()
        end
    end

    return newControl
end

---@param control any
---@return ListView 
function ListViewManager:CreateListViewInstance(control)
    local options = {
        _Control = control
    }

    return self:CreateListView(options)
end

---@class ListViewColumnCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Width integer 
---@field AutoSizeFitItem boolean @해당 Column 의 아이템 Text에 맞춰서 자동 너비 조절, true 시 Width 옵션 무시
---@field AutoSizeFitHeader boolean @해당 Column Text 에 맞춰서 자동 너비 조절, true 시 Width 옵션 무시
---@field Text string
---@field TextAlign integer @텍스트 정렬, Left - 0, Right - 1, Center - 2

---ListViewColumn object를 생성합니다.
---@param options ListViewColumnCreateOptions
---@return ListViewColumn
function ListViewManager:CreateColumn(options)
    local newControl = nil

    if options ~= nil then
        if options._Control ~= nil then
            return ListViewColumn:Instance(options._Control)
        end

        newControl = ListViewColumn:New()

        newControl:SetWidth(options.Width)

        newControl:SetAutoSizeFitItem(options.AutoSizeFitItem)
        newControl:SetAutoSizeFitHeader(options.AutoSizeFitHeader)

        newControl:SetText(options.Text)
        newControl:SetAlign(options.TextAlign)
    end

    return newControl
end

---@param control any
---@return ListViewColumn
function ListViewManager:CreateColumnInstance(control)
    local options = {
        _Control = control
    }

    return self:CreateColumn(options)
end

---@class ListViewRowColor
---@field Normal integer | RGB @기본 텍스트 색
---@field Focused integer | RGB @마우스를 올려 포커스된 텍스트 색
---@field Disabled integer | RGB @비활성화 기본 텍스트 색
---@field DisableFocused integer | RGB @마우스를 올려 포커스된 비활성화 텍스트 색

---@class ListViewRowEvent
---@field Active function @row가 active 됐을 때 실행되는 함수, OneClickItemActivate 옵션이 설정되어 있으면 한번 클릭, 아니면 두번 클릭으로 활성화된다.

---@class ListViewRowCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Enabled boolean @row 활성화 여부
---@field Event ListViewRowEvent
---@field TextColor ListViewRowColor
---@field BackgroundColor ListViewRowColor

---ListViewRow object를 생성합니다.
---@param options ListViewRowCreateOptions
---@return ListViewRow
function ListViewManager:CreateRow(options)
    local newControl = nil

    if options ~= nil then
        if options._Control ~= nil then
            return ListViewRow:Instance(options._Control)
        end

        newControl = ListViewRow:New()

        newControl:SetEnabled(options.Enabled)

        if options.Event ~= nil then
            newListRow:SetActiveEvent(options.Event.Active)
        end

        if options.Items ~= nil then
            for i = 1, #options.Items do
                newControl:AddItem(options.Items[i])
            end
        end

        if options.BackgroundColor ~= nil then
            newControl:SetNormalBackgroundColor(options.BackgroundColor.Normal)
            newControl:SetFocusedBackgroundColor(options.BackgroundColor.Focused)
            newControl:SetDisabledBackgroundColor(options.BackgroundColor.Disabled)
            newControl:SetDisableFocusedBackgroundColor(options.BackgroundColor.DisableFocused)
        end

        if options.TextColor ~= nil then
            newControl:SetNormalTextColor(options.TextColor.Normal)
            newControl:SetFocusedTextColor(options.TextColor.Focused)
            newControl:SetDisabledTextColor(options.TextColor.Disabled)
            newControl:SetDisableFocusedTextColor(options.TextColor.DisableFocused)
        end
    end

    return newControl
end

---@param control any
---@return ListViewRow
function ListViewManager:CreateRowInstnace(control)
    local options = {
        _Control = control
    }

    return self:CreateRow(options)
end

---@class ListViewItemColor
---@field Normal integer | RGB @기본 텍스트 색
---@field Focused integer | RGB @마우스를 올려 포커스된 텍스트 색
---@field Disabled integer | RGB @비활성화 기본 텍스트 색
---@field DisableFocused integer | RGB @마우스를 올려 포커스된 비활성화 텍스트 색

---@class ListViewItemCreateOptions
---@field _Control any @해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 object를 생성한다.
---@field Font FontOption
---@field TextColor ListViewItemColor
---@field Text string
---@field TextAlign integer @텍스트 정렬, Left - 0, Right - 1, Center -2
---@field BackgroundColor ListViewItemColor

---ListViewItem object를 생성합니다. Listview item 이란 listview row 의 column 개수만큼 존재하는 Text 입니다.
---@param options ListViewItemCreateOptions
---@return ListViewItem
function ListViewManager:CreateItem(options)
    local newControl = nil

    if options ~= nil then
        if options._Control ~= nil then
            return ListViewItem:Instance(options._Control)
        end

        newControl = ListViewItem:New()

        if options.BackgroundColor ~= nil then
            newControl:SetNormalBackgroundColor(options.BackgroundColor.Normal)
            newControl:SetFocusedBackgroundColor(options.BackgroundColor.Focused)
            newControl:SetDisabledBackgroundColor(options.BackgroundColor.Disabled)
            newControl:SetDisableFocusedBackgroundColor(options.BackgroundColor.DisableFocused)
        end

        newControl:SetText(options.Text)
        newControl:SetAlign(options.TextAlign)

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
    end

    return newControl
end

