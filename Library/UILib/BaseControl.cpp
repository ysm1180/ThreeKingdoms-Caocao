#include "BaseControl.h"
#include "ControlManager.h"
#include "LuaLib\LuaTinker.h"

namespace jojogame {
void CBaseControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CBaseControl, "_BaseControl");

    LUA_METHOD(IsEnabled);
    LUA_METHOD(IsVisible);
    LUA_METHOD(GetWidth);
    LUA_METHOD(GetHeight);
    LUA_METHOD(GetX);
    LUA_METHOD(GetY);
    LUA_METHOD(GetStyle);

    LUA_METHOD(SetEnabled);
    LUA_METHOD(SetVisible);
    LUA_METHOD(SetWidth);
    LUA_METHOD(SetHeight);
    LUA_METHOD(SetX);
    LUA_METHOD(SetY);
    LUA_METHOD(SetCreateEvent);
    LUA_METHOD(SetDestroyEvent);
    LUA_METHOD(SetClickEvent);
    LUA_METHOD(SetStyle);

    LUA_METHOD(Show);
    LUA_METHOD(Hide);
}

CBaseControl::CBaseControl()
{
}

CBaseControl::~CBaseControl()
{
}

inline bool CBaseControl::IsEnabled() const
{
    return _isEnabled;
}

inline bool CBaseControl::IsVisible() const
{
    return _isVisible;
}

inline int CBaseControl::GetWidth() const
{
    return _size.cx;
}

inline int CBaseControl::GetHeight() const
{
    return _size.cy;
}

inline int CBaseControl::GetX() const
{
    return _position.x;
}

inline int CBaseControl::GetY() const
{
    return _position.y;
}

inline LONG CBaseControl::GetStyle() const
{
    return _style;
}

inline std::wstring CBaseControl::GetCreateEvent() const
{
    return _createEvent;
}

inline std::wstring CBaseControl::GetDestroyEvent() const
{
    return _destroyEvent;
}

inline std::wstring CBaseControl::GetClickEvent() const
{
    return _clickEvent;
}

inline HWND CBaseControl::GetHWnd() const
{
    return _hWnd;
}

void CBaseControl::SetStyle(const LONG style)
{
    _style = style;
}

void CBaseControl::SetVisible(const bool visible)
{
    _isVisible = visible;
}

void CBaseControl::SetEnabled(const bool enabled)
{
    _isEnabled = enabled;
}

void CBaseControl::SetY(const int y)
{
    _position.y = y;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
        AdjustWindowRect(&rect, _style, FALSE);

        SetWindowPos(_hWnd, NULL, _position.x, _position.y, _size.cx, _size.cy, SWP_NOSIZE || SWP_NOZORDER);
    }
}

void CBaseControl::SetX(const int x)
{
    _position.x = x;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
        AdjustWindowRect(&rect, _style, FALSE);

        SetWindowPos(_hWnd, NULL, _position.x, _position.y, _size.cx, _size.cy, SWP_NOSIZE || SWP_NOZORDER);
    }
}

void CBaseControl::SetWidth(const int width)
{
    _size.cx = width;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
        AdjustWindowRect(&rect, _style, FALSE);

        SetWindowPos(_hWnd, NULL, _position.x, _position.y, _size.cx, _size.cy, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void CBaseControl::SetHeight(const int height)
{
    _size.cy = height;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
        AdjustWindowRect(&rect, _style, FALSE);
        SetWindowPos(_hWnd, NULL, _position.x, _position.y, _size.cx, _size.cy, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void CBaseControl::SetCreateEvent(const std::wstring createEvent)
{
    _createEvent = createEvent;
}

void CBaseControl::SetDestroyEvent(const std::wstring destroyEvent)
{
    _destroyEvent = destroyEvent;
}

void CBaseControl::SetClickEvent(const std::wstring clickEvent)
{
    _clickEvent = clickEvent;
}

void CBaseControl::Show()
{
    ShowWindow(_hWnd, TRUE);
}

void CBaseControl::Hide()
{
    ShowWindow(_hWnd, FALSE);
}
}