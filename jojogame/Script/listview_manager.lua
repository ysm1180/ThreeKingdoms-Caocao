require "Script\\control_manager.lua"
require "Script\\listview.lua"

ListViewManager = ControlManager:Instance
{
    Create = function(self, options)
        OUTPUT("-------- Start Create : ListView --------")

        local newListView = nil

        if options ~= nil then
            if options.Control ~= nil then
                newListView = ListView:Instance(options.Control)
                return newListView
            else
                newListView = ListView:New(options.Parent)
            end

            newListView:SetSize(options.Width, options.Height)

            if options.Center then
                local x, y = ControlManager:GetCenterPosition(options.Parent, options.Width, options.Height, true)
                if x ~= nil and y ~= nil then
                    options.X = x
                    options.Y = y
                end
            end
            newListView:Move(options.X, options.Y)

            if options.Event then
                newListView:SetCreateEvent(options.Event.Create)
                newListView:SetDestroyEvent(options.Event.Destroy)
                newListView:SetMouseLButtonUpEvent(options.Event.MouseLButtonUp)
                newListView:SetMouseLButtonDownEvent(options.Event.MouseLButtonDown)
                newListView:SetMouseMoveEvent(options.Event.MouseMove)
                newListView:SetMouseHoverEvent(options.Event.MouseHover)
                newListView:SetMouseLeaveEvent(options.Event.MouseLeave)
            end

            if options.Option then
                newListView:SetShowColumn(options.Option.ShowColumn)
                newListView:SetSortClickColumn(options.Option.SortClickedColumn)
                newListView:SetHotClick(options.Option.HotClick)
                newListView:SetTrackingSelect(options.Option.TrackingSelect)
            end

            newListView:SetUserData(options.UserData)

            newListView:Create()

            if options.Columns then    
                for i = 1, #options.Columns do
                    OUTPUT(i)
                    newListView:AddColumn(options.Columns[i])
                end
            end

            if options.Show then
                newListView:Show()
            end
        end

        OUTPUT("-------- End Create : ListView --------")
        return newListView
    end,

    CreateColumn = function(self, options)
        OUTPUT("-------- Start Create : ListView Column --------")

        local newListViewColumn = nil

        if options ~= nil then
            if options.Control ~= nil then
                newListViewColumn = ListViewColumn:Instance(options.Control)
                return newListView
            else
                newListViewColumn = ListViewColumn:New()
            end

            newListViewColumn:SetWidth(options.Width)            

            if options.Text then
                newListViewColumn:SetText(options.Text.Content)
                newListViewColumn:SetAlign(options.Text.Align)
            end
        end

        OUTPUT("-------- End Create : ListView Column --------")
        return newListViewColumn
    end,
}