#include "LuaForm.h"
#include "Application.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <windowsx.h>

namespace jojogame {

LRESULT CALLBACK procForm(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
        case WM_CREATE:
        {
            const auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            const auto lpParamCreate = createStruct->lpCreateParams;
            CWindow* form = reinterpret_cast<CWindow *>(lpParamCreate);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(form));

            return 0;
        }

        case WM_SETCURSOR:
        {
            break;
        }

        case WM_NOTIFY:
            break;

        case WM_LBUTTONUP:
        {
            break;
        }

        case WM_ACTIVATEAPP:
        {
            if (wParam == TRUE)
            {
            }
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
            // Form 의 close 함수 호출, 리턴값(close) = true 이면 종료 취소
            const auto form = reinterpret_cast<CWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            auto closeFunction = form->closeFunction();
            const auto notClose = lua_tinker::call<bool>(LuaJojo::getInstance()->getLuaState(), closeFunction.c_str());
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
            return 0;
        }

        case WM_ERASEBKGND:
        {
            // Form의 Background color 로 배경색을 설정한다.
            auto form = reinterpret_cast<CWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect((HDC)wParam, &rect, form->backBrush());
            return TRUE;
        }
    }

    return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

CWindow::CWindow(bool popup)
{
    if (!popup)
    {
        _style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    }
    else
    {
        _style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    }
}

CWindow::~CWindow()
{
    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }
}

bool CWindow::controlBox() const
{
    return _controlBox;
}

bool CWindow::maxButton() const
{
    return _maxButton;
}

bool CWindow::minButton() const
{
    return _minButton;
}

std::wstring CWindow::titleName() const
{
    return _titleName;
}

bool CWindow::titleBar() const
{
    return _titleBar;
}

std::wstring CWindow::activeFunction() const
{
    return _activeFunction;
}

std::wstring CWindow::closeFunction() const
{
    return _closeFunction;
}

HBRUSH CWindow::backBrush() const
{
    return _backBrush;
}

LuaMenubar* CWindow::menu()
{
    return _menu;
}

void CWindow::setControlBox(const bool controlBox)
{
    _controlBox = controlBox;
    if (_controlBox)
    {
        _style |= WS_SYSMENU;
    }
    else
    {
        _style &= ~WS_SYSMENU;
    }

    if (_hwnd != nullptr)
    {
        SetWindowLongPtr(_hwnd, GWL_STYLE, _style);
    }
}

void CWindow::setMaxButton(const bool maxButton)
{
    _maxButton = maxButton;
    if (_maxButton)
    {
        _style |= WS_MAXIMIZEBOX;
    }
    else
    {
        _style &= ~WS_MAXIMIZEBOX;
    }

    if (_hwnd != nullptr)
    {
        SetWindowLongPtr(_hwnd, GWL_STYLE, _style);
    }
}

void CWindow::setMinButton(const bool minButton)
{
    _minButton = minButton;
    if (_minButton)
    {
        _style |= WS_MINIMIZEBOX;
    }
    else
    {
        _style &= ~WS_MINIMIZEBOX;
    }

    if (_hwnd != nullptr)
    {
        SetWindowLongPtr(_hwnd, GWL_STYLE, _style);
    }
}

void CWindow::setTitleName(std::wstring title)
{
    _titleName = title;
    if (_hwnd != nullptr)
    {
        SetWindowTextW(_hwnd, _titleName.c_str());
    }
}

void CWindow::setTitleBar(const bool titleBar)
{
    _titleBar = titleBar;
    if (_titleBar)
    {
        _style |= WS_CAPTION;
    }
    else
    {
        _style &= ~WS_CAPTION;
    }
    if (_hwnd != nullptr)
    {
        SetWindowLongPtr(_hwnd, GWL_STYLE, _style);
    }
}

void CWindow::setActiveFunction(std::wstring activeFunction)
{
    _activeFunction = activeFunction;
}

void CWindow::setCloseFunction(std::wstring closeFunction)
{
    _closeFunction = closeFunction;
}

void CWindow::setIcon(std::wstring iconFilePath)
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
    SendMessage(_hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(_icon));
}

void CWindow::setBackColor(const COLORREF backColor)
{
    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }
    _backBrush = CreateSolidBrush(backColor);
    refresh();
}

void CWindow::create()
{
    RECT rect;

    SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
    AdjustWindowRect(&rect, _style, FALSE);
    _hwnd = CreateWindowW( L"jojo_form",
        _titleName.c_str(),
        _style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        _parentHwnd,
        nullptr,
        Application::getInstance()->getHInstance(),
        static_cast<LPVOID>(this));
}

