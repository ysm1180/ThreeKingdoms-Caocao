require "Script\\control.lua"

ListViewColumn = ControlObject:Instance
{
    New = function(self)
        local newListViewColumn = {}
        
        newListViewColumn.control = controlManager:CreateListViewColumn()
        setmetatable(newListViewColumn, self)
        self.__index = self

        OUTPUT("Make ListView Column Instance")
        
        return newListViewColumn
    end,

    Text = function(self)
        return self.control:GetText()
    end,

    SetAlign = function(self, align)
        if align ~= nil then
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
}

ListView = Control:Instance
{
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

    SetShowColumn = function(self, value)
        if typeof(value) == "boolean" then
            self.control:SetShowColumn(value)
            OUTPUT("Set Show Column : " .. tostring(value))
        end
    end,

    SetSortClickedColumn = function(self, value)
        if typeof(value) == "boolean" then
            self.control:SetSortClickedColumn(value)
            OUTPUT("Set Sort Clicked Column : " .. tostring(value))
        end
    end,

    SetHotClick = function(self, value)
        if typeof(value) == "boolean" then
            self.control:SetHotClick(value)
            OUTPUT("Set Hot Click : " .. tostring(value))
        end
    end,

    SetTrackingSelect = function(self, value)
        if typeof(value) == "boolean" then
            self.control:SetTrackingSelect(value)
            OUTPUT("Set Tracking Select : " .. tostring(value))
        end
    end,

    AddColumn = function(self, column)
        if column ~= nil then
            self.control:AddColumn(column.control)
        end
    end,
}