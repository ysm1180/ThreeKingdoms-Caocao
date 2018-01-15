#pragma once

#include "base/Ref.h"

namespace jojogame {
class __declspec(dllexport) LuaWindow : public Ref
{
public:
    LuaWindow();
    ~LuaWindow();

    int getDesktopWidth();
    int getDesktopHeight();

    static LuaWindow* getInstance();
private:

    static LuaWindow* s_sharedWindow;
};
}
