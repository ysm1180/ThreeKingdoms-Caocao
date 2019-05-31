#include "ListviewControl.h"

#include "ControlManager.h"
#include "WindowControl.h"

#include <CommCtrl.h>
#include <CommCtrl.h>

namespace jojogame {
WNDPROC CListViewControl::s_originalProc = nullptr;

LRESULT CListViewControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT trackMouseEvent;

    switch (msg)
    {
    case WM_CREATE:
    {
        auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void* lpParamCreate = createStruct->lpCreateParams;
        auto listView = reinterpret_cast<CListViewControl *>(lpParamCreate);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(listView));

        auto createEvent = listView->GetCreateEvent();
        if (createEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(createEvent, listView);
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = listView->GetMouseLButtonUpEvent();
        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
                                            listView,
                                            static_cast<int>(wParam),
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        break;
    }

    case WM_LBUTTONDOWN:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = listView->GetMouseLButtonDownEvent();

        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
                                            listView,
                                            static_cast<int>(wParam),
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        break;
    }

    case WM_MOUSEMOVE:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseMoveEvent = listView->GetMouseMoveEvent();
        if (mouseMoveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent,
                                            listView,
                                            static_cast<int>(wParam),
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        if (!listView->_isHover)
        {
            trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
            trackMouseEvent.dwFlags = TME_HOVER;
            trackMouseEvent.hwndTrack = hWnd;
            trackMouseEvent.dwHoverTime = 1;
            TrackMouseEvent(&trackMouseEvent);
        }
        break;
    }

    case WM_MOUSEHOVER:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (!listView->_isHover)
        {
            auto mouseEnterEvent = listView->GetMouseEnterEvent();
            if (mouseEnterEvent != LUA_NOREF)
            {
                CLuaTinker::GetLuaTinker().Call(mouseEnterEvent, listView);
            }
        }

        listView->_isHover = true;
        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_LEAVE;
        trackMouseEvent.hwndTrack = hWnd;
        trackMouseEvent.dwHoverTime = 10;
        TrackMouseEvent(&trackMouseEvent);

        if (!listView->IsTrackingSelect())
        {
            return 0;
        }
        break;
    }

    case WM_MOUSELEAVE:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLeaveEvent = listView->GetMouseLeaveEvent();
        if (mouseLeaveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, listView);
        }

        listView->_isHover = false;
        break;
    }

    case WM_DESTROY:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto destroyEvent = listView->GetDestroyEvent();

        if (destroyEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(destroyEvent, listView);
        }

        break;
    }

    case WM_ERASEBKGND:
    {
        auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (!listView->IsTransparentBackground())
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            FillRect((HDC)wParam, &rect, listView->GetBackgroundBrush());
        }
        return TRUE;
    }
    }


    return CallWindowProc(CListViewControl::GetOriginalProc(), hWnd, msg, wParam, lParam);
}



void CListViewColumn::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CListViewColumn, "_ListViewColumn");

    LUA_METHOD(IsAutoSizeFitItem);
    LUA_METHOD(IsAutoSizeFitHeader);
    LUA_METHOD(GetAlign);
    LUA_METHOD(GetWidth);
    LUA_METHOD(GetText);

    LUA_METHOD(SetAutoSizeFitItem);
    LUA_METHOD(SetAutoSizeFitHeader);
    LUA_METHOD(SetAlign);
    LUA_METHOD(SetWidth);
    LUA_METHOD(SetText);
}

CListViewColumn::CListViewColumn()
{
}

CListViewColumn::~CListViewColumn()
{
}

bool CListViewColumn::IsAutoSizeFitItem() const
{
    return _isAutoSizeFitItem;
}

bool CListViewColumn::IsAutoSizeFitHeader() const
{
    return _isAutoSizeFitHeader;
}

int CListViewColumn::GetIndex() const
{
    return _index;
}

int CListViewColumn::GetAlign() const
{
    return _align;
}

int CListViewColumn::GetWidth() const
{
    if (_isAutoSizeFitItem || _isAutoSizeFitHeader)
    {
        if (_parentListView)
        {
            return ListView_GetColumnWidth(_parentListView->GetHWnd(), _index);
        }
    }
    return _width;
}

std::wstring CListViewColumn::GetText() const
{
    return _text;
}

void CListViewColumn::SetParentListView(CListViewControl* parent)
{
    _parentListView = parent;
}

void CListViewColumn::SetAutoSizeFitItem(bool value)
{
    _isAutoSizeFitItem = value;

    if (_parentListView)
    {
        ListView_SetColumnWidth(_parentListView->GetHWnd(), _index, LVSCW_AUTOSIZE);
    }
}