void CWindow::createModal(CWindow& parentForm)
{
    RECT rect;

    _parentHwnd = parentForm._hwnd;
    SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
    AdjustWindowRect(&rect, _style, FALSE);
    _hwnd = CreateWindowW(L"jojo_form",
        _titleName.c_str(),
        _style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        _parentHwnd,
        nullptr,
        Application::getInstance()->getHInstance(),
        static_cast<LPVOID>(this));
}

void CWindow::close() const
{
    PostMessage(_hwnd, WM_CLOSE, 0, 0);
}

int CWindow::showModal()
{
    auto quit = false;
    auto dialogResult = 0;
    _dialogResult = &dialogResult;

    ShowWindow(_hwnd, TRUE);
    EnableWindow(_parentHwnd, FALSE);

    MSG msg;
    for (;;)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                quit = true;
                break;
            }

            if (!IsWindow(_hwnd))
            {
                break;
            }
        }
    }
    SetWindowPos(_parentHwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    EnableWindow(_parentHwnd, TRUE);

    if (quit)
    {
        PostQuitMessage(0);
    }

    return dialogResult;
}

void CWindow::setDialogResult(const int value) const
{
    *_dialogResult = value;
}

void CWindow::refresh() const
{
    RECT rect;

    GetClientRect(_hwnd, &rect);
    InvalidateRect(_hwnd, &rect, TRUE);
}

void CWindow::destroy()
{
    DestroyWindow(_hwnd);
    _hwnd = nullptr;
}

void CWindow::registerToLua()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<CWindow>(lua, "Form");

    lua_tinker::class_inh<CWindow, CControl>(lua);
    lua_tinker::class_con<CWindow>(lua, lua_tinker::constructor<CWindow, bool>);
    lua_tinker::class_def<CWindow>(lua, "setMaxButton", &CWindow::setMaxButton);
    lua_tinker::class_def<CWindow>(lua, "setMinButton", &CWindow::setMinButton);
    lua_tinker::class_def<CWindow>(lua, "setControlBox", &CWindow::setControlBox);
    lua_tinker::class_def<CWindow>(lua, "setTitleBar", &CWindow::setTitleBar);
    lua_tinker::class_def<CWindow>(lua, "setTitleName", &CWindow::setTitleName);
    lua_tinker::class_def<CWindow>(lua, "setActiveFunction", &CWindow::setActiveFunction);
    lua_tinker::class_def<CWindow>(lua, "setCloseFunction", &CWindow::setCloseFunction);
    lua_tinker::class_def<CWindow>(lua, "setBackColor", &CWindow::setBackColor);
    lua_tinker::class_def<CWindow>(lua, "setIcon", &CWindow::setIcon);
    lua_tinker::class_def<CWindow>(lua, "maxButton", &CWindow::maxButton);
    lua_tinker::class_def<CWindow>(lua, "minButton", &CWindow::minButton);
    lua_tinker::class_def<CWindow>(lua, "controlBox", &CWindow::controlBox);
    lua_tinker::class_def<CWindow>(lua, "titleBar", &CWindow::titleBar);
    lua_tinker::class_def<CWindow>(lua, "titleName", &CWindow::titleName);
    lua_tinker::class_def<CWindow>(lua, "activeFunction", &CWindow::activeFunction);
    lua_tinker::class_def<CWindow>(lua, "closeFunction", &CWindow::closeFunction);
    lua_tinker::class_def<CWindow>(lua, "create", &CWindow::create);
    lua_tinker::class_def<CWindow>(lua, "destroy", &CWindow::destroy);
    lua_tinker::class_def<CWindow>(lua, "createModal", &CWindow::createModal);
    lua_tinker::class_def<CWindow>(lua, "close", &CWindow::close);
    lua_tinker::class_def<CWindow>(lua, "showModal", &CWindow::showModal);
    lua_tinker::class_def<CWindow>(lua, "setDialogResult", &CWindow::setDialogResult);
    lua_tinker::class_def<CWindow>(lua, "refresh", &CWindow::refresh);

    // WNDCLASS 초기화
    WNDCLASS wndClass;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = static_cast<HBRUSH>(GetSysColorBrush(COLOR_3DFACE));
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hInstance = Application::getInstance()->getHInstance();
    wndClass.lpfnWndProc = static_cast<WNDPROC>(procForm);
    wndClass.lpszClassName = L"jojo_form";
    wndClass.lpszMenuName = nullptr;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndClass);
}
}
