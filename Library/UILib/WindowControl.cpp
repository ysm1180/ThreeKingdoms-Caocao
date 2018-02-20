#include "WindowControl.h"

#include "MoviePlayerControl.h"
#include "ButtonControl.h"
#include "ControlManager.h"

#include "BaseLib\ConsoleOutput.h"

#include <WinUser.h>

namespace jojogame {
LRESULT CALLBACK CWindowControl::OnControlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
        case WM_CREATE:
        {
            // WindowControl 클래스의 포인터를 hwnd의 GWLP_USERDATA 에 저장
            // GetWindowLongPtr 을 통해 WindowControl instance 를 불러와서 함수 호출 가능
            LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            auto lpParamCreate = createStruct->lpCreateParams;
            auto window = reinterpret_cast<CWindowControl *>(lpParamCreate);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

            auto createFunction = window->GetCreateEvent();
            if (createFunction.length())
            {
                CLuaTinker::GetLuaTinker().Call(createFunction.c_str(), window);
            }

            return 0;
        }

        case WM_SETCURSOR:
        {
            break;
        }

        case WM_LBUTTONUP:
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLButtonUpEvent = window->GetMouseLButtonUpEvent();
            if (mouseLButtonUpEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(), window);
            }
            break;
        }

        case WM_LBUTTONDOWN:
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto mouseLButtonDownEvent = window->GetMouseLButtonDownEvent();
            if (mouseLButtonDownEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(mouseLButtonDownEvent.c_str(), window);
            }
            break;
        }

        case WM_ACTIVATEAPP:
        {
            if (wParam == TRUE)
            {
                auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
                auto activeEvent = window->GetActiveEvent();
                if (activeEvent.length())
                {
                    CLuaTinker::GetLuaTinker().Call(activeEvent.c_str(), window);
                }
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
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto closeEvent = window->GetCloseEvent();
            if (closeEvent.length())
            {
                // notClose = true 이면 종료 취소
                const auto notClose = CLuaTinker::GetLuaTinker().Call<bool>(closeEvent.c_str(), window);
                if (notClose)
                {
                    return 0;
                }
            }
            break;
        }

        case WM_DESTROY:
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto destroyEvent = window->GetDestroyEvent();
            if (destroyEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call<void>(destroyEvent.c_str(), window);
            }

            return 0;
        }

        case WM_NOTIFY:
        {

            break;
        }

        case WM_DRAWITEM:
        {
            auto item = (DRAWITEMSTRUCT *)lParam;
            if (item->CtlType == ODT_BUTTON)
            {
                auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr(item->hwndItem, GWLP_USERDATA));

                COLORREF backgroundColor, borderColor;
                if (button->IsPushed())
                {
                    backgroundColor = button->GetBackgroundColor().pushed;
                    borderColor = button->GetBorderColor().pushed;
                    SetTextColor(item->hDC, button->GetTextColor().pushed);
                } else if (button->IsHovered())
                {
                    backgroundColor = button->GetBackgroundColor().focused;
                    borderColor = button->GetBorderColor().focused;
                    SetTextColor(item->hDC, button->GetTextColor().focused);
                } else {
                    backgroundColor = button->GetBackgroundColor().normal;
                    borderColor = button->GetBorderColor().normal;
                    SetTextColor(item->hDC, button->GetTextColor().normal);
                }

                RECT rect;
                SetRect(&rect, item->rcItem.left, item->rcItem.top, item->rcItem.right, item->rcItem.bottom);
                int borderWidth = button->GetBorderWidth();
                HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
                HBRUSH borderBrush = CreateSolidBrush(borderColor);
                FillRect(item->hDC, &rect, borderBrush);
                SetRect(&rect, rect.left + borderWidth, rect.top + borderWidth, rect.right - borderWidth, rect.bottom - borderWidth);
                FillRect(item->hDC, &rect, backgroundBrush);

                auto originalFont = SelectFont(item->hDC, button->GetFont()->GetHFont());
                SetBkMode(item->hDC, TRANSPARENT);
                DrawText(item->hDC, button->GetText().c_str(), -1, &rect,
                         DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                SelectFont(item->hDC, originalFont);
                SetBkMode(item->hDC, OPAQUE);
            }
            break;
        }

        case WM_CTLCOLORBTN:
        {
            auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr((HWND) lParam, GWLP_USERDATA));
            RECT rect;

            GetClientRect(button->GetHWnd(), &rect);

            if (button->IsTransparentBackground())
            {
                GetClientRect(button->GetHWnd(), &rect);
                auto rgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
                SelectClipRgn((HDC) wParam, rgn);

                auto originalFont = SelectFont((HDC) wParam, button->GetFont()->GetHFont());
                SetBkMode((HDC) wParam, TRANSPARENT);
                DrawText((HDC) wParam, button->GetText().c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                SelectFont((HDC) wParam, originalFont);
                SetBkMode((HDC) wParam, OPAQUE);

                int borderWidth = button->GetBorderWidth();
                ExcludeClipRect((HDC) wParam, rect.left + borderWidth, rect.top + borderWidth, rect.right - borderWidth, rect.bottom - borderWidth);
            }

            break;
        }

        case WM_PAINT:
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            for (auto image : window->_images)
            {
                BitBlt(hdc, image->x, image->y, image->width, image->height, image->memDC, 0, 0, SRCCOPY);
            }

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_ERASEBKGND:
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            RECT rect;
            GetClientRect(hWnd, &rect);
            FillRect((HDC) wParam, &rect, window->_backBrush);
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
    LUA_METHOD(SetTitlebar);
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
    //_style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    _style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
}

