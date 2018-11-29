#include "WindowControl.h"

#include "MoviePlayerControl.h"
#include "ButtonControl.h"
#include "MenuControl.h"
#include "ToolbarControl.h"
#include "ControlManager.h"
#include "MenuManager.h"
#include "ToolbarManager.h"
#include "LayoutControl.h"
#include "ListviewControl.h"
#include "StaticControl.h"
#include "GroupBoxControl.h"
#include "CheckBoxControl.h"
#include "RadioButtonControl.h"

#include <Uxtheme.h>
#include <Vsstyle.h>

namespace jojogame {
LRESULT CALLBACK CWindowControl::OnControlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
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

        auto createEvent = window->GetCreateEvent();
        if (createEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(createEvent, window);
        }

        return 0;
    }

    case WM_SETCURSOR:
    {
        break;
    }

    case WM_KEYDOWN:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto keyDownEvent = window->GetKeyDownEvent();
        if (keyDownEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(keyDownEvent,
                                            window,
                                            (int)wParam);
        }
        break;
    }

    case WM_KEYUP:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto keyUpEvent = window->GetKeyUpEvent();
        if (keyUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(keyUpEvent,
                                            window,
                                            (int)wParam);
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLButtonUpEvent = window->GetMouseLButtonUpEvent();
        if (mouseLButtonUpEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
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
        if (mouseLButtonDownEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLButtonDownEvent,
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
        if (mouseMoveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseMoveEvent,
                                            window,
                                            (int)wParam,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam));
        }

        if (!window->_isHover)
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
        auto mouseHoverEvent = window->GetMouseEnterEvent();
        if (mouseHoverEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseHoverEvent, window);
        }

        window->_isHover = true;
        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_LEAVE;
        trackMouseEvent.hwndTrack = hWnd;
        TrackMouseEvent(&trackMouseEvent);
        break;
    }

    case WM_MOUSELEAVE:
    {
        auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto mouseLeaveEvent = window->GetMouseLeaveEvent();
        if (mouseLeaveEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, window);
        }

        window->_isHover = false;
        break;
    }

    case WM_ACTIVATEAPP:
    {
        if (wParam == TRUE)
        {
            auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            auto activeEvent = window->GetActiveEvent();
            if (activeEvent != LUA_NOREF)
            {
                CLuaTinker::GetLuaTinker().Call(activeEvent, window);
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
            if (clickEvent != LUA_NOREF)
            {
                CLuaTinker::GetLuaTinker().Call(clickEvent, menuItem);
            }
        }

        auto toolbarButton = CToolbarManager::GetInstance().GetToolbarButton(id);
        if (toolbarButton)
        {
            auto clickEvent = toolbarButton->GetClickEvent();
            if (clickEvent != LUA_NOREF)
            {
                CLuaTinker::GetLuaTinker().Call(clickEvent, menuItem);
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
        if (sizeEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(sizeEvent, window, LOWORD(lParam), HIWORD(lParam));
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
        if (closeEvent != LUA_NOREF)
        {
            // notClose = true 이면 종료 취소
            const auto notClose = CLuaTinker::GetLuaTinker().Call<bool>(closeEvent, window);
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
        if (destroyEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call<void>(destroyEvent, window);
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
        else if (pnmhdr->code == LVN_ITEMACTIVATE)
        {
            auto listView = reinterpret_cast<CListViewControl *>(GetWindowLongPtr(pnmhdr->hwndFrom, GWLP_USERDATA));

            if (listView)
            {
                auto lpnmia = (LPNMITEMACTIVATE)lParam;
                auto row = listView->GetRow(lpnmia->iItem + 1);
                if (row->IsEnabled())
                {
                    auto itemActiveEvent = row->GetActiveEvent();
                    if (itemActiveEvent != LUA_NOREF)
                    {
                        CLuaTinker::GetLuaTinker().Call<void>(itemActiveEvent,
                                                              listView,
                                                              static_cast<int>(lpnmia->iItem),
                                                              static_cast<int>(lpnmia->uKeyFlags));
                    }
                }
            }
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
        else if (item->CtlType == ODT_LISTVIEW)
        {
            CListViewControl* listView = (CListViewControl *)item->CtlID;
            item->itemHeight = listView->GetRowHeight();
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
            if (button->IsPressed())
            {
                backgroundColor = button->GetBackgroundColor().pushed;
                borderColor = button->GetBorderColor().pushed;
                SetTextColor(item->hDC, button->GetTextColor().pushed);
            }
            else if (button->IsHover())
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
                MoveToEx(item->hDC, rect.left, rect.top + 1, nullptr);
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
        else if (item->CtlType == ODT_LISTVIEW)
        {
            auto listview = (CListViewControl *)item->CtlID;
            auto row = (CListViewRow *)item->itemData;

            int left = item->rcItem.left;
            for (int i = 0; i < listview->GetColumnCount(); ++i)
            {
                bool isSelected = false;
                auto listViewItem = row->GetItem(i + 1);
                COLORREF backgroundColor;

                if (row->IsEnabled())
                {
                    if (item->itemState & ODS_SELECTED)
                    {
                        backgroundColor = listViewItem->GetFocusedBackgroundColor();
                        SetTextColor(item->hDC, listViewItem->GetFocusedTextColor());
                        isSelected = true;
                    }
                    else
                    {
                        backgroundColor = listViewItem->GetNormalBackgroundColor();
                        SetTextColor(item->hDC, listViewItem->GetNormalTextColor());
                    }
                }
                else
                {
                    if (item->itemState & ODS_SELECTED)
                    {
                        backgroundColor = listViewItem->GetDisableFocusedBackgroundColor();
                        SetTextColor(item->hDC, listViewItem->GetDisableFocusedTextColor());
                        isSelected = true;
                    }
                    else
                    {
                        backgroundColor = listViewItem->GetDisabledBackgroundColor();
                        SetTextColor(item->hDC, listViewItem->GetDisabledTextColor());
                    }
                }

                RECT rect;
                auto columnWidth = ListView_GetColumnWidth(item->hwndItem, i);
                SetRect(&rect, left, item->rcItem.top, left + columnWidth, item->rcItem.bottom);
                

                if (listview->IsTransparentBackground())
                {
                    auto window = reinterpret_cast<CWindowControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

                    if (isSelected)
                    {
                        HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
                        FillRect(item->hDC, &rect, backgroundBrush);
                        DeleteBrush(backgroundBrush);
                    }
                    else
                    {
                        FillRect(item->hDC, &rect, window->GetBackgroundBrush());
                    }

                    RECT rectByWindow;
                    SetRect(&rectByWindow, rect.left, rect.top, rect.right, rect.bottom);
                    ClientToScreen(listview->GetHWnd(), reinterpret_cast<POINT*>(&rectByWindow.left));  // convert top-left
                    ClientToScreen(listview->GetHWnd(), reinterpret_cast<POINT*>(&rectByWindow.right)); // convert bottom-right
                    ScreenToClient(hWnd, reinterpret_cast<POINT*>(&rectByWindow.left));
                    ScreenToClient(hWnd, reinterpret_cast<POINT*>(&rectByWindow.right));
                    rectByWindow.left -= rect.left;
                    rectByWindow.top -= rect.top;

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

                        layout->SetX(layout->GetX() - rectByWindow.left);
                        layout->SetY(layout->GetY() - rectByWindow.top);

                        if (isSelected)
                        {
                            layout->Draw(item->hDC, rect, backgroundColor);
                        }
                        else
                        {
                            layout->Draw(item->hDC, rect);
                        }

                        layout->SetX(layout->GetX() + rectByWindow.left);
                        layout->SetY(layout->GetY() + rectByWindow.top);

                        if (isFitWidth)
                        {
                            layout->SetWidth(0);
                        }
                        if (isFitHeight)
                        {
                            layout->SetHeight(0);
                        }
                    }
                }
                else
                {
                    HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
                    FillRect(item->hDC, &rect, backgroundBrush);
                    DeleteBrush(backgroundBrush);
                }

                auto originalFont = SelectFont(item->hDC, listViewItem->GetFont()->GetHFont());
                SetBkMode(item->hDC, TRANSPARENT);
                if (listViewItem->GetAlign() == 0)
                {
                    DrawText(item->hDC, listViewItem->GetText().c_str(), -1, &rect,
                             DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
                else if (listViewItem->GetAlign() == 1)
                {
                    DrawText(item->hDC, listViewItem->GetText().c_str(), -1, &rect,
                             DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
                }
                else if (listViewItem->GetAlign() == 2)
                {
                    DrawText(item->hDC, listViewItem->GetText().c_str(), -1, &rect,
                             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                SelectFont(item->hDC, originalFont);
                SetBkMode(item->hDC, OPAQUE);

                left += columnWidth;
            }
        }
        else if (item->CtlType == ODT_STATIC)
        {
            auto staticControl = (CStaticControl *)item->CtlID;

            RECT rect;
            SetRect(&rect, item->rcItem.left, item->rcItem.top, item->rcItem.right, item->rcItem.bottom);

            if (!staticControl->IsTransparentBackground())
            {
                FillRect(item->hDC, &rect, staticControl->GetBackgroundBrush());
            }

            auto originalFont = SelectFont(item->hDC, staticControl->GetFont()->GetHFont());
            SetBkMode(item->hDC, TRANSPARENT);
            if (staticControl->GetAlign() == 0)
            {
                DrawText(item->hDC, staticControl->GetText().c_str(), -1, &rect, DT_LEFT | DT_WORD_ELLIPSIS);
            }
            else if (staticControl->GetAlign() == 1)
            {
                DrawText(item->hDC, staticControl->GetText().c_str(), -1, &rect, DT_RIGHT | DT_WORD_ELLIPSIS);
            }
            else if (staticControl->GetAlign() == 2)
            {
                DrawText(item->hDC, staticControl->GetText().c_str(), -1, &rect, DT_CENTER | DT_WORD_ELLIPSIS);
            }
            SetBkMode(item->hDC, OPAQUE);
            SelectFont(item->hDC, originalFont);
        }

        break;
    }

    case WM_CTLCOLORSTATIC:
    {
        auto control = reinterpret_cast<CBaseControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
        if (control->GetType() == L"static")
        {
            auto staticControl = reinterpret_cast<CStaticControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));

            ::SetTextColor((HDC)wParam, staticControl->GetTextColor());
            return (LRESULT)::GetSysColorBrush(COLOR_WINDOW);
        }
        else if (control->GetType() == L"groupbox")
        {
            auto groupBox = reinterpret_cast<CGroupBoxControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));

            RECT rect;
            SIZE size;

            auto originalFont = SelectFont((HDC)wParam, groupBox->GetFont()->GetHFont());
            GetTextExtentPoint32((HDC)wParam, groupBox->GetText().c_str(), groupBox->GetText().length(), &size);

            GetClientRect(groupBox->GetHWnd(), &rect);
            SetRect(&rect, rect.left + 4, rect.top, rect.left + size.cx + 4, rect.top + size.cy);
            SetBkMode((HDC)wParam, TRANSPARENT);
            DrawText((HDC)wParam, groupBox->GetText().c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectFont((HDC)wParam, originalFont);
            SetBkMode((HDC)wParam, OPAQUE);

            ExcludeClipRect((HDC)wParam, rect.left, rect.top, rect.right, rect.bottom + 2);
        }
        else if (control->GetType() == L"checkbox")
        {
            auto checkBox = reinterpret_cast<CCheckBoxControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));

            RECT rect;
            GetClientRect(checkBox->GetHWnd(), &rect);

            int checkBoxWidth = GetSystemMetrics(SM_CXMENUCHECK) - GetSystemMetrics(SM_CXEDGE);
            int checkBoxHeight = GetSystemMetrics(SM_CYMENUCHECK) - GetSystemMetrics(SM_CYEDGE);
            int height = rect.bottom - rect.top;

            if (checkBoxHeight > height)
            {
                checkBoxHeight = height;
            }

            RECT textRect;
            SetRect(&textRect, rect.left + checkBoxWidth + 4, rect.top + (height - 1) / 2 - checkBoxHeight / 2,
                    rect.right, rect.bottom - (height / 2 - checkBoxHeight / 2) + 1);
            auto originalFont = SelectFont((HDC)wParam, checkBox->GetFont()->GetHFont());
            SetBkMode((HDC)wParam, TRANSPARENT);
            DrawText((HDC)wParam, checkBox->GetText().c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SelectFont((HDC)wParam, originalFont);
            SetBkMode((HDC)wParam, OPAQUE);

            RECT checkBoxRect;
            SetRect(&checkBoxRect, rect.left, rect.top + height / 2 - checkBoxHeight / 2, rect.left + checkBoxWidth,
                    rect.bottom - (height / 2 - checkBoxHeight / 2));
            auto theme = checkBox->GetTheme();
            if (checkBox->IsChecked())
            {
                if (checkBox->IsPressed())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_CHECKEDPRESSED, &checkBoxRect, nullptr);
                }
                else if (checkBox->IsHover())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_CHECKEDHOT, &checkBoxRect, nullptr);
                }
                else
                {
                    if (checkBox->IsEnabled())
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_CHECKEDNORMAL, &checkBoxRect, nullptr);
                    }
                    else
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_CHECKEDDISABLED, &checkBoxRect,
                                            nullptr);
                    }
                }
            }
            else
            {
                if (checkBox->IsPressed())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_UNCHECKEDPRESSED, &checkBoxRect, nullptr);
                }
                else if (checkBox->IsHover())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_UNCHECKEDHOT, &checkBoxRect, nullptr);
                }
                else
                {
                    if (checkBox->IsEnabled())
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_UNCHECKEDNORMAL, &checkBoxRect,
                                            nullptr);
                    }
                    else
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_CHECKBOX, CBS_UNCHECKEDDISABLED, &checkBoxRect,
                                            nullptr);
                    }
                }
            }

            GetClientRect(checkBox->GetHWnd(), &rect);
            ExcludeClipRect((HDC)wParam, rect.left, rect.top, rect.right, rect.bottom);
        }
        else if (control->GetType() == L"radiobutton")
        {
            auto radioButton = reinterpret_cast<CRadioButtonControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));

            RECT rect;
            GetClientRect(radioButton->GetHWnd(), &rect);

            int radioWidth = GetSystemMetrics(SM_CXMENUCHECK) - GetSystemMetrics(SM_CXEDGE);
            int radioHeight = GetSystemMetrics(SM_CYMENUCHECK) - GetSystemMetrics(SM_CYEDGE);
            int height = rect.bottom - rect.top;

            if (radioHeight > height)
            {
                radioHeight = height;
            }

            RECT textRect;
            SetRect(&textRect, rect.left + radioWidth + 4, rect.top + (height - 1) / 2 - radioHeight / 2, rect.right,
                    rect.bottom - (height / 2 - radioHeight / 2) + 1);
            auto originalFont = SelectFont((HDC)wParam, radioButton->GetFont()->GetHFont());
            SetBkMode((HDC)wParam, TRANSPARENT);
            DrawText((HDC)wParam, radioButton->GetText().c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SelectFont((HDC)wParam, originalFont);
            SetBkMode((HDC)wParam, OPAQUE);

            RECT radioButtonRect;
            SetRect(&radioButtonRect, rect.left, rect.top + height / 2 - radioHeight / 2, rect.left + radioWidth,
                    rect.bottom - (height / 2 - radioHeight / 2));
            auto theme = radioButton->GetTheme();
            if (radioButton->IsChecked())
            {
                if (radioButton->IsPressed())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_CHECKEDPRESSED, &radioButtonRect,
                                        nullptr);
                }
                else if (radioButton->IsHover())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_CHECKEDHOT, &radioButtonRect, nullptr);
                }
                else
                {
                    if (radioButton->IsEnabled())
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_CHECKEDNORMAL, &radioButtonRect,
                                            nullptr);
                    }
                    else
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_CHECKEDDISABLED, &radioButtonRect,
                                            nullptr);
                    }
                }
            }
            else
            {
                if (radioButton->IsPressed())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_UNCHECKEDPRESSED, &radioButtonRect,
                                        nullptr);
                }
                else if (radioButton->IsHover())
                {
                    DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_UNCHECKEDHOT, &radioButtonRect,
                                        nullptr);
                }
                else
                {
                    if (radioButton->IsEnabled())
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_UNCHECKEDNORMAL, &radioButtonRect,
                                            nullptr);
                    }
                    else
                    {
                        DrawThemeBackground(theme, (HDC)wParam, BP_RADIOBUTTON, RBS_UNCHECKEDDISABLED, &radioButtonRect,
                                            nullptr);
                    }
                }
            }

            GetClientRect(radioButton->GetHWnd(), &rect);
            ExcludeClipRect((HDC)wParam, rect.left, rect.top, rect.right, rect.bottom);
        }


        break;
    }

    case WM_CTLCOLORBTN:
    {
        auto button = reinterpret_cast<CButtonControl *>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
        if (button->GetType() == L"button")
        {
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

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP bitmap = CreateCompatibleBitmap(hdc, window->GetWidth(), window->GetHeight());
        HBITMAP oldBitmap = SelectBitmap(memDC, bitmap);
        RECT rect;
        SetRect(&rect, 0, 0, window->GetWidth(), window->GetHeight());
        FillRect(memDC, &rect, window->GetBackgroundBrush());

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

            layout->Draw(memDC, ps.rcPaint);

            if (isFitWidth)
            {
                layout->SetWidth(0);
            }
            if (isFitHeight)
            {
                layout->SetHeight(0);
            }
        }
        BitBlt(hdc, 0, 0, window->GetWidth(), window->GetHeight(), memDC, 0, 0, SRCCOPY);

        SelectBitmap(memDC, oldBitmap);
        DeleteBitmap(bitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_ERASEBKGND:
    {
        return 0;
    }
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void CWindowControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN_CHILD(CWindowControl, "_Window", CBaseControl);

    LUA_METHOD(IsMaxButton);
    LUA_METHOD(IsMinButton);
    LUA_METHOD(IsControlBox);
    LUA_METHOD(IsTitleBar);
    LUA_METHOD(IsSizable);
    LUA_METHOD(GetTitleName);
    LUA_METHOD(GetActiveEvent);
    LUA_METHOD(GetCloseEvent);
    LUA_METHOD(GetSizeEvent);
    LUA_METHOD(GetKeyDownEvent);
    LUA_METHOD(GetKeyUpEvent);
    LUA_METHOD(GetMenu);
    LUA_METHOD(GetBackgroundColor);

    LUA_METHOD(SetParentWindow);
    LUA_METHOD(SetMaxButton);
    LUA_METHOD(SetMinButton);
    LUA_METHOD(SetControlBox);
    LUA_METHOD(SetTitlebar);
    LUA_METHOD(SetSizable);
    LUA_METHOD(SetTitleName);
    LUA_METHOD(SetActiveEvent);
    LUA_METHOD(SetCloseEvent);
    LUA_METHOD(SetSizeEvent);
    LUA_METHOD(SetKeyDownEvent);
    LUA_METHOD(SetKeyUpEvent);
    LUA_METHOD(SetIcon);
    LUA_METHOD(SetBackgroundColor);
    LUA_METHOD(SetDialogResult);
    LUA_METHOD(SetMenu);

    LUA_METHOD(AddLayout);
    LUA_METHOD(DeleteLayout);
    LUA_METHOD(Clear);

    LUA_METHOD(Create);
    LUA_METHOD(ShowModalWindow);
    LUA_METHOD(Destroy);
    LUA_METHOD(Close);

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
    _type = L"window";
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

bool CWindowControl::IsSizable() const
{
    return _isSizable;
}

int CWindowControl::GetX() const
{
    if (_hWnd != nullptr)
    {
        RECT rect;
        GetWindowRect(_hWnd, &rect);
        return rect.left;
    }
    return _position.x;
}

int CWindowControl::GetY() const
{
    if (_hWnd != nullptr)
    {
        RECT rect;
        GetWindowRect(_hWnd, &rect);
        return rect.top;
    }
    return _position.y;
}

int CWindowControl::GetActiveEvent() const
{
    return _activeEvent;
}

int CWindowControl::GetCloseEvent() const
{
    return _closeEvent;
}

int CWindowControl::GetSizeEvent() const
{
    return _sizeEvent;
}

int CWindowControl::GetKeyDownEvent() const
{
    return _keyDownEvent;
}

int CWindowControl::GetKeyUpEvent() const
{
    return _keyUpEvent;
}

COLORREF CWindowControl::GetBackgroundColor()
{
    return _backgroundColor;
}

HBRUSH CWindowControl::GetBackgroundBrush()
{
    return _backBrush;
}

CMenu* CWindowControl::GetMenu()
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

        int diffX = 0;
        int diffY = 0;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOSIZE || SWP_NOZORDER);
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

        int diffX = 0;
        int diffY = 0;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOSIZE || SWP_NOZORDER);
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

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOMOVE | SWP_NOZORDER);
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

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOMOVE | SWP_NOZORDER);
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

