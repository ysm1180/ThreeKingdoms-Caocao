#include "MenuControl.h"

#include "CommonLib/MenuManager.h"
#include "BaseLib/ConsoleOutput.h"

#include <algorithm>

namespace jojogame {
void CMenuItem::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMenuItem, "_MenuItem");

    LUA_METHOD(IsEnabled);
    LUA_METHOD(GetText);

    LUA_METHOD(SetEnabled);
    LUA_METHOD(SetText);
    LUA_METHOD(SetClickEvent);
    LUA_METHOD(SetChildMenu);
}

CMenuItem::CMenuItem()
{
    _childMenu = nullptr;
}

CMenuItem::~CMenuItem()
{
}

bool CMenuItem::IsEnabled()
{
    return _isEnabled;
}

HMENU CMenuItem::GetChildMenu()
{
    return _childMenu;
}

std::wstring CMenuItem::GetText()
{
    return _text;
}

std::wstring CMenuItem::GetClickEvent()
{
    return _clickEvent;
}

int CMenuItem::GetPosition()
{
    return _position;
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
        ::SetMenuItemInfo(_parentMenu->GetHMenu(), static_cast<UINT>(_position), TRUE, &itemInfo);
           
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
        ::SetMenuItemInfo(_parentMenu->GetHMenu(), static_cast<UINT>(_position), TRUE, &itemInfo);
    }
}

void CMenuItem::SetClickEvent(std::wstring clickEvent)
{
    _clickEvent = clickEvent;
}

void CMenuItem::SetChildMenu(CMenu *childMenu)
{
    _childMenu = childMenu->GetHMenu();
}

void CMenuItem::SetParentMenu(CMenu *parentMenu)
{
    _parentMenu = parentMenu;
}

void CMenuItem::SetPosition(int position)
{
    _position = position;
}

/****************************************
                CMenu
*****************************************/

void CMenu::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMenu, "_Menu");

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
            int index = CMenuManager::GetInstance().AddMenuItemByHandle(menuItem, menuItem->GetChildMenu());
            AppendMenu(_menu, MF_STRING | MF_POPUP, (UINT)menuItem->GetChildMenu(), menuItem->GetText().c_str());
        }
        else
        {
            if (wcscmp(menuItem->GetText().c_str(), L"-") == 0)
            {
                AppendMenu(_menu, MF_SEPARATOR, 0, 0);
            } 
            else
            {
                int index = CMenuManager::GetInstance().AddMenuItem(menuItem);
                AppendMenu(_menu, MF_STRING, index, menuItem->GetText().c_str());
            }
        }
        menuItem->SetParentMenu(this);
        menuItem->SetPosition(_count++);
        _menuItems.push_back(menuItem);
    }
}

void CMenu::DeleteMeuItemByPosition(int position)
{
    auto result = ::DeleteMenu(_menu, position, MF_BYPOSITION);
    if (result == 0)
    {
        CConsoleOutput::OutputConsoles(L"Fail to delete menu");
    }
}

}