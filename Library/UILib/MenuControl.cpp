#include "MenuControl.h"

#include "MenuManager.h"
#include "BaseLib/ConsoleOutput.h"
#include "WindowControl.h"

namespace three_kingdoms
{
void CMenuItem::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMenuItem, "_MenuItem");

    LUA_METHOD(IsEnabled);
    LUA_METHOD(GetText);
    LUA_METHOD(GetFont);
    LUA_METHOD(GetClickEvent);
    LUA_METHOD(GetChildMenu);

    LUA_METHOD(GetNormalBackgroundColor);
    LUA_METHOD(GetFocusedBackgroundColor);
    LUA_METHOD(GetDisabledBackgroundColor);
    LUA_METHOD(GetDisableFocusedBackgroundColor);
    LUA_METHOD(GetNormalTextColor);
    LUA_METHOD(GetFocusedTextColor);
    LUA_METHOD(GetDisabledTextColor);
    LUA_METHOD(GetDisableFocusedTextColor);

    LUA_METHOD(SetEnabled);
    LUA_METHOD(SetText);
    LUA_METHOD(SetClickEvent);
    LUA_METHOD(SetChildMenu);
    LUA_METHOD(SetNormalBackgroundColor);
    LUA_METHOD(SetFocusedBackgroundColor);
    LUA_METHOD(SetDisabledBackgroundColor);
    LUA_METHOD(SetDisableFocusedBackgroundColor);
    LUA_METHOD(SetNormalTextColor);
    LUA_METHOD(SetFocusedTextColor);
    LUA_METHOD(SetDisabledTextColor);
    LUA_METHOD(SetDisableFocusedTextColor);
}

CMenuItem::CMenuItem()
    : _font()
{
    _childMenu = nullptr;
}

CMenuItem::~CMenuItem()
{
    if (_clickEvent != LUA_NOREF)
    {
        luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _clickEvent);
    }
}

bool CMenuItem::IsEnabled() const
{
    return _isEnabled;
}

int CMenuItem::GetIndex() const
{
    return _index;
}

CMenu *CMenuItem::GetChildMenu() const
{
    return _childMenu;
}

std::wstring CMenuItem::GetText() const
{
    return _text;
}

int CMenuItem::GetClickEvent() const
{
    return _clickEvent;
}

CTextFont *CMenuItem::GetFont()
{
    return &_font;
}

MenuItemStateColor CMenuItem::GetBackgroundColor() const
{
    return _backgroundColor;
}

MenuItemStateColor CMenuItem::GetTextColor() const
{
    return _textColor;
}

void CMenuItem::SetEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;

    if (_parentMenu)
    {
        MENUITEMINFO itemInfo{};
        itemInfo.cbSize = sizeof(MENUITEMINFO);
        itemInfo.fMask = MIIM_STATE;
        if (_isEnabled)
        {
            itemInfo.fState = MFS_ENABLED;
        }
        else
        {
            itemInfo.fState = MFS_DISABLED;
        }
        ::SetMenuItemInfo(_parentMenu->GetHMenu(), static_cast<UINT>(_index), FALSE, &itemInfo);

        if (_parentMenu->GetParentWindow())
        {
            DrawMenuBar(_parentMenu->GetParentWindow()->GetHWnd());
        }
    }
}

void CMenuItem::SetText(std::wstring text)
{
    _text = text;

    if (_parentMenu)
    {
        MENUITEMINFO itemInfo{};
        itemInfo.cbSize = sizeof(MENUITEMINFO);
        itemInfo.fMask = MIIM_STRING;
        itemInfo.dwTypeData = const_cast<LPWSTR>(_text.c_str());
        ::SetMenuItemInfo(_parentMenu->GetHMenu(), static_cast<UINT>(_index), TRUE, &itemInfo);

        if (_parentMenu->GetParentWindow())
        {
            DrawMenuBar(_parentMenu->GetParentWindow()->GetHWnd());
        }
    }
}

void CMenuItem::SetClickEvent()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    if (lua_isfunction(l, -1))
    {
        lua_pushvalue(l, -1);
        _clickEvent = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    lua_pop(l, 1);
}

void CMenuItem::SetChildMenu(CMenu *childMenu)
{
    _childMenu = childMenu;
}

void CMenuItem::SetParentMenu(CMenu *parentMenu)
{
    _parentMenu = parentMenu;
}

void CMenuItem::SetIndex(int index)
{
    _index = index;
}

