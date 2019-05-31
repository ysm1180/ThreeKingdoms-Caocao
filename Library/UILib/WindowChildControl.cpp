#include "WindowChildControl.h"
#include "WindowControl.h"
#include "ToolbarControl.h"

namespace jojogame {
void CWindowChildControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN_CHILD(CWindowChildControl, "_ChlidControl", CBaseControl);

    LUA_METHOD(GetLuaY);
    LUA_METHOD(GetParentWindow);

    LUA_METHOD(SetLuaY);
}

int CWindowChildControl::GetLuaY() const
{
    auto window = dynamic_cast<CWindowControl*>(_parentControl);
    auto toolbar = window->GetToolbar();
    int toolbarHeight = 0;
    if (toolbar)
    {
        toolbarHeight = toolbar->GetHeight();
    }

    return _position.y - toolbarHeight;
}

CWindowControl* CWindowChildControl::GetParentWindow() const
{
    return dynamic_cast<CWindowControl*>(_parentControl);
}

void CWindowChildControl::SetLuaY(const int y)
{
    auto window = dynamic_cast<CWindowControl*>(_parentControl);
    auto toolbar = window->GetToolbar();
    int toolbarHeight = 0;
    if (toolbar)
    {
        toolbarHeight = toolbar->GetHeight();
    }

    if (_position.y != y + toolbarHeight)
    {
        _position.y = y + toolbarHeight;

        if (_hWnd != nullptr)
        {
            RECT rect;

            SetRect(&rect, GetX(), _position.y, GetX() + GetWidth(), _position.y + GetHeight());
            AdjustWindowRect(&rect, _style, FALSE);
            const int diffX = GetX() - rect.left;
            const int diffY = _position.y - rect.top;
            SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

            SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                         SWP_NOSIZE || SWP_NOZORDER);
        }
    }
}
}