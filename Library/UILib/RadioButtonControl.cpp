#include "RadioButtonControl.h"
#include "ControlManager.h"

namespace three_kingdoms
{
WNDPROC CRadioButtonControl::s_originalProc = nullptr;

LRESULT CRadioButtonControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT trackMouseEvent;

    switch (msg)
    {
    case WM_CREATE:
    {
        auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void *lpParamCreate = createStruct->lpCreateParams;
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(lpParamCreate);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(radioButtonControl));

        auto createEvent = radioButtonControl->GetCreateEvent();
        if (createEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(createEvent, radioButtonControl);
        }

        radioButtonControl->_theme = OpenThemeData(hWnd, L"Button");
        SetWindowTheme(hWnd, L"", L"");
        break;
    }

    case WM_LBUTTONUP:
    {
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = radioButtonControl->GetMouseLButtonUpEvent();
        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
                                            radioButtonControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        radioButtonControl->_isPressed = false;

        InvalidateRect(radioButtonControl->_hWnd, NULL, TRUE);
        UpdateWindow(radioButtonControl->_hWnd);

        break;
    }

    case WM_LBUTTONDOWN:
    {
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = radioButtonControl->GetMouseLButtonDownEvent();

        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
                                            radioButtonControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        radioButtonControl->_isPressed = true;

        InvalidateRect(radioButtonControl->_hWnd, NULL, TRUE);
        UpdateWindow(radioButtonControl->_hWnd);
        break;
    }

    case WM_MOUSEMOVE:
    {
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseMoveEvent = radioButtonControl->GetMouseMoveEvent();
        if (mouseMoveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent,
                                            radioButtonControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        if (!radioButtonControl->_isHover)
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
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseHoverEvent = radioButtonControl->GetMouseEnterEvent();
        if (mouseHoverEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseHoverEvent, radioButtonControl);
        }

        radioButtonControl->_isHover = true;

        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_LEAVE;
        trackMouseEvent.hwndTrack = hWnd;
        trackMouseEvent.dwHoverTime = 100;
        TrackMouseEvent(&trackMouseEvent);

        InvalidateRect(radioButtonControl->_hWnd, NULL, TRUE);
        UpdateWindow(radioButtonControl->_hWnd);
        break;
    }

    case WM_MOUSELEAVE:
    {
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLeaveEvent = radioButtonControl->GetMouseLeaveEvent();
        if (mouseLeaveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, radioButtonControl);
        }

        radioButtonControl->_isPressed = false;
        radioButtonControl->_isHover = false;

        InvalidateRect(radioButtonControl->_hWnd, NULL, TRUE);
        UpdateWindow(radioButtonControl->_hWnd);

        break;
    }

    case WM_DESTROY:
    {
        auto radioButtonControl = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto destroyEvent = radioButtonControl->GetDestroyEvent();

        if (destroyEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(destroyEvent, radioButtonControl);
        }
        break;
    }
    }
    return CallWindowProc(CRadioButtonControl::GetOriginalProc(), hWnd, msg, wParam, lParam);
}

void CRadioButtonControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN_CHILD(CRadioButtonControl, "_RadioButtonControl", CWindowChildControl);

    LUA_METHOD(IsChecked);
    LUA_METHOD(GetText);

    LUA_METHOD(SetText);
    LUA_METHOD(SetChecked);

    LUA_METHOD(Create);
    LUA_METHOD(Destroy);

    WNDCLASS wndClass;
    GetClassInfo(NULL, TEXT("Button"), &wndClass);
    s_originalProc = wndClass.lpfnWndProc;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC)CRadioButtonControl::OnControlProc;
    wndClass.lpszClassName = L"jojo_radio";
    RegisterClass(&wndClass);
}

CRadioButtonControl::CRadioButtonControl(bool isGroupStart)
    : _font(this)
{
    _style = WS_TABSTOP | WS_CHILD | BS_AUTORADIOBUTTON | (isGroupStart ? WS_GROUP : 0);
    _type = L"radiobutton";
}

CRadioButtonControl::~CRadioButtonControl()
{
}

bool CRadioButtonControl::IsChecked()
{
    if (_hWnd != nullptr)
    {
        return SendMessage(_hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
    }
    else
    {
        return _isChecked;
    }
}

std::wstring CRadioButtonControl::GetText()
{
    return _text;
}

CTextFont *CRadioButtonControl::GetFont()
{
    return &_font;
}

HTHEME CRadioButtonControl::GetTheme()
{
    return _theme;
}

void CRadioButtonControl::SetText(std::wstring text)
{
    if (_text != text)
    {
        _text = text;

        if (_hWnd != nullptr)
        {
            SetWindowText(_hWnd, _text.c_str());
        }
    }
}

void CRadioButtonControl::SetChecked(bool checked)
{
    _isChecked = checked;
    if (_hWnd != nullptr)
    {
        if (checked)
        {
            SendMessage(_hWnd, BM_SETCHECK, BST_CHECKED, 0);
        }
        else
        {
            SendMessage(_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
        }
    }
}

bool CRadioButtonControl::Create()
{
    if (_parentControl != nullptr)
    {
        _hWnd = CreateWindow(L"jojo_radio",
                             _text.c_str(),
                             _style,
                             _position.x,
                             _position.y,
                             _size.cx,
                             _size.cy,
                             _parentControl->GetHWnd(),
                             (HMENU)this,
                             CControlManager::GetInstance().GetHInstance(),
                             this);

        if (_isChecked)
        {
            SendMessage(_hWnd, BM_SETCHECK, BST_CHECKED, 0);
        }
        else
        {
            SendMessage(_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
        }

        _font.ResetFont();
    }

    return _hWnd != nullptr;
}

WNDPROC CRadioButtonControl::GetOriginalProc()
{
    return s_originalProc;
}

} // namespace jojogame
