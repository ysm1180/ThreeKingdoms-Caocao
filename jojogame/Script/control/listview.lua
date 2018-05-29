require "control.lua"
require "text_control.lua"

-- @class ListViewColumn : Listview 컨트롤의 Column 을 Wrapping 한 클래스
-- @inherit ControlObject
ListViewColumn = ControlObject:Instance
{
    -- @description Align
    COLUMN_TEXT_ALIGN = {
        LEFT = 0,
        RIGHT = 1,
        CENTER = 2,
    },

    -- @description 기존 컨트롤을 받아와 ListViewColumn 클래스를 생성합니다.
    -- @param control control : 기존의 ListViewColumn 컨트롤
    -- @return ListViewColumn : 기존 control 의 wrapping 된 ListViewColumn 클래스
    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description ListView Column 을 새로 생성합니다.
    -- @return ListViewColumn : 새로 생성된 ListViewColumn class
    New = function(self)
        local newControl = {}
        
        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateListViewColumn()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description Column 너비를 반환합니다.
    -- @return int : Column 너비
    Width = function(self)
        return self.control:GetWidth()
    end,

    -- @description Column 텍스트를 반환합니다.
    -- @return string : 텍스트
    Text = function(self)
        return self.control:GetText()
    end,

    -- @description Column 텍스트 정렬을 설정합니다.
    -- @param align int : 정렬, Left - 0, Right - 1, Center - 2
    SetAlign = function(self, align)
        if align ~= nil and align >= COLUMN_TEXT_ALIGN.LEFT and align <= COLUMN_TEXT_ALIGN.CENTER then
            self.control:SetAlign(align)
        end
    end,

    -- @description Column 너비를 설정합니다.
    -- @param width int : 너비
    SetWidth = function(self, width)
        if width ~= nil then
            self.control:SetWidth(width)
            self.Width = width
        end
    end,

    -- @description Column 텍스트를 설정합니다.
    -- @param text string : 텍스트
    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)
        end
    end,

    -- @description 아이템에 너비 자동 맞춤 여부를 설정합니다.
    -- @param value boolean : 자동 맞춤 여부
    SetAutoSizeFitItem = function(self, value)
        if type(value) == "boolean" then
            self.control:SetAutoSizeFitItem(value)
        end
    end,

    -- @description Column Text에 너비 자동 맞춤 여부를 설정합니다.
    -- @param value boolean : 자동 맞춤 여부
    SetAutoSizeFitHeader = function(self, value)
        if type(value) == "boolean" then
            self.control:SetAutoSizeFitHeader(value)
        end
    end,
}

-- @class ListViewItem : Listview 컨트롤의 아이템을 Wrapping 한 클래스
-- @inherit TextControl
ListViewItem = TextControl:Instance
{
    -- @description 기본 색
    DEFAULT_BACKGROUND_COLOR = {
        NORMAL = {
            R = 0xFF,
            G = 0xFF,
            B = 0xFF,
        },
        FOCUSED = {
            R = 0x00,
            G = 0x78,
            B = 0xD7,
        },
        DISABLED = {
            R = 0xFF,
            G = 0xFF,
            B = 0xFF,
        },
        DISABLE_FOCUSED = {
            R = 0xE6,
            G = 0xE6,
            B = 0xE6,
        },
    },
    DEFAULT_TEXT_COLOR = {
        NORMAL = {
            R = 0x00,
            G = 0x00,
            B = 0x00,
        },
        FOCUSED = {
            R = 0xFF,
            G = 0xFF,
            B = 0xFF,
        },
        DISABLED = {
            R = 0x6D,
            G = 0x6D,
            B = 0x6D,
        },
        DISABLE_FOCUSED = {
            R = 0x6D,
            G = 0x6D,
            B = 0x6D,
        },
    },

    -- @description Align
    ITEM_TEXT_ALIGN = {
        LEFT = 0,
        RIGHT = 1,
        CENTER = 2,
    },

    -- @description 기존 컨트롤을 받아와 ListViewItem 클래스를 생성합니다.
    -- @param control control : 기존의 ListViewItem 컨트롤
    -- @return ListViewItem : 기존 control 의 wrapping 된 ListViewItem 클래스
    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description ListView Item 을 새로 생성합니다.
    -- @return ListViewItem : 새로 생성된 ListViewItem class
    New = function(self)
        local newControl = {}
        
        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateListViewItem()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 텍스트를 반환합니다.
    -- @return string : 텍스트
    Text = function(self)
        return self.control:GetText()
    end,

    -- @description 텍스트를 설정합니다.
    -- @param text string : 텍스트
    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)
        end
    end,

    -- @description 텍스트 정렬을 설정합니다.
    -- @param align int : 정렬, Left - 0, Right - 1, Center - 2
    SetAlign = function(self, align)
        if align ~= nil and align >= ITEM_TEXT_ALIGN.LEFT and align <= ITEM_TEXT_ALIGN.CENTER then
            self.control:SetAlign(align)
        end
    end,

    -- @description 기본 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 비활성화된 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisabledBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLED.B
            self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 비활성화 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisableFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 기본 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetNormalTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 비활성화된 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisabledTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLED.B
            self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 비활성화 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisableFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,
}

