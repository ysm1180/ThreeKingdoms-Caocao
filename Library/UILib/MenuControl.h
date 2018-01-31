#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>

namespace jojogame {
class  CMenuItem
{
public:
    static void RegisterFunctions(lua_State *L);

    CMenuItem();
    virtual ~CMenuItem();

    HMENU item();
    std::wstring text();
    std::wstring clickFunction();

    void setText(std::wstring text);
    void setClickFunction(std::wstring clickFunction);

    void setChildMenuItem(CMenuItem *child);
private:
    HMENU _item = nullptr;
    int _index = 0;
    std::wstring _text = L"";
    std::wstring _mouseLButtonUpEvent = L"";
};

class CMenubar
{
public:
    static void RegisterFunctions(lua_State *L);

    CMenubar();
    virtual ~CMenubar();

    void AddMenuItem(CMenuItem* menuItem);
private:
    HMENU _menu = nullptr;
};
}
