#include "WindowControl.h"

#include "MoviePlayerControl.h"
#include "ButtonControl.h"
#include "MenuControl.h"
#include "ToolbarControl.h"
#include "ControlManager.h"
#include "MenuManager.h"
#include "ToolbarManager.h"
#include "LayoutControl.h"

namespace jojogame {
LRESULT CALLBACK CWindowControl::OnControlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static bool isHover = false;
    TRACKMOUSEEVENT trackMouseEvent;

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
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent.c_str(), 
                                            window,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }
        break;
    }

    case WM_LBUTTONDOWN:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonDownEvent = window->GetMouseLButtonDownEvent();
        if (mouseLButtonDownEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonDownEvent.c_str(), 
                                            window,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }
        break;
    }

    case WM_MOUSEMOVE:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseMoveEvent = window->GetMouseMoveEvent();
        if (mouseMoveEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent.c_str(),
                                            window,
                                            (int)wParam,
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
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseHoverEvent = window->GetMouseHoverEvent();
        if (mouseHoverEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseHoverEvent.c_str(), window);
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
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLeaveEvent = window->GetMouseLeaveEvent();
        if (mouseLeaveEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent.c_str(), window);
        }

        isHover = false;
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
        auto id = LOWORD(wParam);
        auto menuItem = CMenuManager::GetInstance().GetMenuItem(id);
        if (menuItem)
        {
            auto clickEvent = menuItem->GetClickEvent();
            if (clickEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(clickEvent.c_str(), menuItem);
            }
        }

        auto toolbarButton = CToolbarManager::GetInstance().GetToolbarButton(id);
        if (toolbarButton)
        {
            auto clickEvent = toolbarButton->GetClickEvent();
            if (clickEvent.length())
            {
                CLuaTinker::GetLuaTinker().Call(clickEvent.c_str(), menuItem);
            }
        }
        break;
    }

    case WM_SIZE:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        window->SetWidth(LOWORD(lParam));
        window->SetHeight(HIWORD(lParam));

        auto sizeEvent = window->GetSizeEvent();
        if (sizeEvent.length())
        {
            CLuaTinker::GetLuaTinker().Call(sizeEvent.c_str(), window, LOWORD(lParam), HIWORD(lParam));
        }
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
        auto pnmhdr = (LPNMHDR)lParam;
        auto lpTooltip = (LPTOOLTIPTEXT)lParam;

        if (pnmhdr->code == TTN_NEEDTEXT)
        {
            auto index = lpTooltip->hdr.idFrom;
            auto button = CToolbarManager::GetInstance().GetToolbarButton(index);
            wcscpy_s(lpTooltip->szText, button->GetTooltipText().c_str());
        }

        break;
    }


    case WM_MEASUREITEM:
    {
        auto item = (MEASUREITEMSTRUCT *)lParam;
        if (item->CtlType == ODT_MENU)
        {
            auto menuItem = (CMenuItem *)item->itemData;
            auto hdc = GetDC(hWnd);
            SIZE size;

            auto originalFont = SelectFont(hdc, menuItem->GetFont()->GetHFont());
            GetTextExtentPoint32(hdc, menuItem->GetText().c_str(), menuItem->GetText().length(), &size);
            SelectFont(hdc, originalFont);

            item->itemWidth = size.cx;
            if (wcscmp(menuItem->GetText().c_str(), L"-") == 0)
            {
                item->itemHeight = 3;
            }
            else
            {
                item->itemHeight = size.cy + 10;
            }
            ReleaseDC(hWnd, hdc);
        }
        break;
    }

    case WM_MENUCHAR:
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
            }
            else if (button->IsHovered())
            {
                backgroundColor = button->GetBackgroundColor().focused;
                borderColor = button->GetBorderColor().focused;
                SetTextColor(item->hDC, button->GetTextColor().focused);
            }
            else
            {
                backgroundColor = button->GetBackgroundColor().normal;
                borderColor = button->GetBorderColor().normal;
                SetTextColor(item->hDC, button->GetTextColor().normal);
            }
            
            RECT rect;
            SetRect(&rect, item->rcItem.left, item->rcItem.top, item->rcItem.right, item->rcItem.bottom);
            int borderWidth = button->GetBorderWidth();
            if (borderWidth > 0)
            {
                HBRUSH borderBrush = CreateSolidBrush(borderColor);
                FillRect(item->hDC, &rect, borderBrush);
                DeleteBrush(borderBrush);
            }
            
            if (!button->IsTransparentBackground())
            {
                HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
                SetRect(&rect, rect.left + borderWidth, rect.top + borderWidth, rect.right - borderWidth,
                        rect.bottom - borderWidth);
                FillRect(item->hDC, &rect, backgroundBrush);
                DeleteBrush(backgroundBrush);
            }
            
            auto originalFont = SelectFont(item->hDC, button->GetFont()->GetHFont());
            SetBkMode(item->hDC, TRANSPARENT);
            DrawText(item->hDC, button->GetText().c_str(), -1, &rect,
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectFont(item->hDC, originalFont);
            SetBkMode(item->hDC, OPAQUE);
        }
        else if (item->CtlType == ODT_MENU)
        {
            auto menu = CMenuManager::GetInstance().GetMenuItem(item->itemID);
            RECT rect;
            SetRect(&rect, item->rcItem.left, item->rcItem.top, item->rcItem.right, item->rcItem.bottom);

            COLORREF backgroundColor;

            if (wcscmp(menu->GetText().c_str(), L"-") == 0)
            {
                backgroundColor = menu->GetBackgroundColor().normal;
                HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
                FillRect(item->hDC, &rect, backgroundBrush);

                auto pen = CreatePen(PS_SOLID, 1, menu->GetTextColor().normal);
                auto oldPen = (HPEN)SelectObject(item->hDC, pen);
                MoveToEx(item->hDC, rect.left, rect.top + 1, NULL);
                LineTo(item->hDC, rect.right, rect.top + 1);
                SelectObject(item->hDC, oldPen);
            }
            else
            {
                if (item->itemState & ODS_SELECTED)
                {
                    if (menu->IsEnabled())
                    {
                        backgroundColor = menu->GetBackgroundColor().focused;
                    }
                    else
                    {
                        backgroundColor = menu->GetBackgroundColor().disableFocused;
                    }
                }
                else
                {
                    if (menu->IsEnabled())
                    {
                        backgroundColor = menu->GetBackgroundColor().normal;
                    }
                    else
                    {
                        backgroundColor = menu->GetBackgroundColor().disabled;
                    }
                }
                HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
                FillRect(item->hDC, &rect, backgroundBrush);

                SetRect(&rect, item->rcItem.left + 4, item->rcItem.top, item->rcItem.right, item->rcItem.bottom);
                if (item->itemState & ODS_SELECTED)
                {
                    if (menu->IsEnabled())
                    {
                        SetTextColor(item->hDC, menu->GetTextColor().focused);
                    }
                    else
                    {
                        SetTextColor(item->hDC, menu->GetTextColor().disableFocused);
                    }
                }
                else
                {
                    if (menu->IsEnabled())
                    {
                        SetTextColor(item->hDC, menu->GetTextColor().normal);
                    }
                    else
                    {
                        SetTextColor(item->hDC, menu->GetTextColor().disabled);
                    }
                }

                auto originalFont = SelectFont(item->hDC, menu->GetFont()->GetHFont());
                SetBkMode(item->hDC, TRANSPARENT);
                DrawText(item->hDC, menu->GetText().c_str(), -1, &rect,
                         DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                SelectFont(item->hDC, originalFont);
                SetBkMode(item->hDC, OPAQUE);
            }
        }
        
        break;
    }

    case WM_CTLCOLORBTN:
    {
        auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
        RECT rect;

        GetClientRect(button->GetHWnd(), &rect);

        if (button->IsTransparentBackground())
        {
            GetClientRect(button->GetHWnd(), &rect);
            auto rgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
            SelectClipRgn((HDC)wParam, rgn);

            auto originalFont = SelectFont((HDC)wParam, button->GetFont()->GetHFont());
            SetBkMode((HDC)wParam, TRANSPARENT);
            DrawText((HDC)wParam, button->GetText().c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectFont((HDC)wParam, originalFont);
            SetBkMode((HDC)wParam, OPAQUE);

            int borderWidth = button->GetBorderWidth();
            ExcludeClipRect((HDC)wParam, rect.left + borderWidth, rect.top + borderWidth, rect.right - borderWidth,
                            rect.bottom - borderWidth);
        }

        break;
    }

    case WM_PAINT:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        bool isFitWidth = false;
        bool isFitHeight = false;

        for (auto layout : window->_layouts)
        {  
            isFitWidth = isFitHeight = false;
            
            if (layout->GetWidth() == 0)
            {
                isFitWidth = true;
                layout->SetWidth(window->GetWidth());
            }
            if (layout->GetHeight() == 0)
            {
                isFitHeight = true;
                layout->SetHeight(window->GetHeight());
            }
            
            layout->Draw(hdc);

            if (isFitWidth)
            {
                layout->SetWidth(0);
            }
            if (isFitHeight)
            {
                layout->SetHeight(0);
            }
        }

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_ERASEBKGND:
    {
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
    LUA_METHOD(GetSizeEvent);
    LUA_METHOD(GetMenu);

    LUA_METHOD(SetMaxButton);
    LUA_METHOD(SetMinButton);
    LUA_METHOD(SetControlBox);
    LUA_METHOD(SetTitlebar);
    LUA_METHOD(SetTitleName);
    LUA_METHOD(SetActiveEvent);
    LUA_METHOD(SetCloseEvent);
    LUA_METHOD(SetSizeEvent);
    LUA_METHOD(SetIcon);
    LUA_METHOD(SetBackColor);
    LUA_METHOD(SetDialogResult);
    LUA_METHOD(SetMenu);

    LUA_METHOD(AddLayout);
    LUA_METHOD(DeleteLayout);

    LUA_METHOD(Create);
    LUA_METHOD(ShowModalWindow);
    LUA_METHOD(Destroy);
    LUA_METHOD(Close);
    LUA_METHOD(Refresh);
    LUA_METHOD(RefreshRegion);

    // WNDCLASS 초기화
    WNDCLASS wndClass;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = static_cast<HBRUSH>(GetSysColorBrush(COLOR_3DFACE));
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hIcon = nullptr;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = static_cast<WNDPROC>(OnControlProc);
    wndClass.lpszClassName = L"jojo_form";
    wndClass.lpszMenuName = nullptr;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndClass);
}