void CListViewColumn::SetAutoSizeFitHeader(bool value)
{
    _isAutoSizeFitHeader = value;

    if (_parentListView)
    {
        ListView_SetColumnWidth(_parentListView->GetHWnd(), _index, LVSCW_AUTOSIZE_USEHEADER);
    }
}

void CListViewColumn::SetIndex(int index)
{
    _index = index;
}

void CListViewColumn::SetText(std::wstring text)
{
    _text = text;

    if (_parentListView)
    {
        LVCOLUMN lvColumn;
        lvColumn.mask = LVCF_TEXT;
        lvColumn.pszText = const_cast<wchar_t *>(_text.c_str());
        ListView_SetColumn(_parentListView->GetHWnd(), _index, &lvColumn);
    }
}

void CListViewItem::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CListViewItem, "_ListViewItem");

    LUA_METHOD(GetNormalBackgroundColor);
    LUA_METHOD(GetFocusedBackgroundColor);
    LUA_METHOD(GetDisabledBackgroundColor);
    LUA_METHOD(GetDisableFocusedBackgroundColor);
    LUA_METHOD(GetNormalTextColor);
    LUA_METHOD(GetFocusedTextColor);
    LUA_METHOD(GetDisabledTextColor);
    LUA_METHOD(GetDisableFocusedTextColor);
    LUA_METHOD(GetText);
    LUA_METHOD(GetAlign);
    LUA_METHOD(GetFont);

    LUA_METHOD(SetNormalBackgroundColor);
    LUA_METHOD(SetFocusedBackgroundColor);
    LUA_METHOD(SetDisabledBackgroundColor);
    LUA_METHOD(SetDisableFocusedBackgroundColor);
    LUA_METHOD(SetNormalTextColor);
    LUA_METHOD(SetFocusedTextColor);
    LUA_METHOD(SetDisabledTextColor);
    LUA_METHOD(SetDisableFocusedTextColor);
    LUA_METHOD(SetText);
    LUA_METHOD(SetAlign);
}

CListViewItem::CListViewItem()
{
}

CListViewItem::~CListViewItem()
{
}

std::wstring CListViewItem::GetText()
{
    return _text;
}

CTextFont* CListViewItem::GetFont()
{
    return &_font;
}

COLORREF CListViewItem::GetNormalBackgroundColor()
{
    return _backgroundColor.normal;
}

COLORREF CListViewItem::GetFocusedBackgroundColor()
{
    return _backgroundColor.focused;
}

COLORREF CListViewItem::GetDisabledBackgroundColor()
{
    return _backgroundColor.disabled;
}

COLORREF CListViewItem::GetDisableFocusedBackgroundColor()
{
    return _backgroundColor.disableFocused;
}

COLORREF CListViewItem::GetNormalTextColor()
{
    return _textColor.normal;
}

COLORREF CListViewItem::GetFocusedTextColor()
{
    return _textColor.focused;
}

COLORREF CListViewItem::GetDisabledTextColor()
{
    return _textColor.disabled;
}

COLORREF CListViewItem::GetDisableFocusedTextColor()
{
    return _textColor.disableFocused;
}

int CListViewItem::GetAlign()
{
    return _align;
}

void CListViewItem::SetParentListRow(CListViewRow* parent)
{
    _parentListRow = parent;
}

void CListViewItem::SetText(std::wstring text)
{
    _text = text;
}

void CListViewItem::SetNormalBackgroundColor(COLORREF color)
{
    _backgroundColor.normal = color;

    _Update();
}

void CListViewItem::SetFocusedBackgroundColor(COLORREF color)
{
    _backgroundColor.focused = color;

    _Update();
}

void CListViewItem::SetDisabledBackgroundColor(COLORREF color)
{
    _backgroundColor.disabled = color;

    _Update();
}

void CListViewItem::SetDisableFocusedBackgroundColor(COLORREF color)
{
    _backgroundColor.disableFocused = color;

    _Update();
}

void CListViewItem::SetNormalTextColor(COLORREF color)
{
    _textColor.normal = color;

    _Update();
}

void CListViewItem::SetFocusedTextColor(COLORREF color)
{
    _textColor.focused = color;

    _Update();
}

void CListViewItem::SetDisabledTextColor(COLORREF color)
{
    _textColor.disabled = color;

    _Update();
}

void CListViewItem::SetDisableFocusedTextColor(COLORREF color)
{
    _textColor.disableFocused = color;

    _Update();
}

