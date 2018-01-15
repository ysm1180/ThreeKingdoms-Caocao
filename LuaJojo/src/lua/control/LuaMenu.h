#pragma once

#include "base/Ref.h"

#include <windows.h>
#include <string>

namespace jojogame {

class __declspec(dllexport) LuaMenuItem
{
public:
    LuaMenuItem();
    virtual ~LuaMenuItem();

    HMENU item();
    std::wstring text();
    std::wstring clickFunction();

    void setText(std::wstring text);
    void setClickFunction(std::wstring clickFunction);

    void setChildMenuItem(LuaMenuItem *child);

    static void registerToLua();
private:
    HMENU _item = nullptr;
    int _index = 0;
    std::wstring _text = L"";
    std::wstring _clickFunction = L"";
};

class __declspec(dllexport) LuaMenubar
{
public:
    LuaMenubar();
    virtual ~LuaMenubar();

    void addMenuItem(LuaMenuItem* menuItem);

    static void registerToLua();
private:
    HMENU _menu = nullptr;
};

}
