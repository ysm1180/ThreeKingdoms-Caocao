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

    LUA_METHOD(SetTransparentBackground);
    LUA_METHOD(SetTransparentBorder);
    LUA_METHOD(SetText);
    LUA_METHOD(SetParentWindow);

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
    _style = WS_TABSTOP | BS_PUSHBUTTON | WS_CHILD;
}

CButtonControl::~CButtonControl()
{
}

bool CButtonControl::IsTransparentBackground()
{
    return _isTransparentBackground;
}

bool CButtonControl::IsTransparentBorder()
{
    return _isTransparentBorder;
}

CTextFont *CButtonControl::GetFont()
{
    return &_font;
}

std::wstring CButtonControl::GetText()
{
    return _text;
}

void CButtonControl::SetTransparentBackground(bool isTransparentBackground)
{
    _isTransparentBackground = isTransparentBackground;

    UpdateWindow(_hWnd);
}

void CButtonControl::SetTransparentBorder(bool isTransparentBorder)
{
    _isTransparentBorder = isTransparentBorder;

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