CWindowControl::CWindowControl()
{
    _style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
    //_style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
}

CWindowControl::~CWindowControl()
{
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

std::wstring CWindowControl::GetSizeEvent() const
{
    return _sizeEvent;
}

CMenu *CWindowControl::GetMenu()
{
    return _menu;
}

void CWindowControl::SetY(const int y)
{
    _position.y = y;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, GetX(), _position.y, GetX() + GetWidth(), _position.y + GetHeight());
        if (_menu)
        {
            AdjustWindowRect(&rect, _style, TRUE);
        }
        else
        {
            AdjustWindowRect(&rect, _style, FALSE);
        }

        int diffX = GetX() - rect.left;
        int diffY = _position.y - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE || SWP_NOZORDER);
    }
}

void CWindowControl::SetX(const int x)
{
    _position.x = x;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, _position.x, GetY(), _position.x + GetWidth(), GetY() + GetHeight());
        if (_menu)
        {
            AdjustWindowRect(&rect, _style, TRUE);
        }
        else
        {
            AdjustWindowRect(&rect, _style, FALSE);
        }

        int diffX = _position.x - rect.left;
        int diffY = GetY() - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE || SWP_NOZORDER);
    }
}

void CWindowControl::SetWidth(const int width)
{
    _size.cx = width;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, GetX(), GetY(), GetX() + _size.cx, GetY() + GetHeight());
        if (_menu)
        {
            AdjustWindowRect(&rect, _style, TRUE);
        }
        else
        {
            AdjustWindowRect(&rect, _style, FALSE);
        }

        int diffX = GetX() - rect.left;
        int diffY = GetY() - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void CWindowControl::SetHeight(const int height)
{
    _size.cy = height;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, GetX(), GetY(), GetX() + GetWidth(), GetY() + _size.cy);
        if (_menu)
        {
            AdjustWindowRect(&rect, _style, TRUE);
        }
        else
        {
            AdjustWindowRect(&rect, _style, FALSE);
        }

        int diffX = GetX() - rect.left;
        int diffY = GetY() - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
    }
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

