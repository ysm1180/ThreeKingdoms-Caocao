#include "WindowControl.h"
#include "MoviePlayerControl.h"
#include "ControlManager.h"

#include <Vfw.h>

namespace jojogame {
LRESULT CALLBACK CWindowControl::OnControlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_CREATE:
    {
        // Form 클래스의 포인터를 hwnd의 GWLP_USERDATA 에 저장
        // GetWindowLongPtr 을 통해 Form instance 를 불러와서 함수 호출 가능
        LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        auto lpParamCreate = createStruct->lpCreateParams;
        auto window = reinterpret_cast<CWindowControl *>(lpParamCreate);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

        return 0;
    }

    case WM_SETCURSOR:
    {
        break;
    }
    case MCIWNDM_NOTIFYPOS:
    {
        CMoviePlayerControl * playedMovie = reinterpret_cast<CMoviePlayerControl *>(GetWindowLongPtr((HWND)wParam, GWLP_USERDATA));
        if (MCIWndGetEnd((HWND)wParam) == lParam)
        {
            playedMovie->Stop();
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        break;
    }

    case WM_ACTIVATEAPP:
    {
        if (wParam == TRUE)
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto activeFunction = window->GetActiveEvent();
            //CLuaTinker::GetLuaTinker().Call<void>(activeFunction.c_str());
        }
        return 0;
    }

    case WM_COMMAND:
    {
        break;
    }

    case WM_SIZE:
    {
        break;
    }

    case WM_MOVE:
    {
        break;
    }

    case WM_CLOSE:
    {
        // close 함수 호출, 리턴값(close) = true 이면 종료 취소
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto closeFunction = window->GetCloseEvent();
        const auto notClose = CLuaTinker::GetLuaTinker().Call<bool>(closeFunction.c_str());
        if (notClose)
        {
            return 0;
        }
        break;
    }

    case WM_DESTROY:
    {
        return 0;
    }

    case WM_PAINT:
    {
        break;
    }

    case WM_ERASEBKGND:
    {
        // Background color 로 배경색을 설정한다.
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect((HDC)wParam, &rect, window->_backBrush);
        return TRUE;
    }
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void CWindowControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN_CHILD(CWindowControl, "_Window", CBaseControl);

    LUA_METHOD(IsMaxButton);
    LUA_METHOD(IsMinButton);
    LUA_METHOD(IsControlBox);
    LUA_METHOD(IsTitleBar);
    LUA_METHOD(GetTitleName);
    LUA_METHOD(GetActiveEvent);
    LUA_METHOD(GetCloseEvent);

    LUA_METHOD(SetMaxButton);
    LUA_METHOD(SetMinButton);
    LUA_METHOD(SetControlBox);
    LUA_METHOD(SetTitleBar);
    LUA_METHOD(SetTitleName);
    LUA_METHOD(SetActiveEvent);
    LUA_METHOD(SetCloseEvent);
    LUA_METHOD(SetIcon);
    LUA_METHOD(SetBackColor);
    LUA_METHOD(SetDialogResult);
    LUA_METHOD(SetMenu);

    LUA_METHOD(Create);
    LUA_METHOD(ShowModalWindow);
    LUA_METHOD(Destroy);
    LUA_METHOD(Close);
    LUA_METHOD(Refresh);

    // WNDCLASS 초기화
    WNDCLASS wndClass;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = static_cast<HBRUSH>(GetSysColorBrush(COLOR_3DFACE));
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = static_cast<WNDPROC>(OnControlProc);
    wndClass.lpszClassName = L"jojo_form";
    wndClass.lpszMenuName = nullptr;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndClass);
}

CWindowControl::CWindowControl()
{
    _style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    // _style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
}

CWindowControl::~CWindowControl()
{
    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }
}

bool CWindowControl::IsControlBox() const
{
    return _isControlBox;
}

bool CWindowControl::IsMaxButton() const
{
    return _isMaxButton;
}

bool CWindowControl::IsMinButton() const
{
    return _isMinButton;
}

std::wstring CWindowControl::GetTitleName() const
{
    return _titleName;
}

bool CWindowControl::IsTitleBar() const
{
    return _isTitleBar;
}

std::wstring CWindowControl::GetActiveEvent() const
{
    return _activeEvent;
}

