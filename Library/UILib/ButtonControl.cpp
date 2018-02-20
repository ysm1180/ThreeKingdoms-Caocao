#include "ButtonControl.h"

#include "ControlManager.h"
#include "WindowControl.h"

namespace jojogame {
WNDPROC CButtonControl::s_originalProc = nullptr;

LRESULT CALLBACK CButtonControl::OnControlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static bool isHover = false;
    TRACKMOUSEEVENT trackMouseEvent;

    switch (iMessage)
    {
        case WM_CREATE:
        {
            auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            void *lpParamCreate = createStruct->lpCreateParams;
            auto button = reinterpret_cast<CButtonControl *>(lpParamCreate);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(button));

            auto createEvent = button->GetCreateEvent();
            if (createEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(createEvent.c_str(), button);
            }
            break;
        }

        case WM_LBUTTONUP:
        {
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLButtonUpEvent = button->GetMouseLButtonUpEvent();
            if (mouseLButtonUpEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(), button);
            }

            button->_pushed = false;
            break;
        }

        case WM_LBUTTONDOWN:
        {
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLButtonUpEvent = button->GetMouseLButtonDownEvent();

            if (mouseLButtonUpEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(), button);
            }

            button->_pushed = true;

            break;
        }

        case WM_MOUSEMOVE:
        {
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseMoveEvent = button->GetMouseMoveEvent();
            if (mouseMoveEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseMoveEvent.c_str(),
                                                (int) wParam,
                                                GET_X_LPARAM(lParam),
                                                GET_Y_LPARAM(lParam));
            }

            if (isHover == false)
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
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseHoverEvent = button->GetMouseHoverEvent();
            if (mouseHoverEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseHoverEvent.c_str());
            }

            button->_hovered = true;
            InvalidateRect(button->_hWnd, NULL, TRUE);
            UpdateWindow(button->_hWnd);

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
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLeaveEvent = button->GetMouseLeaveEvent();
            if (mouseLeaveEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent.c_str());
            }

            button->_hovered = false;
            InvalidateRect(button->_hWnd, NULL, TRUE);
            UpdateWindow(button->_hWnd);

            isHover = false;
            break;
        }

        case WM_DESTROY:
        {
            break;
        }
    }
    return CallWindowProc(CButtonControl::GetOriginalProc(), hWnd, iMessage, wParam, lParam);
}


void CButtonControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN_CHILD(CButtonControl, "_ButtonControl", CBaseControl);

    LUA_METHOD(GetText);
    LUA_METHOD(GetFont);
    LUA_METHOD(GetBackgroundColor);
    LUA_METHOD(GetBorderColor);
    LUA_METHOD(GetBorderWidth);

    LUA_METHOD(SetTransparentBackground);
    LUA_METHOD(SetText);
    LUA_METHOD(SetParentWindow);
    LUA_METHOD(SetBackgroundColor);
    LUA_METHOD(SetFocusedBackgroundColor);
    LUA_METHOD(SetPushedBackgroundColor);
    LUA_METHOD(SetBorderColor);
    LUA_METHOD(SetFocusedBorderColor);
    LUA_METHOD(SetPushedBorderColor);
    LUA_METHOD(SetTextColor);
    LUA_METHOD(SetFocusedTextColor);
    LUA_METHOD(SetPushedTextColor);
    LUA_METHOD(SetBorderWidth);

    LUA_METHOD(Create);
    LUA_METHOD(Destroy);

    WNDCLASS wndClass;
    GetClassInfo(NULL, TEXT("button"), &wndClass);
    s_originalProc = wndClass.lpfnWndProc;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC) CButtonControl::OnControlProc;
    wndClass.lpszClassName = L"jojo_button";
    RegisterClass(&wndClass);
}

CButtonControl::CButtonControl()
        : _font(this)
{
    //_style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_NOTIFY;
    _style = WS_TABSTOP | BS_OWNERDRAW | WS_CHILD;
}

CButtonControl::~CButtonControl()
{
}

bool CButtonControl::IsTransparentBackground()
{
    return _isTransparentBackground;
}

bool CButtonControl::IsHovered()
{
    return _hovered;
}

bool CButtonControl::IsPushed()
{
    return _pushed;
}

CTextFont *CButtonControl::GetFont()
{
    return &_font;
}

std::wstring CButtonControl::GetText()
{
    return _text;
}

StateColor& CButtonControl::GetBackgroundColor()
{
    return _backgroundColor;
}


StateColor& CButtonControl::GetBorderColor()
{
    return _borderColor;
}

StateColor& CButtonControl::GetTextColor()
{
    return _textColor;
}

int CButtonControl::GetBorderWidth()
{
    return _borderWidth;
}

void CButtonControl::SetTransparentBackground(bool isTransparentBackground)
{
    _isTransparentBackground = isTransparentBackground;

    UpdateWindow(_hWnd);
}

void CButtonControl::SetText(std::wstring text)
{
    _text = text;

    if (_hWnd)
    {
        SetWindowText(_hWnd, _text.c_str());
    }
}

void CButtonControl::SetParentWindow(CWindowControl *parent)
{
    if (parent)
    {
        _parentHWnd = parent->GetHWnd();
    } else
    {
        _parentHWnd = nullptr;
    }
}

void CButtonControl::SetBackgroundColor(COLORREF color)
{
    _backgroundColor.normal = color;
}

void CButtonControl::SetFocusedBackgroundColor(COLORREF color)
{
    _backgroundColor.focused = color;
}

void CButtonControl::SetPushedBackgroundColor(COLORREF color)
{
    _backgroundColor.pushed = color;
}

void CButtonControl::SetBorderColor(COLORREF color)
{
    _borderColor.normal = color;
}

void CButtonControl::SetFocusedBorderColor(COLORREF color)
{
    _borderColor.focused = color;
}

void CButtonControl::SetPushedBorderColor(COLORREF color)
{
    _borderColor.pushed = color;
}

void CButtonControl::SetTextColor(COLORREF color)
{
    _textColor.normal = color;
}

void CButtonControl::SetFocusedTextColor(COLORREF color)
{
    _textColor.focused = color;
}

void CButtonControl::SetPushedTextColor(COLORREF color)
{
    _textColor.pushed = color;
}

void CButtonControl::SetBorderWidth(int width)
{
    _borderWidth = width;
}

bool CButtonControl::Create()
{
    _hWnd = CreateWindow(L"jojo_button",
                         _text.c_str(),
                         _style,
                         _position.x,
                         _position.y,
                         _size.cx,
                         _size.cy,
                         _parentHWnd,
                         (HMENU) _hWnd,
                         CControlManager::GetInstance().GetHInstance(),
                         (LPVOID) this);
    _font.ResetFont();

    return true;
}

void CButtonControl::Destroy()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = nullptr;
    }
}

WNDPROC CButtonControl::GetOriginalProc()
{
    return s_originalProc;
}
}