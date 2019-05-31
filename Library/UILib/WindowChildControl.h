#pragma once
#include "BaseControl.h"

namespace jojogame
{
class CWindowControl;

class CWindowChildControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    int GetLuaY() const;
    CWindowControl *GetParentWindow() const;

    void SetLuaY(int y);
};
} // namespace jojogame