std::wstring CWindowControl::GetCloseEvent() const
{
    return _closeEvent;
}

HBRUSH CWindowControl::GetBackBrush() const
{
    return _backBrush;
}

CMenubar* CWindowControl::GetMenu()
{
    return _menu;
}

void CWindowControl::SetControlBox(const bool isControlBox)
{
    _isControlBox = isControlBox;
    if (_isControlBox)
    {
        _style |= WS_SYSMENU;
    }
    else
    {
        _style &= ~WS_SYSMENU;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CWindowControl::SetMaxButton(const bool isMaxButton)
{
    _isMaxButton = isMaxButton;
    if (_isMaxButton)
    {
        _style |= WS_MAXIMIZEBOX;
    }
    else
    {
        _style &= ~WS_MAXIMIZEBOX;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CWindowControl::SetMinButton(const bool isMinButton)
{
    _isMinButton = isMinButton;
    if (_isMinButton)
    {
        _style |= WS_MINIMIZEBOX;
    }
    else
    {
        _style &= ~WS_MINIMIZEBOX;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CWindowControl::SetTitleName(std::wstring title)
{
    _titleName = title;
    if (_hWnd != nullptr)
    {
        SetWindowTextW(_hWnd, _titleName.c_str());
    }
}

void CWindowControl::SetTitleBar(const bool isTitleBar)
{
    _isTitleBar = isTitleBar;
    if (_isTitleBar)
    {
        _style |= WS_CAPTION;
    }
    else
    {
        _style &= ~WS_CAPTION;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CWindowControl::SetActiveEvent(std::wstring activeEvent)
{
    _activeEvent = activeEvent;
}

void CWindowControl::SetCloseEvent(std::wstring closeEvent)
{
    _closeEvent = closeEvent;
}

void CWindowControl::SetIcon(std::wstring iconFilePath)
{
    if (!iconFilePath.empty())
    {
        _icon = static_cast<HICON>(LoadImageW(nullptr,
            iconFilePath.c_str(),
            IMAGE_ICON,
            0,
            0,
            LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_SHARED));
    }
    else if (iconFilePath == L"DEFAULT")
    {
        _icon = LoadIcon(nullptr, IDI_APPLICATION);
    }
    else
    {
        _icon = nullptr;
    }
    SendMessage(_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(_icon));
}

void CWindowControl::SetBackColor(const COLORREF backColor)
{
    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }
    _backBrush = CreateSolidBrush(backColor);
    Refresh();
}

void CWindowControl::SetMenu(CMenubar * menu)
{
}

void CWindowControl::SetParentWindow(CWindowControl * parent)
{
    if (parent)
    {
        _parentHWnd = parent->_hWnd;
    }
    else
    {
        _parentHWnd = nullptr;
    }
}

bool CWindowControl::Create()
{
    RECT rect;

    SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
    AdjustWindowRect(&rect, _style, FALSE);
    _hWnd = CreateWindowW(L"jojo_form",
        _titleName.c_str(),
        _style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        _parentHWnd,
        nullptr,
        CControlManager::GetInstance().GetHInstance(),
        (LPVOID)this);

    return true;
}

void CWindowControl::Close() const
{
    PostMessage(_hWnd, WM_CLOSE, 0, 0);
}

int CWindowControl::ShowModalWindow()
{
    auto isQuit = false;
    auto dialogResult = 0;
    _dialogResult = &dialogResult;

    ShowWindow(_hWnd, TRUE);
    EnableWindow(_parentHWnd, FALSE);

    MSG msg;
    for (;;)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                isQuit = true;
                break;
            }

            if (!IsWindow(_hWnd))
            {
                break;
            }
        }
    }
    SetWindowPos(_parentHWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    EnableWindow(_parentHWnd, TRUE);

    if (isQuit)
    {
        PostQuitMessage(0);
    }

    return dialogResult;
}

void CWindowControl::SetDialogResult(const int value) const
{
    *_dialogResult = value;
}

void CWindowControl::Refresh() const
{
    RECT rect;

    GetClientRect(_hWnd, &rect);
    InvalidateRect(_hWnd, &rect, TRUE);
}

void CWindowControl::Destroy()
{
    DestroyWindow(_hWnd);
    _hWnd = nullptr;
}
}