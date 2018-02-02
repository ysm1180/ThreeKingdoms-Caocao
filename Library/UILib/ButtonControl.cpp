#include "ButtonControl.h"

#include "ControlManager.h"

namespace jojogame {
WNDPROC CButtonControl::s_originalProc = NULL;

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
            CLuaTinker::GetLuaTinker().Call(createEvent.c_str(), button);

            break;
        }

        case WM_LBUTTONUP:
        {
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLButtonUpEvent = button->GetMouseLButtonUpEvent();

            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(), button);
            break;
        }

        case WM_MOUSEMOVE:
        {
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseMoveEvent = button->GetMouseMoveEvent();
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent.c_str(), 
                (int) wParam, 
                GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));

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
            CLuaTinker::GetLuaTinker().Call(mouseHoverEvent.c_str());

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
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent.c_str());

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
    LUA_BEGIN(CButtonControl, "_ButtonControl");

    WNDCLASS wndClass;
    GetClassInfo(NULL, TEXT("button"), &wndClass);
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC) CButtonControl::OnControlProc;
    wndClass.lpszClassName = L"jojo_button";
    s_originalProc = wndClass.lpfnWndProc;
    RegisterClass(&wndClass);
}

CButtonControl::CButtonControl()
        : _font(this)
{
    _style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_NOTIFY;
}

CButtonControl::~CButtonControl()
{
}

CTextFont& CButtonControl::GetFont()
{
    return _font;
}

std::wstring CButtonControl::GetText()
{
    return _text;
}

void CButtonControl::SetText(std::wstring text)
{
    _text = text;
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
                         nullptr,
                         CControlManager::GetInstance().GetHInstance(),
                         (LPVOID) this);

    return true;
}

void CButtonControl::Destroy()
{
    DestroyWindow(_hWnd);
    _hWnd = nullptr;
}

WNDPROC CButtonControl::GetOriginalProc()
{
    return s_originalProc;
}
}