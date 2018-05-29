require "control_manager.lua"
require "listview.lua"

-- @class ListViewMananger
-- @description ListView class 를 관리하는 Manager class
-- @inherit ControlManager
ListViewManager = ControlManager:Instance
{
    -- @description ListView class 를 생성합니다.
    -- @param options ListViewOptions : control 생성 옵션
    -- @return ListView : 생성된 ListView class
    -- @type ListViewOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
            Parent Window : control 이 생성될 부모 class 지정
            Width int : control 너비
            Height int : control 높이
            Center boolean : true 면 부모의 가운데로 이동
            X int : X 위치
            Y int : Y 위치
            RowHeight int : row 높이
            Event table : {
                Create function : 생성 이벤트
                Destroy function : 파괴 이벤트
                MouseLButtonUp function : 마우스 왼쪽 버튼 눌렀다 땠을 때 이벤트
                MouseLButtonDown function : 마우스 왼쪽 버튼 눌렀을 때 이벤트
                MouseMove function : 마우스 움직일 때 이벤트
                MouseEnter function : 마우스 진입 이벤트
                MouseLeave function : 마우스 벗어났을 때 이벤트
            }
            Option table : {
                ShowBorder boolean : 테두리 Show 여부
                ShowColumn boolean : Column Show 여부
                SortClickedColumn : Column 클릭시 Row 정렬 여부
                OneClickItemActivated : 클릭 한번시 Row Active 활성화 여부
                TrackingSelect : 마우스 이동시 Row 자동 선택 여부
            }
            Background table : {
                Transparent boolean : 배경 투명화 여부
            }
            Columns ListViewColumn[] : ListView 의 열
            Rows ListViewRow[] : ListView 의 행
            UserData string : 임의 데이터
            Show boolean : 생성 후 Show 여부
        }
    ]]
    CreateListView = function(self, options)
        local newControl = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListView:Instance(options._Control)
            end

            newControl = ListView:New(options.Parent)

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

            -- @description Row 높이 설정
            newControl:SetRowHeight(options.RowHeight)

            -- @description 배경 설정
            if options.Background ~= nil then
                newControl:SetTransparentBackground(options.Background.Transparent)
            end

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

            -- @description 옵션 설정
            if options.Option ~= nil then
                newControl:SetShowBorder(options.Option.ShowBorder)
                newControl:SetShowColumn(options.Option.ShowColumn)
                newControl:SetSortClickedColumn(options.Option.SortClickedColumn)
                newControl:SetOneClickItemActivated(options.Option.OneClickItemActivated)
                newControl:SetTrackingSelect(options.Option.TrackingSelect)
            end

            -- @description 임의 데이터 설정
            newControl:SetUserData(options.UserData)

            -- @description 생성
            newControl:Create()

            -- @description Column 설정
            if options.Columns ~= nil then    
                for i = 1, #options.Columns do
                    newControl:AddColumn(options.Columns[i])
                end
            end

            -- @description Row 설정
            if options.Rows ~= nil then
                for i = 1, #options.Rows do
                    newControl:AddRow(options.Rows[i])
                end
            end

            -- @description Show
            if options.Show then
                newControl:Show()
            end
        end

        return newControl
    end,

    -- @description ListView 컨트롤을 ListView class 로 wrapping 하여 생성
    -- @param control control : ListView control
    -- @return ListView : ListView control -> ListView class
    CreateListViewInstance = function(self, control)
        local options = {
            _Control = control,
        }

        return self:CreateListView(options)
    end,

    -- @description ListViewColumn class 를 생성합니다.
    -- @param options ListViewColumnOptions : listview column 생성 옵션
    -- @return ListViewColumn : 생성된 ListViewColumn class
    -- @type ListViewColumnOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
            Width int : column 너비
            AutoSizeFitItem boolean : 해당 Column 의 아이템 Text에 맞춰서 자동 너비 조절, true 시 Width 옵션 무시
            AutoSizeFitHeader boolean : 해당 Column Text 에 맞춰서 자동 너비 조절, true 시 Width 옵션 무시
            Text table : {
                Content string : 텍스트 내용
                Align int : 텍스트 정렬, Left - 0, Right - 1, Center - 2
            }
        }
    ]]
    CreateColumn = function(self, options)
        local newControl = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListViewColumn:Instance(options._Control)
            end

            newControl = ListViewColumn:New()

            newControl:SetWidth(options.Width)            

            newControl:SetAutoSizeFitItem(options.AutoSizeFitItem)
            newControl:SetAutoSizeFitHeader(options.AutoSizeFitHeader)

            if options.Text ~= nil then
                newControl:SetText(options.Text.Content)
                newControl:SetAlign(options.Text.Align)
            end
        end

        return newControl
    end,

    -- @description ListViewColumn 컨트롤을 ListViewColumn class 로 wrapping 하여 생성
    -- @param control control : ListViewColumn control
    -- @return ListViewColumn : ListViewColumn control -> ListViewColumn class
    CreateColumnInstance = function(self, control)
        local options = {
            _Control = control,
        }

        return self:CreateColumn(options)
    end,

    -- @description ListViewRow class 를 생성합니다.
    -- @param options ListViewRowOptions : listview row 생성 옵션
    -- @return ListViewRow : 생성된 ListViewRow class
    -- @type ListViewRowOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
            Enabled boolean : row 활성화 여부
            Event table : {
                Active function : row가 active 됐을 때 실행되는 함수, OneClickItemActivate 옵션이 설정되어 있으면 한번 클릭, 아니면 두번 클릭으로 활성화된다.
            }
            Items ListViewItem[] : row에 추가될 items
            Text table : { 
                Color table : {
                    Normal int : 기본 텍스트 색
                    Focused int : 마우스를 올려 포커스된 텍스트 색
                    Disabled int : 비활성화 기본 텍스트 색
                    DisableFocused int : 마우스를 올려 포커스된 비활성화 텍스트 색
                }
            }
            Background table : {
                Color table : {
                    Normal int : 기본 배경 색
                    Focused int : 마우스를 올려 포커스된 배경 색
                    Disabled int : 비활성화 기본 배경 색
                    DisableFocused int : 마우스를 올려 포커스된 비활성화 배경 색
                }
            }
        }
    ]]
    CreateRow = function(self, options)
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

            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    newControl:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newControl:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newControl:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newControl:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end

            if options.Text ~= nil then
                if options.Text.Color ~= nil then
                    newControl:SetNormalTextColor(options.Text.Color.Normal)
                    newControl:SetFocusedTextColor(options.Text.Color.Focused)
                    newControl:SetDisabledTextColor(options.Text.Color.Disabled)
                    newControl:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
            end                     
        end
		
        return newControl
    end,

    -- @description ListViewRow 컨트롤을 ListViewRow class 로 wrapping 하여 생성
    -- @param control control : ListViewRow control
    -- @return ListViewRow : ListViewRow control -> ListViewRow class
    CreateRowInstnace = function(self, control)
        local options = {
            _Control = control,
        }

        return self:CreateRow(options)
    end,

    -- @description ListViewItem class 를 생성합니다. Listview item 이란 listview row 의 column 개수만큼 존재하는 Text 입니다.
    -- @param options ListViewItemOptions : listview item 생성 옵션
    -- @return ListViewItem : 생성된 ListViewItem class
    -- @type ListViewItemOptions table
    --[[
        {
            _Control control : 해당 옵션이 지정되면 모든 옵션은 무시되고 해당 컨트롤로 class 를 생성한다.
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
                Align int : 텍스트 정렬, Left - 0, Right - 1, Center -2
            }
            Background table : {
                Color table : {
                    Normal int : 기본 배경 색
                    Focused int : 마우스를 올려 포커스된 배경 색
                    Disabled int : 비활성화 기본 배경 색
                    DisableFocused int : 마우스를 올려 포커스된 비활성화 배경 색
                }
            }
        }
    ]]
    CreateItem = function(self, options)
        local newControl = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListViewItem:Instance(options._Control)
            end

            newControl = ListViewItem:New()

            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    newControl:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newControl:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newControl:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newControl:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end

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
                if options.Text.Color ~= nil then
                    newControl:SetNormalTextColor(options.Text.Color.Normal)
                    newControl:SetFocusedTextColor(options.Text.Color.Focused)
                    newControl:SetDisabledTextColor(options.Text.Color.Disabled)
                    newControl:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
            end      
        end
		
        return newControl
    end,
}