require "../base/object.lua"

local currentPath = fileManager:GetWorkingPath()

XML_CONTROL_NAME = {
    WINDOW = "dialog",
    STATIC = "label",
    BUTTON = "button",
    LISTVIEW = "listview",
    LISTVIEW_ITEMS = "items",
    LISTVIEW_ITEM = "item",
    LISTVIEW_ROWS = "rows",
    LISTVIEW_ROW = "row",
    LISTVIEW_COLUMNS = "columns",
    LISTVIEW_COLUMN = "column",
    CHECKBOX = "checkbox",
    RADIOBUTTON = "radio",
    GROUPBOX = "group",
}

local function StrToBoolean(value)
    if value == "true" then
        return true
    else
        return false
    end
end

local function RgbToHex(rgb)
    rgb = rgb:gsub("#", "")
    return tonumber("0x" .. rgb:sub(1))
end

-- @class ControlManager
-- @description Control 생성 관리 클래스
-- @inherit Object
ControlManager =
    Object:Instance {
    BASE_CONTROL = {
        [XML_CONTROL_NAME.WINDOW] = true,
        [XML_CONTROL_NAME.STATIC] = true,
        [XML_CONTROL_NAME.BUTTON] = true,
        [XML_CONTROL_NAME.LISTVIEW] = true,
        [XML_CONTROL_NAME.CHECKBOX] = true,
        [XML_CONTROL_NAME.RADIOBUTTON] = true,
        [XML_CONTROL_NAME.GROUPBOX] = true,
    },
    TEXT_CONTROL = {
        [XML_CONTROL_NAME.STATIC] = true,
        [XML_CONTROL_NAME.BUTTON] = true,
        [XML_CONTROL_NAME.LISTVIEW_ITEM] = true,
        [XML_CONTROL_NAME.CHECKBOX] = true,
        [XML_CONTROL_NAME.RADIOBUTTON] = true,
        [XML_CONTROL_NAME.GROUPBOX] = true,
    },
    ParseFromXML = function(self, file)
        local SLAXML = require "../base/slaxml.lua"
        local xml = io.open(currentPath .. file):read("*all")

        local window = nil
        local childControl = nil

        local function SetAttributeControl(control, name, value)
            if name == "width" then
                control:SetSize(tonumber(value))
            elseif name == "height" then
                control:SetSize(nil, tonumber(value))
            elseif name == "x" then
                control:Move(tonumber(value))
            elseif name == "y" then
                control:Move(nil, tonumber(value))
            end
        end

        local function SetAttributeText(control, name, value)
            if name == "font-name" then
                control:SetFontName(value)
            elseif name == "font-size" then
                control:SetFontSize(tonumber(value))
            elseif name == "bold" then
                control:SetBold(StrToBoolean(value))
            elseif name == "underline" then
                control:SetUnderline(StrToBoolean(value))
            elseif name == "italic" then
                control:SetItalic(StrToBoolean(value))
            end
        end

        local function SetAttributeWindow(control, name, value)
            if name == "title" then
                control:SetTitleName(value)
            elseif name == "maxbutton" then
                control:SetMaxButton(StrToBoolean(value))
            elseif name == "minbutton" then
                control:SetMinButton(StrToBoolean(value))
            elseif name == "background-color" then
                control:SetBackgroundColor(RgbToHex(value))
            end
        end

        local function SetAttributeLabel(control, name, value)
            if name == "autosize" then
                control:SetAutoSize(StrToBoolean(value))
            elseif name == "align" then
                if value == "left" then
                    control:SetAlign(0)
                elseif value == "right" then
                    control:SetAlign(1)
                elseif value == "center" then
                    control:SetAlign(2)
                end
            elseif name == "background-color" then
                if value == "transparent" then
                    control:SetTransparentBackground(true)
                else
                    control:SetTransparentBackground(false)
                    control:SetBackgroundColor(RgbToHex(value))
                end
            elseif name == "color" then
                control:SetTextColor(RgbToHex(value))
            end
        end

        local function SetAttributeButton(control, name, value)
            if name == "background-color" then
                if value == "transparent" then
                    control:SetTransparentBackground(true)
                else
                    control:SetTransparentBackground(false)
                    control:SetNormalBackgroundColor(RgbToHex(value))
                end
            elseif name == "background-color:focused" then
                control:SetFocusedBackgroundColor(RgbToHex(value))
            elseif name == "background-color:pushed" then
                control:SetPushedBackgroundColor(RgbToHex(value))
            elseif name == "border-width" then
                control:SetBorderWidth(tonumber(value))
            elseif name == "border-color" then
                control:SetNormalBorderColor(RgbToHex(value))
            elseif name == "border-color:focused" then
                control:SetFocusedBorderColor(RgbToHex(value))
            elseif name == "border-color:pushed" then
                control:SetPushedBorderColor(RgbToHex(value))
            elseif name == "color" then
                control:SetNormalTextColor(RgbToHex(value))
            elseif name == "color:focused" then
                control:SetFocusedTextColor(RgbToHex(value))
            elseif name == "color:pushed" then
                control:SetPushedTextColor(RgbToHex(value))
            end
        end

        local function SetAttributeListView(control, name, value)
            if name == "show-border" then
                control:SetShowBorder(StrToBoolean(value))
            elseif name == "transparent" then
                control:SetTransparentBackground(StrToBoolean(value))
            end
        end

        local function SetAttributeListViewColumns(control, name, value)
            if name == "visible" then
                control:SetShowColumn(StrToBoolean(value))
            elseif name == "click-sort" then
                control:SetSortClickedColumn(StrToBoolean(value))
            end
        end

        local function SetAttributeListViewColumn(column, name, value)
            if name == "width" then
                column:SetWidth(tonumber(value))
            elseif name == "align" then
                if value == "left" then
                    column:SetAlign(0)
                elseif value == "right" then
                    column:SetAlign(1)
                elseif value == "center" then
                    column:SetAlign(2)
                end
            elseif name == "fit-item" then
                column:SetAutoSizeFitItem(StrToBoolean(value))
            elseif name == "fit-header" then
                column:SetAutoSizeFitHeader(StrToBoolean(value))
            end
        end

        local function SetAttributeListViewRows(control, name, value)
            if name == "height" then
                control:SetRowHeight(tonumber(value))
            elseif name == "tracking-select" then
                control:SetTrackingSelect(StrToBoolean(value))
            elseif name == "one-click" then
                control:SetOneClickItemActivated(StrToBoolean(value))
            end
        end

        local function SetAttributeListViewRow(row, name, value)
            if name == "enabled" then
                row:SetEnabled(StrToBoolean(value))
            elseif name == "background-color" then
                row:SetNormalBackgroundColor(RgbToHex(value))
            elseif name == "background-color:focused" then
                row:SetFocusedBackgroundColor(RgbToHex(value))
            elseif name == "background-color:disabled" then
                row:SetDisabledBackgroundColor(RgbToHex(value))
            elseif name == "background-color:disable-focused" then
                row:SetDisableFocusedBackgroundColor(RgbToHex(value))
            elseif name == "color" then
                row:SetNormalTextColor(RgbToHex(value))
            elseif name == "color:focused" then
                row:SetFocusedTextColor(RgbToHex(value))
            elseif name == "color:disabled" then
                row:SetDisabledTextColor(RgbToHex(value))
            elseif name == "color:disable-focused" then
                row:SetDisableFocusedTextColor(RgbToHex(value))
            end
        end

        local function SetAttributeListViewItem(item, name, value)
            if name == "background-color" then
                item:SetNormalBackgroundColor(RgbToHex(value))
            elseif name == "background-color:focused" then
                item:SetFocusedBackgroundColor(RgbToHex(value))
            elseif name == "background-color:disabled" then
                item:SetDisabledBackgroundColor(RgbToHex(value))
            elseif name == "background-color:disable-focused" then
                item:SetDisableFocusedBackgroundColor(RgbToHex(value))
            elseif name == "color" then
                item:SetNormalTextColor(RgbToHex(value))
            elseif name == "color:focused" then
                item:SetFocusedTextColor(RgbToHex(value))
            elseif name == "color:disabled" then
                item:SetDisabledTextColor(RgbToHex(value))
            elseif name == "color:disable-focused" then
                item:SetDisableFocusedTextColor(RgbToHex(value))
            elseif name == "align" then
                if value == "left" then
                    item:SetAlign(0)
                elseif value == "right" then
                    item:SetAlign(1)
                elseif value == "center" then
                    item:SetAlign(2)
                end
            end
        end

        local childControls = {}
        local childControlTypes = {}
        local id = ""
        local columns = {}
        local column = nil
        local rows = {}
        local row = nil
        local rowItems = {}
        local rowItem = nil

        parser =
            SLAXML:parser {
            startElement = function(name, nsURI, nsPrefix)
                if name == XML_CONTROL_NAME.WINDOW then
                    window = Window:New()
                elseif name == XML_CONTROL_NAME.STATIC then
                    childControl = Static:New(window)
                elseif name == XML_CONTROL_NAME.BUTTON then
                    childControl = Button:New(window)
                elseif name == XML_CONTROL_NAME.LISTVIEW then
                    childControl = ListView:New(window)
                elseif name == XML_CONTROL_NAME.LISTVIEW_COLUMNS then
                    columns = {}
                elseif name == XML_CONTROL_NAME.LISTVIEW_COLUMN then
                    column = ListViewColumn:New()
                elseif name == XML_CONTROL_NAME.LISTVIEW_ROWS then
                    rows = {}
                elseif name == XML_CONTROL_NAME.LISTVIEW_ROW then
                    row = ListViewRow:New()
                elseif name == XML_CONTROL_NAME.LISTVIEW_ITEMS then
                    rowItems = {}
                elseif name == XML_CONTROL_NAME.LISTVIEW_ITEM then
                    rowItem = ListViewItem:New()
                elseif name == XML_CONTROL_NAME.CHECKBOX then
                    childControl = CheckBox:New(window)
                elseif name == XML_CONTROL_NAME.RADIOBUTTON then
                    childControl = RadioButton:New(window)
                elseif name == XML_CONTROL_NAME.GROUPBOX then
                    childControl = GroupBox:New(window)
                end
                table.insert(childControlTypes, 1, name)
            end, -- When "<foo" or <x:foo is seen
            attribute = function(name, value, nsURI, nsPrefix)
                if name == "id" then
                    id = value
                    return
                end

                if self.BASE_CONTROL[childControlTypes[1]] == true then
                    if childControlTypes[1] == XML_CONTROL_NAME.WINDOW then
                        SetAttributeControl(window, name, value)
                    else
                        SetAttributeControl(childControl, name, value)
                    end
                end

                if self.TEXT_CONTROL[childControlTypes[1]] == true then
                    if childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_ITEM then
                        SetAttributeText(rowItem, name, value)
                    else
                        SetAttributeText(childControl, name, value)
                    end
                end

                if childControlTypes[1] == XML_CONTROL_NAME.WINDOW then
                    SetAttributeWindow(window, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.STATIC then
                    SetAttributeLabel(childControl, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.BUTTON then
                    SetAttributeButton(childControl, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW then
                    SetAttributeListView(childControl, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_COLUMN then
                    SetAttributeListViewColumn(column, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_COLUMNS then
                    SetAttributeListViewColumns(childControl, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_ROW then
                    SetAttributeListViewColumns(childControl, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_ROWS then
                    SetAttributeListViewRows(childControl, name, value)
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_ITEM then
                    SetAttributeListViewItem(rowItem, name, value)
                end
            end, -- attribute found on current element
            closeElement = function(name, nsURI)
                if name == XML_CONTROL_NAME.WINDOW then
                    window:Create()
                    for key, value in pairs(childControls) do
                        value:Create()
                    end
                elseif self.BASE_CONTROL[name] == true then
                    childControl:Show()
                    if id ~= "" then
                        childControls[id] = childControl
                    else
                        table.insert(childControls, childControl)
                    end
                end

                if name == XML_CONTROL_NAME.LISTVIEW then
                    for i = 1, #columns do
                        childControl:AddColumn(columns[i])
                    end
                    for i = 1, #rows do
                        childControl:AddRow(rows[i])
                    end
                elseif name == XML_CONTROL_NAME.LISTVIEW_COLUMN then
                    table.insert(columns, column)
                elseif name == XML_CONTROL_NAME.LISTVIEW_ROW then
                    table.insert(rows, row)
                elseif name == XML_CONTROL_NAME.LISTVIEW_ITEM then
                    table.insert(rowItems, rowItem)
                elseif name == XML_CONTROL_NAME.LISTVIEW_ITEMS then
                    for i = 1, #rowItems do
                        row:AddItem(rowItems[i])
                    end
                end

                table.remove(childControlTypes, 1)
            end, -- When "</foo>" or </x:foo> or "/>" is seen
            text = function(text)
                if self.TEXT_CONTROL[childControlTypes[1]] == true then
                    if childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_ITEM then
                        OUTPUT(text)
                        rowItem:SetText(text)
                    else
                        childControl:SetText(text)
                    end
                elseif childControlTypes[1] == XML_CONTROL_NAME.LISTVIEW_COLUMN then
                    column:SetText(text)
                end
            end, -- text and CDATA nodes
            comment = function(content)
            end, -- comments
            pi = function(target, content)
            end -- processing instructions e.g. "<?yes mon?>"
        }

        parser:parse(xml)

        return window, childControls
    end
}