void CListViewItem::SetAlign(int align)
{
    _align = align;

    _Update();
}

void CListViewItem::SetItemIndex(int itemIndex)
{
    _itemIndex = itemIndex;
}

void CListViewItem::_Update()
{
    if (_parentListRow && _parentListRow->GetParentListView())
    {
        auto listView = _parentListRow->GetParentListView();
        auto listViewHWnd = listView->GetHWnd();

        RECT rect;
        ListView_GetSubItemRect(listViewHWnd, _parentListRow->GetRowIndex(), _itemIndex, LVIR_BOUNDS, &rect);
        InvalidateRect(listViewHWnd, &rect, TRUE);
        UpdateWindow(listViewHWnd);
    }
}

void CListViewColumn::SetAlign(int align)
{
    _align = align;

    if (_parentListView)
    {
        LVCOLUMN lvColumn;
        lvColumn.mask = LVCF_FMT;
        lvColumn.fmt = _align;
        ListView_SetColumn(_parentListView->GetHWnd(), _index, &lvColumn);
    }
}

void CListViewColumn::SetWidth(int width)
{
    _width = width;

    if (_parentListView)
    {
        ListView_SetColumnWidth(_parentListView->GetHWnd(), _index, _width);
    }
}

void CListViewRow::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CListViewRow, "_ListViewItemRow");

    LUA_METHOD(IsEnabled);
    LUA_METHOD(GetActiveEvent);
    LUA_METHOD(GetItem);

    LUA_METHOD(SetNormalBackgroundColor);
    LUA_METHOD(SetFocusedBackgroundColor);
    LUA_METHOD(SetNormalTextColor);
    LUA_METHOD(SetFocusedTextColor);
    LUA_METHOD(SetActiveEvent);
    LUA_METHOD(SetEnabled);
    LUA_METHOD(SetItem);

    LUA_METHOD(AddItem);
}

CListViewRow::CListViewRow()
{
}

CListViewRow::~CListViewRow()
{
    if (_activeEvent != LUA_NOREF)
    {
        luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _activeEvent);
    }
}

bool CListViewRow::IsEnabled()
{
    return _isEnabled;
}

CListViewControl* CListViewRow::GetParentListView()
{
    return _parentListView;
}

int CListViewRow::GetActiveEvent()
{
    return _activeEvent;
}

CListViewItem* CListViewRow::GetItem(int subIndex)
{
    if (subIndex > _items.size() || subIndex < 1)
    {
        return nullptr;
    }

    return _items[subIndex - 1];
}

int CListViewRow::GetRowIndex()
{
    return _rowIndex;
}

void CListViewRow::SetEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;

    if (_parentListView)
    {
        RECT rect;
        ListView_GetItemRect(_parentListView->GetHWnd(), _rowIndex, &rect, LVIR_BOUNDS);
        InvalidateRect(_parentListView->GetHWnd(), &rect, TRUE);
        UpdateWindow(_parentListView->GetHWnd());
    }
}

void CListViewRow::SetParentListView(CListViewControl* parent)
{
    _parentListView = parent;
}

void CListViewRow::SetNormalBackgroundColor(COLORREF color)
{
    for (auto& item : _items)
    {
        item->SetNormalBackgroundColor(color);
    }
}

void CListViewRow::SetFocusedBackgroundColor(COLORREF color)
{
    for (auto& item : _items)
    {
        item->SetFocusedBackgroundColor(color);
    }
}

void CListViewRow::SetNormalTextColor(COLORREF color)
{
    for (auto& item : _items)
    {
        item->SetNormalTextColor(color);
    }
}

void CListViewRow::SetFocusedTextColor(COLORREF color)
{
    for (auto& item : _items)
    {
        item->SetFocusedTextColor(color);
    }
}

void CListViewRow::SetActiveEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _activeEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
}

void CListViewRow::SetItem(int subIndex, CListViewItem* item)
{
    if (subIndex > _items.size() || subIndex < 1)
    {
        return;
    }

    _items[subIndex - 1] = item;
}

void CListViewRow::SetRowIndex(int index)
{
    _rowIndex = index;
}

void CListViewRow::AddItem(CListViewItem* item)
{
    item->SetParentListRow(this);
    item->SetItemIndex(_items.size());
    _items.push_back(item);
}

void CListViewControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN_CHILD(CListViewControl, "_ListView", CWindowChildControl);

    LUA_METHOD(IsTransparentBackground);
    LUA_METHOD(GetColumn);
    LUA_METHOD(GetRow);

    LUA_METHOD(SetShowBoder);
    LUA_METHOD(SetShowColumn);
    LUA_METHOD(SetSortClickedColumn);
    LUA_METHOD(SetOneClickItemActivated);
    LUA_METHOD(SetTrackingSelect);
    LUA_METHOD(SetTransparentBackground);
    LUA_METHOD(SetRowHeight);

    LUA_METHOD(AddColumn);
    LUA_METHOD(AddRow);

    LUA_METHOD(Create);
    LUA_METHOD(Destroy);

    WNDCLASS wndClass;
    GetClassInfo(nullptr, TEXT("SysListView32"), &wndClass);
    s_originalProc = wndClass.lpfnWndProc;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC)OnControlProc;
    wndClass.lpszClassName = L"jojo_listview";
    RegisterClass(&wndClass);
}

CListViewControl::CListViewControl()
{
    _style = LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_TABSTOP | LVS_OWNERDRAWFIXED | WS_BORDER;
    _exStyle = LVS_EX_TWOCLICKACTIVATE;
    _type = L"listview";
}

CListViewControl::~CListViewControl()
{
    if (_backgroundBrush)
    {
        DeleteBrush(_backgroundBrush);
    }
}

bool CListViewControl::IsShowBorder() const
{
    return _isShowBorder;
}

bool CListViewControl::IsShowColumn() const
{
    return _isShowColumn;
}

bool CListViewControl::IsSortClickedColumn() const
{
    return _isSortClckedColumn;
}

bool CListViewControl::IsOneClickItemActivated() const
{
    return _isOneClickItemActivated;
}

bool CListViewControl::IsTrackingSelect() const
{
    return _isTrackingSelect;
}

bool CListViewControl::IsTransparentBackground() const
{
    return _isTransparentBackground;
}

CListViewColumn* CListViewControl::GetColumn(int columnIndex)
{
    if (columnIndex > _columns.size() || columnIndex < 1)
    {
        return nullptr;
    }
    else
    {
        return _columns[columnIndex - 1];
    }
}

CListViewRow* CListViewControl::GetRow(int rowIndex)
{
    if (rowIndex > _rows.size() || rowIndex < 1)
    {
        return nullptr;
    }
    else
    {
        return _rows[rowIndex - 1];
    }
}

int CListViewControl::GetColumnCount()
{
    return _columns.size();
}

COLORREF CListViewControl::GetBackgroundColor()
{
    return _backgroundColor;
}

HBRUSH CListViewControl::GetBackgroundBrush()
{
    return _backgroundBrush;
}

int CListViewControl::GetRowHeight()
{
    return _rowHeight;
}

