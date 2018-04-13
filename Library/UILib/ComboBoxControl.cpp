#include "ComboBoxControl.h"
#include "ControlManager.h"

namespace jojogame {

WNDPROC CComboBoxControl::s_originalProc = nullptr;

LRESULT CComboBoxControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT trackMouseEvent;

    switch (msg)
    {
    case WM_CREATE:
    {
        auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void* lpParamCreate = createStruct->lpCreateParams;
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(lpParamCreate);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(comboBoxControl));

        auto createEvent = comboBoxControl->GetCreateEvent();
        if (createEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(createEvent, comboBoxControl);
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = comboBoxControl->GetMouseLButtonUpEvent();
        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
                                            comboBoxControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        comboBoxControl->_isPressed = false;

        InvalidateRect(comboBoxControl->_hWnd, NULL, TRUE);
        UpdateWindow(comboBoxControl->_hWnd);

        break;
    }

    case WM_LBUTTONDOWN:
    {
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = comboBoxControl->GetMouseLButtonDownEvent();

        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
                                            comboBoxControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        comboBoxControl->_isPressed = true;

        InvalidateRect(comboBoxControl->_hWnd, NULL, TRUE);
        UpdateWindow(comboBoxControl->_hWnd);
        break;
    }

    case WM_MOUSEMOVE:
    {
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseMoveEvent = comboBoxControl->GetMouseMoveEvent();
        if (mouseMoveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent,
                                            comboBoxControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        if (!comboBoxControl->_isHover)
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
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseHoverEvent = comboBoxControl->GetMouseEnterEvent();
        if (mouseHoverEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseHoverEvent, comboBoxControl);
        }

        comboBoxControl->_isHover = true;

        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_LEAVE;
        trackMouseEvent.hwndTrack = hWnd;
        trackMouseEvent.dwHoverTime = 100;
        TrackMouseEvent(&trackMouseEvent);

        InvalidateRect(comboBoxControl->_hWnd, NULL, TRUE);
        UpdateWindow(comboBoxControl->_hWnd);
        break;
    }

    case WM_MOUSELEAVE:
    {
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLeaveEvent = comboBoxControl->GetMouseLeaveEvent();
        if (mouseLeaveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, comboBoxControl);
        }

        comboBoxControl->_isPressed = false;
        comboBoxControl->_isHover = false;

        InvalidateRect(comboBoxControl->_hWnd, NULL, TRUE);
        UpdateWindow(comboBoxControl->_hWnd);

        break;
    }

    case WM_DESTROY:
    {
        auto comboBoxControl = reinterpret_cast<CComboBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto destroyEvent = comboBoxControl->GetDestroyEvent();

        if (destroyEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(destroyEvent, comboBoxControl);
        }
        break;
    }
    }
    return CallWindowProc(CComboBoxControl::GetOriginalProc(), hWnd, msg, wParam, lParam);
}

void CComboBoxControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN_CHILD(CComboBoxControl, "_ComboBoxControl", CBaseControl);

    LUA_METHOD(AddItem);

    LUA_METHOD(Create);
    LUA_METHOD(Destroy);

    WNDCLASS wndClass;
    GetClassInfo(NULL, TEXT("combobox"), &wndClass);
    s_originalProc = wndClass.lpfnWndProc;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC)CComboBoxControl::OnControlProc;
    wndClass.lpszClassName = L"jojo_combo";
    RegisterClass(&wndClass);
}

CComboBoxControl::CComboBoxControl()
    : _font(this)
{
    _style = WS_TABSTOP | WS_CHILD | CBS_DROPDOWNLIST;
}

CComboBoxControl::~CComboBoxControl()
{
}

void CComboBoxControl::AddItem(std::wstring item)
{
    _items.push_back(item);
    if (_hWnd != nullptr)
    {
        SendMessage(_hWnd, CB_ADDSTRING, 0, (LPARAM)item.c_str());
    }
}

bool CComboBoxControl::Create()
{
    if (_parentControl != nullptr)
    {
        _hWnd = CreateWindow(L"jojo_combo",
                             L"",
                             _style,
                             _position.x,
                             _position.y,
                             _size.cx,
                             _size.cy,
                             _parentControl->GetHWnd(),
                             (HMENU)this,
                             CControlManager::GetInstance().GetHInstance(),
                             this);

        if (_hWnd != nullptr)
        {
            for (auto item : _items)
            {
                SendMessage(_hWnd, CB_ADDSTRING, 0, (LPARAM)item.c_str());
            }
        }

        _font.ResetFont();
    }

    return _hWnd != nullptr;
}

WNDPROC CComboBoxControl::GetOriginalProc()
{
    return s_originalProc;
}

}
