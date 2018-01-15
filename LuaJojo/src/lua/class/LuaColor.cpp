#include "LuaColor.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

namespace jojogame {
LuaColor* LuaColor::s_sharedColor = nullptr;

LuaColor* LuaColor::getInstance()
{
    if (s_sharedColor == nullptr)
    {
        s_sharedColor = new(std::nothrow) LuaColor();
    }
    return s_sharedColor;
}

LuaColor::LuaColor()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<LuaColor>(lua, "__color__");
    lua_tinker::class_def<LuaColor>(lua, "rgb", &LuaColor::rgb);
    lua_tinker::class_def<LuaColor>(lua, "name", &LuaColor::name);
}

LuaColor::~LuaColor()
{
    s_sharedColor = nullptr;
}

COLORREF LuaColor::rgb(int r, int g, int b)
{
    return RGB(r, g, b);
}

COLORREF LuaColor::name(const std::wstring colorName)
{
    COLORREF c = RGB(0, 0, 0);
    if (colorName == L"black")
    {
        c = RGB(0, 0, 0);
    }
    else if (colorName == L"red")
    {
        c = RGB(0xff, 0, 0);
    }
    else if (colorName == L"white")
    {
        c = RGB(0xff, 0xff, 0xff);
    }

    return c;
}
}