void CListViewControl::SetShowBoder(bool isShowBorder)
{
    _isShowBorder = isShowBorder;

    if (_isShowBorder)
    {
        _style |= WS_BORDER;
    }
    else
    {
        _style &= ~WS_BORDER;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CListViewControl::SetShowColumn(bool isShowColumn)
{
    _isShowColumn = isShowColumn;
    if (!_isShowColumn)
    {
        _style |= LVS_NOCOLUMNHEADER;
    }
    else
    {
        _style &= ~LVS_NOCOLUMNHEADER;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CListViewControl::SetSortClickedColumn(bool isSortClickedColumn)
{
    _isSortClckedColumn = isSortClickedColumn;
    if (!_isSortClckedColumn)
    {
        _style |= LVS_NOSORTHEADER;
    }
    else
    {
        _style &= ~LVS_NOSORTHEADER;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CListViewControl::SetOneClickItemActivated(bool isHotClick)
{
    _isOneClickItemActivated = isHotClick;

    if (_isOneClickItemActivated)
    {
        _exStyle |= LVS_EX_ONECLICKACTIVATE;
        _exStyle &= ~LVS_EX_TWOCLICKACTIVATE;
    }
    else
    {
        _exStyle &= ~LVS_EX_ONECLICKACTIVATE;
        _exStyle |= LVS_EX_TWOCLICKACTIVATE;
    }

    if (_hWnd != nullptr)
    {
        ListView_SetExtendedListViewStyle(_hWnd, _exStyle);
    }
}

void CListViewControl::SetTrackingSelect(bool isTrackingSelect)
{
    _isTrackingSelect = isTrackingSelect;

    if (_isTrackingSelect)
    {
        _exStyle |= LVS_EX_TRACKSELECT;
    }
    else
    {
        _exStyle &= ~LVS_EX_TRACKSELECT;
    }

    if (_hWnd != nullptr)
    {
        ListView_SetExtendedListViewStyle(_hWnd, _exStyle);
        ListView_SetHoverTime(_hWnd, 1);
    }
}

void CListViewControl::SetTransparentBackground(bool isTransparentBackground)
{
    if (_isTransparentBackground != isTransparentBackground)
    {
        _isTransparentBackground = isTransparentBackground;

        Refresh();
    }
}

void CListViewControl::SetParentWindow(CWindowControl* parent)
{
    if (parent)
    {
        _parentControl = parent;
    }
    else
    {
        _parentControl = nullptr;
    }
}

void CListViewControl::SetBackgroundColor(COLORREF color)
{
    if (_backgroundColor != color)
    {
        _backgroundColor = color;
    }
}

void CListViewControl::SetRowHeight(int rowHeight)
{
    if (_rowHeight != rowHeight)
    {
        _rowHeight = rowHeight;
    }
}

void CListViewControl::AppendColumn(CListViewColumn* column)
{
    int index;

    if (_hWnd != nullptr)
    {
        LVCOLUMN lvColumn;
        auto text = column->GetText();
        if (column->GetIndex() == -1)
        {
            index = _columns.size();
        } else
        {
            index = column->GetIndex();
        }

        lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvColumn.fmt = column->GetAlign();
        lvColumn.pszText = const_cast<LPWSTR>(text.c_str());
        lvColumn.iSubItem = index;
        lvColumn.cx = column->GetWidth();
        ListView_InsertColumn(_hWnd, index, &lvColumn);
        if (column->IsAutoSizeFitHeader())
        {
            ListView_SetColumnWidth(_hWnd, index, LVSCW_AUTOSIZE_USEHEADER);
        }
        else if (column->IsAutoSizeFitItem())
        {
            ListView_SetColumnWidth(_hWnd, index, LVSCW_AUTOSIZE);
        }
    }
}

void CListViewControl::AddColumn(CListViewColumn* column)
{
    const int index = _columns.size();

    this->AppendColumn(column);

    column->SetIndex(index);
    column->SetParentListView(this);
    _columns.push_back(column);
}

void CListViewControl::AppendRow(CListViewRow* row)
{
    if (_hWnd != nullptr)
    {
        int itemIndex;

        for (auto i = 0; i < _columns.size(); ++i)
        {
            auto listViewItem = row->GetItem(i + 1);
            std::wstring text;
            if (listViewItem)
            {
                text = listViewItem->GetText();
            }
            else
            {
                text = L"";
            }

            if (row->GetRowIndex() == -1)
            {
                itemIndex = _rows.size();
            } else
            {
                itemIndex = row->GetRowIndex();
            }

            if (i == 0)
            {
                LVITEM item;

                item.mask = LVIF_TEXT | LVIF_PARAM;
                item.pszText = const_cast<wchar_t *>(text.c_str());
                item.iItem = itemIndex;
                item.iSubItem = 0;
                item.lParam = reinterpret_cast<LPARAM>(row);
                ListView_InsertItem(_hWnd, &item);
            }
            else
            {
                ListView_SetItemText(_hWnd, itemIndex, i, const_cast<wchar_t *>(text.c_str()));
            }

            if (_columns[i]->IsAutoSizeFitItem())
            {
                ListView_SetColumnWidth(_hWnd, i, LVSCW_AUTOSIZE);
            }
            else if (_columns[i]->IsAutoSizeFitHeader())
            {
                ListView_SetColumnWidth(_hWnd, i, LVSCW_AUTOSIZE_USEHEADER);
            }
        }
    }
}

void CListViewControl::AddRow(CListViewRow* row)
{
    const int itemIndex = _rows.size();

    this->AppendRow(row);

    row->SetRowIndex(itemIndex);
    row->SetParentListView(this);
    _rows.push_back(row);
}

bool CListViewControl::Create()
{
    _hWnd = CreateWindow(L"jojo_listview",
        L"",
        _style,
        _position.x, _position.y,
        _size.cx, _size.cy,
        _parentControl->GetHWnd(),
        (HMENU)this,
        CControlManager::GetInstance().GetHInstance(),
        this);

    if (_hWnd != nullptr)
    {
        ListView_SetExtendedListViewStyle(_hWnd, _exStyle | LVS_EX_FULLROWSELECT);
        ListView_SetHoverTime(_hWnd, 1);

        for (auto column : _columns)
        {
            this->AppendColumn(column);
        }

        for (auto row : _rows)
        {
            this->AppendRow(row);
        }
    }

    return _hWnd != nullptr;
}

WNDPROC CListViewControl::GetOriginalProc()
{
    return s_originalProc;
}
}