-- @class ListViewRow : Listview 컨트롤의 row 를 wrapping 한 클래스
-- @inherit ControlObject
ListViewRow = ControlObject:Instance
{
    -- @description 기본 색
    DEFAULT_BACKGROUND_COLOR = {
        NORMAL = {
            R = 0xFF,
            G = 0xFF,
            B = 0xFF,
        },
        FOCUSED = {
            R = 0x00,
            G = 0x78,
            B = 0xD7,
        },
        DISABLED = {
            R = 0xFF,
            G = 0xFF,
            B = 0xFF,
        },
        DISABLE_FOCUSED = {
            R = 0xE6,
            G = 0xE6,
            B = 0xE6,
        },
    },
    DEFAULT_TEXT_COLOR = {
        NORMAL = {
            R = 0x00,
            G = 0x00,
            B = 0x00,
        },
        FOCUSED = {
            R = 0xFF,
            G = 0xFF,
            B = 0xFF,
        },
        DISABLED = {
            R = 0x6D,
            G = 0x6D,
            B = 0x6D,
        },
        DISABLE_FOCUSED = {
            R = 0x6D,
            G = 0x6D,
            B = 0x6D,
        },
    },

    -- @description 기존 컨트롤을 받아와 ListViewRow 클래스를 생성합니다.
    -- @param control control : 기존의 ListViewRow 컨트롤
    -- @return ListViewRow : 기존 control 의 wrapping 된 ListViewRow 클래스
    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description ListView Row 을 새로 생성합니다.
    -- @return ListViewRow : 새로 생성된 ListViewRow class
    New = function(self)
        local newControl = {}
        
        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateListViewRow()
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description 활성화 여부를 반환합니다.
    -- @return boolean : 활성화 여부
    Enabled = function(self)
        return self.control:IsEnabled()
    end,

    -- @description 아이템을 추가합니다.
    -- @param item ListViewItem : 추가할 리스트뷰 아이템
    AddItem = function(self, item)
        if item ~= nil then
            self.control:AddItem(item.control)
        end
    end,

    -- @description 해당 index 열에 해당하는 아이템을 설정합니다.
    -- @param index int : 아이템이 설정될 column 위치
    -- @param item ListViewItem : 추가할 리스트뷰 아이템
    SetItem = function(self, index, item)
        if item ~= nil then
            self.control:SetItem(index, item.control)
        end
    end,

    -- @description 활성화 여부를 설정합니다.
    -- @param value boolean : 활성화 여부
    SetEnabled = function(self, value)
        if type(value) == "boolean" then
            self.control:SetEnabled(value)
        end
    end,

    -- @description 활성 이벤트를 설정합니다.
    -- @param event function : 활성 이벤트
    SetActiveEvent = function(self, event)
        if event ~= nil then
            self.control:SetActiveEvent(event)
        end
    end,

    -- @description 기본 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 비활성화된 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisabledBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLED.B
            self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 비활성화 배경 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisableFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 기본 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetNormalTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 비활성화된 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisabledTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLED.B
            self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,

    -- @description 마우스로 Focus 된 비활성화 글자 색을 설정합니다.
    -- @param color int | table { R int, G int, B int } : 색
    SetDisableFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
        end
    end,
}