void CWindowControl::SetSizable(bool isSizable)
{
    _isSizable = isSizable;
    if (_isSizable)
    {
        _style |= WS_THICKFRAME;
    }
    else
    {
        _style &= ~WS_THICKFRAME;
    }

    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWL_STYLE, _style);
    }
}

void CWindowControl::SetActiveEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _activeEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
}

void CWindowControl::SetCloseEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _closeEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
}

void CWindowControl::SetSizeEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _sizeEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
}

void CWindowControl::SetKeyDownEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _keyDownEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
}

void CWindowControl::SetKeyUpEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _keyUpEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
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

void CWindowControl::SetBackgroundColor(const COLORREF backColor)
{
    if (_backgroundColor != backColor)
    {
        _backgroundColor = backColor;

        if (_backBrush != nullptr)
        {
            DeleteBrush(_backBrush);
        }
        _backBrush = CreateSolidBrush(_backgroundColor);
        Refresh();
    }
}

void CWindowControl::SetMenu(CMenu* menu)
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

void CWindowControl::SetParentWindow(CWindowControl* parent)
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

void CWindowControl::AddLayout(CLayoutControl* layout, bool isShow)
{
    layout->SetHide(!isShow);
    _layouts.push_back(layout);

    RECT rect;
    if (layout->GetWidth() == 0 && layout->GetHeight() == 0)
    {
        GetClientRect(_hWnd, &rect);
    }
    else
    {
        SetRect(&rect, layout->GetX(), layout->GetY(), layout->GetX() + layout->GetWidth(),
                layout->GetY() + layout->GetHeight());
    }
    InvalidateRect(_hWnd, &rect, TRUE);
    UpdateWindow(_hWnd);

    layout->AddParentWindow(this);
}

void CWindowControl::DeleteLayout(CLayoutControl* layout)
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
                SetRect(&rect, layout->GetX(), layout->GetY(), layout->GetX() + layout->GetWidth(),
                        layout->GetY() + layout->GetHeight());
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
    if (_parentControl != nullptr)
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


    if (_hWnd != nullptr)
    {
        SendMessage(_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(_icon));
    }

    if (_menu)
    {
        ::SetMenu(_hWnd, _menu->GetHMenu());
    }

    return _hWnd != nullptr;
}

void CWindowControl::Close() const
{
    PostMessage(_hWnd, WM_CLOSE, 0, 0);
}

void CWindowControl::Clear()
{
    RECT rect;
    GetClientRect(_hWnd, &rect);

    for (auto layout : this->_layouts)
    {
        layout->RemoveParentWIndow(this);
    }
    this->_layouts.clear();

    InvalidateRect(_hWnd, &rect, TRUE);
    UpdateWindow(_hWnd);
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

}
