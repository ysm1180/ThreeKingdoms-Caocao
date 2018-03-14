#include "ListviewControl.h"

#include "ControlManager.h"
#include "WindowControl.h"

#include <CommCtrl.h>
#include <CommCtrl.h>

namespace jojogame {
WNDPROC CListViewControl::s_originalProc = nullptr;

LRESULT CListViewControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool isHover = false;
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
            if (createEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(createEvent.c_str(), listView);
            }
            break;
        }

    case WM_LBUTTONUP:
        {
            auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLButtonUpEvent = listView->GetMouseLButtonUpEvent();
            if (mouseLButtonUpEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(),
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

            if (mouseLButtonUpEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(),
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
            if (mouseMoveEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseMoveEvent.c_str(),
                                                listView,
                                                static_cast<int>(wParam),
                                                GET_X_LPARAM(lParam),
                                                GET_Y_LPARAM(lParam));
            }

            if (!isHover)
            {
                trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
                trackMouseEvent.dwFlags = TME_HOVER;
                trackMouseEvent.hwndTrack = hWnd;
                trackMouseEvent.dwHoverTime = 10;
                TrackMouseEvent(&trackMouseEvent);
            }
            break;
        }

    case WM_MOUSEHOVER:
        {
            auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseHoverEvent = listView->GetMouseHoverEvent();
            if (mouseHoverEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseHoverEvent.c_str(), listView);
            }

            isHover = true;
            trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
            trackMouseEvent.dwFlags = TME_LEAVE;
            trackMouseEvent.hwndTrack = hWnd;
            trackMouseEvent.dwHoverTime = 500;
            TrackMouseEvent(&trackMouseEvent);
            break;
        }

    case WM_MOUSELEAVE:
        {
            auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLeaveEvent = listView->GetMouseLeaveEvent();
            if (mouseLeaveEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent.c_str(), listView);
            }

            isHover = false;
            break;
        }

    case WM_DESTROY:
        {
            break;
        }
    }
    return CallWindowProc(CListViewControl::GetOriginalProc(), hWnd, msg, wParam, lParam);
}

void CListViewColumn::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CListViewColumn, "_ListViewColumn");

    LUA_METHOD(GetAlign);
    LUA_METHOD(GetWidth);
    LUA_METHOD(GetText);

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

CListViewControl* CListViewColumn::GetParentListView()
{
    return _parentListView;
}

int CListViewColumn::GetIndex()
{
    return _index;
}

int CListViewColumn::GetAlign()
{
    return _align;
}

int CListViewColumn::GetWidth()
{
    return _width;
}

std::wstring CListViewColumn::GetText()
{
    return _text;
}

void CListViewColumn::SetParentListView(CListViewControl* parent)
{
    _parentListView = parent;
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

void CListViewItem::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CListViewItem, "_ListViewItem");
}

CListViewItem::CListViewItem()
{
}

CListViewItem::~CListViewItem()
{
}

void CListViewControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN_CHILD(CListViewControl, "_ListView", CBaseControl);

    LUA_METHOD(SetShowColumn);
    LUA_METHOD(SetSortClickedColumn);
    LUA_METHOD(SetHotClick);
    LUA_METHOD(SetTrackingSelect);

    LUA_METHOD(AddColumn);

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
    _style = LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_TABSTOP ;
}

CListViewControl::~CListViewControl()
{
}

bool CListViewControl::IsShowColumn()
{
    return _isShowColumn;
}

bool CListViewControl::IsSortClickedColumn()
{
    return _isSortClckedColumn;
}

bool CListViewControl::IsHotClick()
{
    return _isHotClick;
}

bool CListViewControl::IsTrackingSelect()
{
    return _isTrackingSelect;
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

    if (_hWnd)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CListViewControl::SetHotClick(bool isHotClick)
{
    _isHotClick = isHotClick;
    if (_hWnd)
    {
        DWORD exStyle = ListView_GetExtendedListViewStyle(_hWnd);
        if (_isHotClick)
        {
            ListView_SetExtendedListViewStyle(_hWnd, exStyle | LVS_EX_ONECLICKACTIVATE);
        }
        else
        {
            ListView_SetExtendedListViewStyle(_hWnd, exStyle & (~LVS_EX_ONECLICKACTIVATE));
        }
    }
}

void CListViewControl::SetTrackingSelect(bool isTrackingSelect)
{
    _isTrackingSelect = isTrackingSelect;
    if (_hWnd)
    {
        DWORD dwStyle = ListView_GetExtendedListViewStyle(_hWnd);
        if (_isTrackingSelect)
        {
            ListView_SetExtendedListViewStyle(_hWnd, dwStyle | LVS_EX_TRACKSELECT);
            ListView_SetHoverTime(_hWnd, 1);
        }
        else
        {
            ListView_SetExtendedListViewStyle(_hWnd, dwStyle & (~LVS_EX_TRACKSELECT));
        }
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

void CListViewControl::AddColumn(CListViewColumn* column)
{
    if (_hWnd)
    {
        LVCOLUMN lvColumn;
        int index = _columns.size();
        auto text = column->GetText();

        lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvColumn.fmt = column->GetAlign();
        lvColumn.pszText = (LPWSTR)(text.c_str());
        lvColumn.iSubItem = index;
        lvColumn.cx = column->GetWidth();
        ListView_InsertColumn(_hWnd, index, &lvColumn);

        _columns.push_back(column);

        column->SetIndex(index);
        column->SetParentListView(this);
    }
}

bool CListViewControl::Create()
{
    _hWnd = CreateWindow(L"jojo_listview",
        L"",
        _style,
        _position.x, _position.y,
        _size.cx, _size.cy,
        _parentControl->GetHWnd(),
        nullptr,
        CControlManager::GetInstance().GetHInstance(),
        this);

    return _hWnd != nullptr;
}

void CListViewControl::Destroy()
{
    DestroyWindow(_hWnd);
}

WNDPROC CListViewControl::GetOriginalProc()
{
    return s_originalProc;
}
}