-- @class ListView : 게임 내의 ListView 컨트롤을 Wrapping 하는 클래스
-- @inherit Control
ListView = Control:Instance
{
    -- @description 기존 컨트롤을 받아와 ListView 클래스를 생성합니다.
    -- @param control control : 기존의 ListView 컨트롤
    -- @return ListView : 기존 control 의 wrapping 된 ListView 클래스
    Instance = function(self, control)
        local newControl = {}

        newControl.control = control
        setmetatable(newControl, self)
        self.__index = self

        return newControl
    end,

    -- @description ListView 을 새로 생성합니다.
    -- @parent parent Window : ListView 가 생성될 부모 Window
    -- @return ListView : 새로 생성된 ListView class
    New = function(self, parent)
        local newControl = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateListView(parentControl)
        setmetatable(newControl, self)
        self.__index = self
        
        return newControl
    end,

    -- @description 생성
    Create = function(self)
        self.control:Create()
    end,
    
    -- @description 파괴
    Destroy = function(self)
        self.control:Destroy()
    end,

    -- @description 해당 위치의 Column 을 반환합니다.
    -- @param columnIndex int : Column 위치, 1부터 시작
    -- @retrun _Column : listview column control
    -- @warning 반환된 값을 사용하려면 ListViewColumn class 로 wrapping 한 다음 사용! (ListViewManager:CreateColumnInstance)
    Column = function(self, columnIndex)
        return self.control:GetColumn(columnIndex)
    end,

    -- @description 해당 위치의 Row 을 반환합니다.
    -- @param rowIndex int : Row 위치, 1부터 시작
    -- @retrun _Row : listview row control
    -- @warning 반환된 값을 사용하려면 ListViewRow class 로 wrapping 한 다음 사용! (ListViewManager:CreateRowInstance)
    Row = function(self, rowIndex)
        return self.control:GetRow(rowIndex)
    end,

    -- @description 테두리를 보여줄지 여부를 설정합니다.
    -- @param value boolean : 테두리 show 여부
    SetShowBorder = function(self, value)
        if type(value) == "boolean" then
            self.control:SetShowBorder(value)
        end
    end,

    -- @description 테두리를 보여줄지 여부를 설정합니다.
    -- @param value boolean : 테두리 show 여부
    SetShowColumn = function(self, value)
        if type(value) == "boolean" then
            self.control:SetShowColumn(value)
        end
    end,

    -- @description Column을 보여줄지 여부를 설정합니다.
    -- @param value boolean : column show 여부
    SetSortClickedColumn = function(self, value)
        if type(value) == "boolean" then
            self.control:SetSortClickedColumn(value)
        end
    end,

    -- @description 한번 클릭 시 활성화 여부를 설정합니다.
    -- @param value boolean : 설정 여부
    SetOneClickItemActivated = function(self, value)
        if type(value) == "boolean" then
            self.control:SetOneClickItemActivated(value)
        end
    end,

    -- @description 자동 선택 여부를 설정합니다.
    -- @param value boolean : 트래킹 사용 여부
    SetTrackingSelect = function(self, value)
        if type(value) == "boolean" then
            self.control:SetTrackingSelect(value)
        end
    end,

    -- @description 배경 투명화 여부를 설정합니다.
    -- @param value boolean : 투명화 여부
    SetTransparentBackground = function(self, value)
        if type(value) == "boolean" then
            self.control:SetTransparentBackground(value)
        end
    end,

    -- @description Row 높이를 설정합니다.
    -- @param height int : row 높이
    SetRowHeight = function(self, height)
        if height ~= nil then
            self.control:SetRowHeight(height)
        end
    end,

    -- @description 열을 추가합니다.
    -- @param column ListViewColumn : 추가할 열
    AddColumn = function(self, column)
        if column ~= nil then
            self.control:AddColumn(column.control)
        end
    end,

    -- @description 행을 추가합니다.
    -- @param row ListViewRow : 추가할 행
    AddRow = function(self, row)
        if row ~= nil then
            self.control:AddRow(row.control)
        end
    end,
}