require "Script\\control.lua"
require "Script\\text_control.lua"

ListViewColumn = ControlObject:Instance
{
    COLUMN_TEXT_ALIGN = {
        LEFT = 0,
        RIGHT = 1,
        CENTER = 2,
    },

    Instance = function(self, control)
        local newListViewColumn = {}

        newListViewColumn.control = control
        setmetatable(newListViewColumn, self)
        self.__index = self

        OUTPUT("Get ListView Column Instance")        
        
        return newListViewColumn
    end,

    New = function(self)
        local newListViewColumn = {}
        
        newListViewColumn.control = controlManager:CreateListViewColumn()
        setmetatable(newListViewColumn, self)
        self.__index = self

        OUTPUT("Make ListView Column Instance")
        
        return newListViewColumn
    end,

    Width = function(self)
        return self.control:GetWidth()
    end,

    Text = function(self)
        return self.control:GetText()
    end,

    SetAlign = function(self, align)
        if align ~= nil and align >= COLUMN_TEXT_ALIGN.LEFT and align <= COLUMN_TEXT_ALIGN.CENTER then
            self.control:SetAlign(align)
        end
    end,

    SetWidth = function(self, width)
        if width ~= nil then
            self.control:SetWidth(width)
            self.Width = width
        end
    end,

    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)
        end
    end,

    SetAutoSizeFitItem = function(self, value)
        if type(value) == "boolean" then
            self.control:SetAutoSizeFitItem(value)
        end
    end,

    SetAutoSizeFitHeader = function(self, value)
        if type(value) == "boolean" then
            self.control:SetAutoSizeFitHeader(value)
        end
    end,
}

ListViewItem = TextControl:Instance
{
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

    ITEM_TEXT_ALIGN = {
        LEFT = 0,
        RIGHT = 1,
        CENTER = 2,
    },

    Instance = function(self, control)
        local newListViewItem = {}

        newListViewItem.control = control
        setmetatable(newListViewItem, self)
        self.__index = self

        OUTPUT("Get ListView Item Instance")        
        
        return newListViewItem
    end,

    New = function(self)
        local newListViewItem = {}
        
        newListViewItem.control = controlManager:CreateListViewItem()
        setmetatable(newListViewItem, self)
        self.__index = self

        OUTPUT("Make ListView Item Instance")
        
        return newListViewItem
    end,

    Text = function(self)
        return self.control:GetText()
    end,

    SetText = function(self, text)
        if text ~= nil then
            self.control:SetText(text)
        end
    end,

    SetAlign = function(self, align)
        if align ~= nil and align >= ITEM_TEXT_ALIGN.LEFT and align <= ITEM_TEXT_ALIGN.CENTER then
            self.control:SetAlign(align)
        end
    end,

    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Normal Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Focused Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisabledBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLED.B
            self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disabled Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisableFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Focused Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetNormalTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Normal Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Focused Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisabledTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLED.B
            self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisableFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Focused Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,
}

ListViewRow = ControlObject:Instance
{
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

    Instance = function(self, control)
        local newListViewRow = {}

        newListViewRow.control = control
        setmetatable(newListViewRow, self)
        self.__index = self

        OUTPUT("Get ListView Row Instance")        
        
        return newListViewRow
    end,

    New = function(self)
        local newListViewRow = {}
        
        newListViewRow.control = controlManager:CreateListViewRow()
        setmetatable(newListViewRow, self)
        self.__index = self

        OUTPUT("Make ListView Row Instance")
        
        return newListViewRow
    end,

    Enabled = function(self)
        return self.control:IsEnabled()
    end,

    AddItem = function(self, item)
        if item ~= nil then
            self.control:AddItem(item.control)
        end
    end,

    SetItem = function(self, item)
        if item ~= nil then
            self.control:SetItem(item.control)
        end
    end,

    SetEnabled = function(self, value)
        if type(value) == "boolean" then
            self.control:SetEnabled(value)
        end
    end,

    SetNormalBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.NORMAL.B
            self.control:SetNormalBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Normal Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Focused Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisabledBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLED.B
            self.control:SetDisabledBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disabled Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisableFocusedBackgroundColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetDisableFocusedBackgroundColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Focused Background Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetNormalTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.NORMAL.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.NORMAL.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.NORMAL.B
            self.control:SetNormalTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Normal Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.FOCUSED.B
            self.control:SetFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Focused Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisabledTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_TEXT_COLOR.DISABLED.R
            color.G = color.G or self.DEFAULT_TEXT_COLOR.DISABLED.G
            color.B = color.B or self.DEFAULT_TEXT_COLOR.DISABLED.B
            self.control:SetDisabledTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,

    SetDisableFocusedTextColor = function(self, color)
        if color ~= nil then
            if type(color) == "number" then
                color = {R = color % 0x100, G = math.floor(color / 0x100) % 0x100, B = math.floor(color / 0x10000) % 0x100}
            end

            color.R = color.R or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.R
            color.G = color.G or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.G
            color.B = color.B or self.DEFAULT_BACKGROUND_COLOR.DISABLE_FOCUSED.B
            self.control:SetDisableFocusedTextColor(gameManager:Color(color.R, color.G, color.B))
    
            OUTPUT(
                "Set Disable Focused Text Color #" ..
                    string.format("%02x", color.R) .. string.format("%02x", color.G) .. string.format("%02x", color.B)
            )
        end
    end,
}

ListView = Control:Instance
{
    Instance = function(self, control)
        local newListView = {}

        newListView.control = control
        setmetatable(newListView, self)
        self.__index = self

        OUTPUT("Get ListView Instance")        
        
        return newListView
    end,

    New = function(self, parent)
        local newListView = {}
        local parentControl = nil

        if parent ~= nil then
            parentControl = parent.control
        end

        newListView.control = controlManager:CreateListView(parentControl)
        setmetatable(newListView, self)
        self.__index = self
        
        return newListView
    end,

    Create = function(self)
        self.control:Create()

        OUTPUT("ListView Created")
    end,
    
    Destroy = function(self)
        self.control:Destroy()

        OUTPUT("ListView Destroy")
    end,

    Column = function(self, columnIndex)
        return self.control:GetColumn(columnIndex)
    end,

    Row = function(self, rowIndex)
        return self.control:GetRow(rowIndex)
    end,

    SetShowBorder = function(self, value)
        if type(value) == "boolean" then
            self.control:SetShowBorder(value)
            OUTPUT("Set Show Border : " .. tostring(value))
        end
    end,

    SetShowColumn = function(self, value)
        if type(value) == "boolean" then
            self.control:SetShowColumn(value)
            OUTPUT("Set Show Column : " .. tostring(value))
        end
    end,

    SetSortClickedColumn = function(self, value)
        if type(value) == "boolean" then
            self.control:SetSortClickedColumn(value)
            OUTPUT("Set Sort Clicked Column : " .. tostring(value))
        end
    end,

    SetOneClickItemActivated = function(self, value)
        if type(value) == "boolean" then
            self.control:SetOneClickItemActivated(value)
            OUTPUT("Set Hot Click : " .. tostring(value))
        end
    end,

    SetTrackingSelect = function(self, value)
        if type(value) == "boolean" then
            self.control:SetTrackingSelect(value)
            OUTPUT("Set Tracking Select : " .. tostring(value))
        end
    end,

    SetRowHeight = function(self, height)
        if height ~= nil then
            self.control:SetRowHeight(height)
        end
    end,

    AddColumn = function(self, column)
        if column ~= nil then
            self.control:AddColumn(column.control)
        end
    end,

    AddRow = function(self, row)
        if row ~= nil then
            self.control:AddRow(row.control)
        end
    end,
}