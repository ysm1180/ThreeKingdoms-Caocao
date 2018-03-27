require "Script\\control_manager.lua"
require "Script\\listview.lua"

ListViewManager = ControlManager:Instance
{
    CreateListView = function(self, options)
        OUTPUT("-------- Start Create : ListView --------")

        local newListView = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListView:Instance(options._Control)
            end

            newListView = ListView:New(options.Parent)

            newListView:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newListView:Move(options.X, options.Y)

            newListView:SetRowHeight(options.RowHeight)

            if options.Event then
                newListView:SetCreateEvent(options.Event.Create)
                newListView:SetDestroyEvent(options.Event.Destroy)
                newListView:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newListView:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newListView:SetMouseMoveEvent(options.Event.MouseMove)
                newListView:SetMouseEnterEvent(options.Event.MouseEnter)
                newListView:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            if options.Option then
                newListView:SetShowBorder(options.Option.ShowBorder)
                newListView:SetShowColumn(options.Option.ShowColumn)
                newListView:SetSortClickedColumn(options.Option.SortClickedColumn)
                newListView:SetOneClickItemActivated(options.Option.OneClickItemActivated)
                newListView:SetTrackingSelect(options.Option.TrackingSelect)
            end

            newListView:SetUserData(options.UserData)

            newListView:Create()

            if options.Columns then    
                for i = 1, #options.Columns do
                    newListView:AddColumn(options.Columns[i])
                end
            end

            if options.Rows then
                for i = 1, #options.Rows do
                    newListView:AddRow(options.Rows[i])
                end
            end

            if options.Show then
                newListView:Show()
            end
        end

        OUTPUT("-------- End Create : ListView --------")
        return newListView
    end,

    CreateListViewInstance = function(self, control)
        OUTPUT("-------- Create ListView By Instance --------")
        

        local options = {
            _Control = control,
        }

        OUTPUT("-------- End Create ListView By Instance --------")        
        
        return self:CreateListView(options)
    end,

    CreateColumn = function(self, options)
        OUTPUT("-------- Start Create : ListView Column --------")

        local newListViewColumn = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListViewColumn:Instance(options._Control)
            end

            newListViewColumn = ListViewColumn:New()

            newListViewColumn:SetWidth(options.Width)            

            newListViewColumn:SetAutoSizeFitItem(options.AutoSizeFitItem)
            newListViewColumn:SetAutoSizeFitHeader(options.AutoSizeFitHeader)

            if options.Text then
                newListViewColumn:SetText(options.Text.Content)
                newListViewColumn:SetAlign(options.Text.Align)
            end
        end

        OUTPUT("-------- End Create : ListView Column --------")
        return newListViewColumn
    end,

    CreateColumnInstance = function(self, control)
        OUTPUT("-------- Create ListView Column By Instance --------")

        local options = {
            _Control = control,
        }

        OUTPUT("-------- End Create ListView Column By Instance --------")        
        
        return self:CreateColumn(options)
    end,

    CreateRow = function(self, options)
        OUTPUT("-------- Start Create : ListView Row --------")

        local newListViewRow = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListViewRow:Instance(options._Control)
            end

            newListViewRow = ListViewRow:New()            

            newListViewRow:SetEnabled(options.Enabled)

            if options.Event ~= nil then
                newListRow:SetActiveEvent(options.Event.Active)
            end

            if options.Items ~= nil then
                for i = 1, #options.Items do
                    newListViewRow:AddItem(options.Items[i])
                end
            end

            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    newListViewRow:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newListViewRow:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newListViewRow:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newListViewRow:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end

            if options.Text ~= nil then
                if options.Text.Color ~= nil then
                    newListViewRow:SetNormalTextColor(options.Text.Color.Normal)
                    newListViewRow:SetFocusedTextColor(options.Text.Color.Focused)
                    newListViewRow:SetDisabledTextColor(options.Text.Color.Disabled)
                    newListViewRow:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
            end                     
        end
		
        OUTPUT("-------- End Create : ListView Row --------")
        return newListViewRow
    end,

    CreateRowInstnace = function(self, control)
        OUTPUT("-------- Create ListView Row By Instance --------")

        local options = {
            _Control = control,
        }

        OUTPUT("-------- End Create ListView Row By Instance --------")        
        
        return self:CreateRow(options)
    end,

    CreateItem = function(self, options)
        OUTPUT("-------- Start Create : ListView Item --------")

        local newListViewItem = nil

        if options ~= nil then
            if options._Control ~= nil then
                return ListViewItem:Instance(options._Control)
            end

            newListViewItem = ListViewItem:New()

            if options.Background ~= nil then
                if options.Background.Color ~= nil then
                    newListViewItem:SetNormalBackgroundColor(options.Background.Color.Normal)
                    newListViewItem:SetFocusedBackgroundColor(options.Background.Color.Focused)
                    newListViewItem:SetDisabledBackgroundColor(options.Background.Color.Disabled)
                    newListViewItem:SetDisableFocusedBackgroundColor(options.Background.Color.DisableFocused)
                end
            end

            if options.Text ~= nil then
                if options.Text.Font ~= nil then
                    newListViewItem:SetFontName(options.Text.Font.Name)
                    newListViewItem:SetFontSize(options.Text.Font.Size)
                    newListViewItem:SetBold(options.Text.Font.Bold)
                    newListViewItem:SetUnderline(options.Text.Font.Underline)
                    newListViewItem:SetItalic(options.Text.Font.Italic)
                end

                newListViewItem:SetAlign(options.Text.Align)
                newListViewItem:SetText(options.Text.Content)
                if options.Text.Color ~= nil then
                    newListViewItem:SetNormalTextColor(options.Text.Color.Normal)
                    newListViewItem:SetFocusedTextColor(options.Text.Color.Focused)
                    newListViewItem:SetDisabledTextColor(options.Text.Color.Disabled)
                    newListViewItem:SetDisableFocusedTextColor(options.Text.Color.DisableFocused)
                end
            end      
        end
		
        OUTPUT("-------- End Create : ListView Item --------")
        return newListViewItem
    end,
}