void CWindowControl::SetTitlebar(bool isTitlebar)
{
    _isTitlebar = isTitlebar;
    if (_isTitlebar)
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

void CWindowControl::SetSizeEvent(std::wstring sizeEvent)
{
    _sizeEvent = sizeEvent;
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

void CWindowControl::SetMenu(CMenu *menu)
{
    if (_menu)
    {
        _menu->SetParentWindow(nullptr);
    }

    _menu = menu;
    
    if (_menu)
    {
        ::SetMenu(_hWnd, _menu->GetHMenu());
        _menu->SetParentWindow(this);
    }
    else
    {
        ::SetMenu(_hWnd, nullptr);
    }
}

void CWindowControl::SetParentWindow(CWindowControl *parent)
{
    if (parent)
    {
        _parentControl = parent;
    }
    else
    {
        _parentControl = nullptr;
    }
}

void CWindowControl::AddLayout(CLayoutControl *layout)
{
    _layouts.push_back(layout);

    RECT rect;
    if (layout->GetWidth() == 0 && layout->GetHeight() == 0)
    {
        GetClientRect(_hWnd, &rect);
    }
    else
    {
        SetRect(&rect, layout->GetX(), layout->GetY(), layout->GetX() + layout->GetWidth(), layout->GetY() + layout->GetHeight());
    }
    InvalidateRect(_hWnd, &rect, TRUE);
    UpdateWindow(_hWnd);

    layout->AddParentWindow(this);
}

void CWindowControl::DeleteLayout(CLayoutControl * layout)
{
    auto iter = std::begin(_layouts);

    while (iter != std::end(_layouts))
    {
        if (*iter == layout)
        {
            RECT rect;
            auto layout = *iter;

            if (layout->GetWidth() == 0 && layout->GetHeight() == 0)
            {
                GetClientRect(_hWnd, &rect);
            }
            else
            {
                SetRect(&rect, layout->GetX(), layout->GetY(), layout->GetX() + layout->GetWidth(), layout->GetY() + layout->GetHeight());
            }

            _layouts.erase(iter);

            InvalidateRect(_hWnd, &rect, TRUE);
            UpdateWindow(_hWnd);

            break;
        }
        else
        {
            ++iter;
        }
    }
}

bool CWindowControl::Create()
{
    RECT rect;

    SetRect(&rect, GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight());
    if (_menu)
    {
        AdjustWindowRect(&rect, _style, TRUE);
    } 
    else
    {
        AdjustWindowRect(&rect, _style, FALSE);
    }

    int diffX = GetX() - rect.left;
    int diffY = GetY() - rect.top;
    SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

    HWND parentHWnd = nullptr;
    if (_parentControl)
    {
        parentHWnd = _parentControl->GetHWnd();
    }

    _hWnd = CreateWindowEx(WS_EX_DLGMODALFRAME, L"jojo_form",
                         _titleName.c_str(),
                         _style,
                         rect.left,
                         rect.top,
                         rect.right - rect.left,
                         rect.bottom - rect.top,
                         parentHWnd,
                         nullptr,
                         CControlManager::GetInstance().GetHInstance(),
                         (LPVOID)this);

    SendMessage(_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(_icon));

    if (_menu)
    {
        ::SetMenu(_hWnd, _menu->GetHMenu());
    }

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
    EnableWindow(_parentControl->GetHWnd(), FALSE);

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
    SetWindowPos(_parentControl->GetHWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    EnableWindow(_parentControl->GetHWnd(), TRUE);

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
    if (_hWnd)
    {
        RECT rect;

        GetClientRect(_hWnd, &rect);
        InvalidateRect(_hWnd, &rect, TRUE);
        UpdateWindow(_hWnd);
    }
}

void CWindowControl::RefreshRegion(int left, int top, int right, int bottom)
{
    if (_hWnd)
    {
        RECT rect;

        SetRect(&rect, left, top, right, bottom);
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

}