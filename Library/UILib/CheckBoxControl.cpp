#include "CheckBoxControl.h"
#include "ControlManager.h"

namespace jojogame {
WNDPROC CCheckBoxControl::s_originalProc = nullptr;

LRESULT CCheckBoxControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    TRACKMOUSEEVENT trackMouseEvent;

    switch (msg)
    {
    case WM_CREATE:
    {
        auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void *lpParamCreate = createStruct->lpCreateParams;
        auto groupControl = reinterpret_cast<CCheckBoxControl *>(lpParamCreate);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(groupControl));

        auto createEvent = groupControl->GetCreateEvent();
        if (createEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(createEvent.c_str(), groupControl);
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        auto groupControl = reinterpret_cast<CCheckBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = groupControl->GetMouseLButtonUpEvent();
        if (mouseLButtonUpEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(),
                                            groupControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        break;
    }

    case WM_LBUTTONDOWN:
    {
        auto button = reinterpret_cast<CCheckBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = button->GetMouseLButtonDownEvent();

        if (mouseLButtonUpEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(),
                                            button,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        break;
    }

    case WM_MOUSEMOVE:
    {
        auto groupControl = reinterpret_cast<CCheckBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseMoveEvent = groupControl->GetMouseMoveEvent();
        if (mouseMoveEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent.c_str(),
                                            groupControl,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        if (!groupControl->_isHover)
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
        auto groupControl = reinterpret_cast<CCheckBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseHoverEvent = groupControl->GetMouseEnterEvent();
        if (mouseHoverEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseHoverEvent.c_str(), groupControl);
        }

        groupControl->_isHover = true;

        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_LEAVE;
        trackMouseEvent.hwndTrack = hWnd;
        trackMouseEvent.dwHoverTime = 100;
        TrackMouseEvent(&trackMouseEvent);
        break;
    }

    case WM_MOUSELEAVE:
    {
        auto groupControl = reinterpret_cast<CCheckBoxControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLeaveEvent = groupControl->GetMouseLeaveEvent();
        if (mouseLeaveEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent.c_str(), groupControl);
        }

        groupControl->_isHover = false;

        break;
    }

    case WM_DESTROY:
    {
        break;
    }
    }
    return CallWindowProc(CCheckBoxControl::GetOriginalProc(), hWnd, msg, wParam, lParam);
}

void CCheckBoxControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN_CHILD(CCheckBoxControl, "_CheckBoxControl", CBaseControl);

    LUA_METHOD(GetText);

    LUA_METHOD(SetText);

    LUA_METHOD(Create);
    LUA_METHOD(Destroy);

    WNDCLASS wndClass;
    GetClassInfo(NULL, TEXT("Button"), &wndClass);
    s_originalProc = wndClass.lpfnWndProc;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC)CCheckBoxControl::OnControlProc;
    wndClass.lpszClassName = L"jojo_check";
    RegisterClass(&wndClass);
}

CCheckBoxControl::CCheckBoxControl()
{
    _style = WS_CHILD | BS_CHECKBOX;
    _type = L"checkbox";
}

CCheckBoxControl::~CCheckBoxControl()
{
}

std::wstring CCheckBoxControl::GetText()
{
    return _text;
}

CTextFont* CCheckBoxControl::GetFont()
{
    return &_font;
}

void CCheckBoxControl::SetText(std::wstring text)
{
    if (_text != text)
    {
        _text = text;
    }
}

bool CCheckBoxControl::Create()
{
    if (_parentControl != nullptr)
    {
        _hWnd = CreateWindow(L"jojo_check",
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

        _font.ResetFont();
    }

    return _hWnd != nullptr;
}

void CCheckBoxControl::Destroy()
{
    if (_hWnd != nullptr)
    {
        DestroyWindow(_hWnd);
        _hWnd = nullptr;
    }
}

WNDPROC CCheckBoxControl::GetOriginalProc()
{
    return s_originalProc;
}
}
