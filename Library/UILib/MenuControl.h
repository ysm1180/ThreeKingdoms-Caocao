#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>

#include <vector>

namespace jojogame {
class CMenu;

class  CMenuItem
{
public:
    static void RegisterFunctions(lua_State *L);

    CMenuItem();
    virtual ~CMenuItem();

    bool IsEnabled();
    HMENU GetChildMenu();
    std::wstring GetText();
    std::wstring GetClickEvent();
    int GetPosition();

    void SetEnabled(bool isEnabled);
    void SetText(std::wstring text);
    void SetClickEvent(std::wstring clickEvent);
    void SetChildMenu(CMenu *childMenu);
    void SetParentMenu(CMenu *parentMenu);
    void SetPosition(int position);

private:
    CMenu * _parentMenu = nullptr;
    int _position = -1;

    HMENU _childMenu = nullptr;
    std::wstring _text = L"";
    std::wstring _clickEvent = L"";

    bool _isEnabled = true;
};

class CMenu
{
public:
    static void RegisterFunctions(lua_State *L);

    CMenu();
    virtual ~CMenu();

    HMENU GetHMenu();

    void AddMenuItem(CMenuItem *menuItem);
    void DeleteMenuitem(CMenuItem *menuItem);
    void DeleteMeuItemByPosition(int position);
private:
    HMENU _menu = nullptr;
    std::vector<CMenuItem *> _menuItems;
    int _count = 0;
};
}