CWindowControl::~CWindowControl()
{
    if (_images.size() > 0)
    {
        for (auto image : _images)
        {
            HBITMAP newBitmap = (HBITMAP) SelectObject(image->memDC, image->oldBitmap);
            DeleteBitmap(newBitmap);
            DeleteDC(image->memDC);
            delete image;
        }
    }

    if (_backBrush != nullptr)
    {
        DeleteBrush(_backBrush);
    }

    if (_hWnd != nullptr)
    {
        DestroyWindow(_hWnd);
        _hWnd = nullptr;
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
    return _isTitlebar;
}

std::wstring CWindowControl::GetActiveEvent() const
{
    return _activeEvent;
}

std::wstring CWindowControl::GetCloseEvent() const
{
    return _closeEvent;
}

CMenubar *CWindowControl::GetMenu()
{
    return _menu;
}

void CWindowControl::SetControlBox(const bool isControlBox)
{
    _isControlBox = isControlBox;
    if (_isControlBox)
    {
        _style |= WS_SYSMENU;
    } else
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
    } else
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
    } else
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

void CWindowControl::SetTitlebar(bool isTitlebar)
{
    _isTitlebar = isTitlebar;
    if (_isTitlebar)
    {
        _style |= WS_CAPTION;
    } else
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
        _icon = static_cast<HICON>(LoadImage(nullptr,
                                             iconFilePath.c_str(),
                                             IMAGE_ICON,
                                             0,
                                             0,
                                             LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_SHARED));
    } else if (iconFilePath == L"DEFAULT")
    {
        _icon = LoadIcon(nullptr, IDI_APPLICATION);
    } else
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

void CWindowControl::SetMenu(CMenubar *menu)
{
}

void CWindowControl::SetParentWindow(CWindowControl *parent)
{
    if (parent)
    {
        _parentHWnd = parent->_hWnd;
    } else
    {
        _parentHWnd = nullptr;
    }
}

bool CWindowControl::Create()
{
    RECT rect;

    SetRect(&rect, GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight());
    AdjustWindowRect(&rect, _style, FALSE);
    int diffX = GetX() - rect.left;
    int diffY = GetY() - rect.top;
    SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

    _hWnd = CreateWindow(L"jojo_form",
                         _titleName.c_str(),
                         _style,
                         rect.left,
                         rect.top,
                         rect.right - rect.left,
                         rect.bottom - rect.top,
                         _parentHWnd,
                         nullptr,
                         CControlManager::GetInstance().GetHInstance(),
                         (LPVOID) this);

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

    if (_hWnd)
    {
        GetClientRect(_hWnd, &rect);
        InvalidateRect(_hWnd, &rect, TRUE);
        UpdateWindow(_hWnd);
    }
}

void CWindowControl::Destroy()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = nullptr;
    }
}

int CWindowControl::SetDrawingImage(unsigned int index, HDC srcDC, BITMAPINFO bitmapInfo, RECT& rect)
{
    int x = rect.left;
    int y = rect.top;
    int width = rect.right - x;
    int height = rect.bottom - y;
    DrawingImageInfo *image = new DrawingImageInfo();
    HDC newDC = CreateCompatibleDC(srcDC);
    HBITMAP newBitmap = CreateDIBSection(srcDC, &bitmapInfo, DIB_RGB_COLORS, (void **) &image->bits, 0, 0);

    image->oldBitmap = (HBITMAP) SelectObject(newDC, newBitmap);
    BitBlt(newDC, 0, 0, width, height, srcDC, 0, 0, SRCCOPY);

    image->memDC = newDC;
    image->bitmapInfo = bitmapInfo;
    image->x = x;
    image->y = y;
    image->width = width;
    image->height = height;

    if (index + 1 > _images.size())
    {
        _images.push_back(image);

        return _images.size() - 1;
    } else
    {
        DrawingImageInfo *unusedData = _images[index];
        HBITMAP newBitmap = (HBITMAP) SelectObject(unusedData->memDC, unusedData->oldBitmap);
        DeleteBitmap(newBitmap);
        DeleteDC(_images[index]->memDC);
        delete _images[index];
        _images[index] = image;

        return index;
    }

}
}