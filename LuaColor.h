#pragma once

#include "base/Ref.h"

#include <windows.h>
#include <string>

namespace jojogame {
class __declspec(dllexport) LuaColor : public Ref
{
public:
    LuaColor();
    ~LuaColor();

    COLORREF rgb(int r, int g, int b);
    COLORREF name(const std::wstring colorName);

    static LuaColor* getInstance();
private:
    static LuaColor* s_sharedColor;
};
}
