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
            LuaForm* form = reinterpret_cast<LuaForm *>(lpParamCreate);
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
            const auto form = reinterpret_cast<LuaForm *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
            auto form = reinterpret_cast<LuaForm *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect((HDC)wParam, &rect, form->backBrush());
            return TRUE;
        }
    }

    return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

LuaForm::LuaForm(bool popup)
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

LuaForm::~LuaForm()
{
    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }
}

bool LuaForm::controlBox() const
{
    return _controlBox;
}

bool LuaForm::maxButton() const
{
    return _maxButton;
}

bool LuaForm::minButton() const
{
    return _minButton;
}

std::wstring LuaForm::titleName() const
{
    return _titleName;
}

bool LuaForm::titleBar() const
{
    return _titleBar;
}

std::wstring LuaForm::activeFunction() const
{
    return _activeFunction;
}

std::wstring LuaForm::closeFunction() const
{
    return _closeFunction;
}

HBRUSH LuaForm::backBrush() const
{
    return _backBrush;
}

LuaMenubar* LuaForm::menu()
{
    return _menu;
}

void LuaForm::setControlBox(const bool controlBox)
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

void LuaForm::setMaxButton(const bool maxButton)
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

void LuaForm::setMinButton(const bool minButton)
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

void LuaForm::setTitleName(std::wstring title)
{
    _titleName = title;
    if (_hwnd != nullptr)
    {
        SetWindowTextW(_hwnd, _titleName.c_str());
    }
}

void LuaForm::setTitleBar(const bool titleBar)
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

void LuaForm::setActiveFunction(std::wstring activeFunction)
{
    _activeFunction = activeFunction;
}

void LuaForm::setCloseFunction(std::wstring closeFunction)
{
    _closeFunction = closeFunction;
}

void LuaForm::setIcon(std::wstring iconFilePath)
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

void LuaForm::setBackColor(const COLORREF backColor)
{
    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }
    _backBrush = CreateSolidBrush(backColor);
    refresh();
}

void LuaForm::create()
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

void LuaForm::createModal(LuaForm& parentForm)
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

void LuaForm::close() const
{
    PostMessage(_hwnd, WM_CLOSE, 0, 0);
}

int LuaForm::showModal()
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

void LuaForm::setDialogResult(const int value) const
{
    *_dialogResult = value;
}

void LuaForm::refresh() const
{
    RECT rect;

    GetClientRect(_hwnd, &rect);
    InvalidateRect(_hwnd, &rect, TRUE);
}

void LuaForm::destroy()
{
    DestroyWindow(_hwnd);
    _hwnd = nullptr;
}

void LuaForm::registerToLua()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<LuaForm>(lua, "Form");

    lua_tinker::class_inh<LuaForm, LuaControl>(lua);
    lua_tinker::class_con<LuaForm>(lua, lua_tinker::constructor<LuaForm, bool>);
    lua_tinker::class_def<LuaForm>(lua, "setMaxButton", &LuaForm::setMaxButton);
    lua_tinker::class_def<LuaForm>(lua, "setMinButton", &LuaForm::setMinButton);
    lua_tinker::class_def<LuaForm>(lua, "setControlBox", &LuaForm::setControlBox);
    lua_tinker::class_def<LuaForm>(lua, "setTitleBar", &LuaForm::setTitleBar);
    lua_tinker::class_def<LuaForm>(lua, "setTitleName", &LuaForm::setTitleName);
    lua_tinker::class_def<LuaForm>(lua, "setActiveFunction", &LuaForm::setActiveFunction);
    lua_tinker::class_def<LuaForm>(lua, "setCloseFunction", &LuaForm::setCloseFunction);
    lua_tinker::class_def<LuaForm>(lua, "setBackColor", &LuaForm::setBackColor);
    lua_tinker::class_def<LuaForm>(lua, "setIcon", &LuaForm::setIcon);
    lua_tinker::class_def<LuaForm>(lua, "maxButton", &LuaForm::maxButton);
    lua_tinker::class_def<LuaForm>(lua, "minButton", &LuaForm::minButton);
    lua_tinker::class_def<LuaForm>(lua, "controlBox", &LuaForm::controlBox);
    lua_tinker::class_def<LuaForm>(lua, "titleBar", &LuaForm::titleBar);
    lua_tinker::class_def<LuaForm>(lua, "titleName", &LuaForm::titleName);
    lua_tinker::class_def<LuaForm>(lua, "activeFunction", &LuaForm::activeFunction);
    lua_tinker::class_def<LuaForm>(lua, "closeFunction", &LuaForm::closeFunction);
    lua_tinker::class_def<LuaForm>(lua, "create", &LuaForm::create);
    lua_tinker::class_def<LuaForm>(lua, "destroy", &LuaForm::destroy);
    lua_tinker::class_def<LuaForm>(lua, "createModal", &LuaForm::createModal);
    lua_tinker::class_def<LuaForm>(lua, "close", &LuaForm::close);
    lua_tinker::class_def<LuaForm>(lua, "showModal", &LuaForm::showModal);
    lua_tinker::class_def<LuaForm>(lua, "setDialogResult", &LuaForm::setDialogResult);
    lua_tinker::class_def<LuaForm>(lua, "refresh", &LuaForm::refresh);

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
