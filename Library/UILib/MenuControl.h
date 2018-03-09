#pragma once

#include "TextFont.h"
#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <vector>

namespace jojogame {
class CWindowControl;
class CMenu;
class CTextFont;

struct MenuItemStateColor
{
    COLORREF normal;
    COLORREF focused;
    COLORREF disabled;
    COLORREF disableFocused;
};

class CMenuItem
{
public:
    static void RegisterFunctions(lua_State *L);

    CMenuItem();
    virtual ~CMenuItem();

    bool IsEnabled();
    int GetIndex();
    CMenu *GetChildMenu();
    std::wstring GetText();
    std::wstring GetClickEvent();
    CTextFont *GetFont();
    MenuItemStateColor GetBackgroundColor();
    MenuItemStateColor GetTextColor();

    COLORREF GetNormalBackgroundColor();
    COLORREF GetFocusedBackgroundColor();
    COLORREF GetDisabledBackgroundColor();
    COLORREF GetDisableFocusedBackgroundColor();
    COLORREF GetNormalTextColor();
    COLORREF GetFocusedTextColor();
    COLORREF GetDisabledTextColor();
    COLORREF GetDisableFocusedTextColor();

    void SetEnabled(bool isEnabled);
    void SetText(std::wstring text);
    void SetClickEvent(std::wstring clickEvent);
    void SetChildMenu(CMenu *childMenu);
    void SetParentMenu(CMenu *parentMenu);
    void SetIndex(int index);
    void SetNormalBackgroundColor(COLORREF color);
    void SetFocusedBackgroundColor(COLORREF color);
    void SetDisabledBackgroundColor(COLORREF color);
    void SetDisableFocusedBackgroundColor(COLORREF color);
    void SetNormalTextColor(COLORREF color);
    void SetFocusedTextColor(COLORREF color);
    void SetDisabledTextColor(COLORREF color);
    void SetDisableFocusedTextColor(COLORREF color);

private:
    CMenu *_parentMenu = nullptr;
    int _index = -1;

    CMenu *_childMenu = nullptr;
    std::wstring _text = L"";
    std::wstring _clickEvent = L"";

    bool _isEnabled = true;

    CTextFont _font;
    MenuItemStateColor _textColor{RGB(0, 0, 0), RGB(0, 0, 0), RGB(0x6D, 0x6D, 0x6D), RGB(0x6D, 0x6D, 0x6D)};
    MenuItemStateColor _backgroundColor{RGB(0xF2, 0xF2, 0xF2), RGB(0x91, 0xC9, 0xF7), RGB(0xF2, 0xF2, 0xF2), RGB(0xE6, 0xE6, 0xE6)};
};

class CMenu
{
public:
    static void RegisterFunctions(lua_State *L);

    CMenu();
    virtual ~CMenu();

    HMENU GetHMenu();
    CWindowControl *GetParentWindow();
    CMenuItem *GetMenuItemByPosition(int position);

    void SetParentWindow(CWindowControl *parent);

    void AddMenuItem(CMenuItem *menuItem);
    bool DeleteMenuitem(CMenuItem *menuItem);
    bool DeleteMeuItemByPosition(int position);

private:
    HMENU _menu = nullptr;
    CWindowControl *_parentWindow = nullptr;
    std::vector<CMenuItem *> _menuItems;
    int _count = 0;
};
}