void CMenuItem::SetNormalBackgroundColor(COLORREF color)
{
    _backgroundColor.normal = color;
}

void CMenuItem::SetFocusedBackgroundColor(COLORREF color)
{
    _backgroundColor.focused = color;
}

void CMenuItem::SetDisabledBackgroundColor(COLORREF color)
{
    _backgroundColor.disabled = color;
}

void CMenuItem::SetDisableFocusedBackgroundColor(COLORREF color)
{
    _backgroundColor.disableFocused = color;
}

void CMenuItem::SetNormalTextColor(COLORREF color)
{
    _textColor.normal = color;
}

void CMenuItem::SetFocusedTextColor(COLORREF color)
{
    _textColor.focused = color;
}

void CMenuItem::SetDisabledTextColor(COLORREF color)
{
    _textColor.disabled = color;
}

void CMenuItem::SetDisableFocusedTextColor(COLORREF color)
{
    _textColor.disableFocused = color;
}

COLORREF CMenuItem::GetNormalBackgroundColor()
{
    return _backgroundColor.normal;
}

COLORREF CMenuItem::GetFocusedBackgroundColor()
{
    return _backgroundColor.focused;
}

COLORREF CMenuItem::GetDisabledBackgroundColor()
{
    return _backgroundColor.disabled;
}

COLORREF CMenuItem::GetDisableFocusedBackgroundColor()
{
    return _backgroundColor.disableFocused;
}

COLORREF CMenuItem::GetNormalTextColor()
{
    return _textColor.normal;
}

COLORREF CMenuItem::GetFocusedTextColor()
{
    return _textColor.focused;
}

COLORREF CMenuItem::GetDisabledTextColor()
{
    return _textColor.disabled;
}

COLORREF CMenuItem::GetDisableFocusedTextColor()
{
    return _textColor.disableFocused;
}

/****************************************
                CMenu
*****************************************/

void CMenu::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMenu, "_Menu");

    LUA_METHOD(GetMenuItemByPosition);
    LUA_METHOD(AddMenuItem);
    LUA_METHOD(DeleteMeuItemByPosition);
}

CMenu::CMenu()
{
    _menu = CreateMenu();
}

CMenu::~CMenu()
{
    if (_menu)
    {
        DestroyMenu(_menu);
    }
}

HMENU CMenu::GetHMenu()
{
    return _menu;
}

CWindowControl *CMenu::GetParentWindow()
{
    return _parentWindow;
}

CMenuItem *CMenu::GetMenuItemByPosition(int position)
{
    if (position > _menuItems.size() || position < 1)
    {
        return nullptr;
    }
    else
    {
        return _menuItems[position - 1];
    }
}

void CMenu::SetParentWindow(CWindowControl *parent)
{
    _parentWindow = parent;
}

void CMenu::AddMenuItem(CMenuItem *menuItem)
{
    auto iter = std::find(_menuItems.begin(), _menuItems.end(), menuItem);
    if (iter != _menuItems.end())
    {
        CConsoleOutput::OutputConsoles(L"Already added menu item");
    }
    else
    {
        if (menuItem->GetChildMenu())
        {
            int index = CMenuManager::GetInstance().AddMenuItemByHandle(menuItem, menuItem->GetChildMenu()->GetHMenu());
            AppendMenu(_menu, MF_STRING | MF_POPUP, (UINT)menuItem->GetChildMenu()->GetHMenu(),
                       menuItem->GetText().c_str());
        }
        else
        {
            int index = CMenuManager::GetInstance().AddMenuItem(menuItem);
            if (menuItem->IsEnabled() && wcscmp(menuItem->GetText().c_str(), L"-") != 0)
            {
                AppendMenu(_menu, MF_OWNERDRAW, index, (LPTSTR)menuItem);
            }
            else
            {
                AppendMenu(_menu, MF_OWNERDRAW | MF_DISABLED, index, (LPTSTR)menuItem);
            }
        }
        menuItem->SetParentMenu(this);
        menuItem->SetIndex(_count++);
        _menuItems.push_back(menuItem);
    }
}

bool CMenu::DeleteMenuitem(CMenuItem *menuItem)
{
    auto result = ::DeleteMenu(_menu, menuItem->GetIndex(), MF_BYCOMMAND);
    return result != 0;
}

bool CMenu::DeleteMeuItemByPosition(int position)
{
    auto result = ::DeleteMenu(_menu, position, MF_BYPOSITION);
    return result != 0;
}

} // namespace jojogame
