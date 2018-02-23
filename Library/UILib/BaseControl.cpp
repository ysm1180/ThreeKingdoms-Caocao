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
    LUA_METHOD(SetMouseLButtonUpEvent);
    LUA_METHOD(SetMouseLButtonDownEvent);
    LUA_METHOD(SetMouseMoveEvent);
    LUA_METHOD(SetMouseHoverEvent);
    LUA_METHOD(SetMouseLeaveEvent);
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

int CBaseControl::GetX() const
{
    if (_hWnd)
    {
        RECT rect;
        GetWindowRect(_hWnd, &rect);
        return rect.left;
    }
    else
    {
        return _position.x;
    }
}

int CBaseControl::GetY() const
{
    if (_hWnd)
    {
        RECT rect;
        GetWindowRect(_hWnd, &rect);
        return rect.top;
    }
    else
    {
        return _position.y;
    }
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

inline std::wstring CBaseControl::GetMouseLButtonUpEvent() const
{
    return _mouseLButtonUpEvent;
}

std::wstring CBaseControl::GetMouseLButtonDownEvent() const
{
    return _mouseLButtonDownEvent;
}

std::wstring CBaseControl::GetMouseMoveEvent() const
{
    return _mouseMoveEvent;
}

std::wstring CBaseControl::GetMouseHoverEvent() const
{
    return _mouseHoverEvent;
}

std::wstring CBaseControl::GetMouseLeaveEvent() const
{
    return _mouseLeaveEvent;
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

        SetRect(&rect, GetX(), _position.y, GetX() + GetWidth(), _position.y + GetHeight());
        AdjustWindowRect(&rect, _style, FALSE);
        int diffX = GetX() - rect.left;
        int diffY = _position.y - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_NOSIZE || SWP_NOZORDER);
    }
}

void CBaseControl::SetX(const int x)
{
    _position.x = x;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, _position.x, GetY(), _position.x + GetWidth(), GetY() + GetHeight());
        AdjustWindowRect(&rect, _style, FALSE);
        int diffX = _position.x - rect.left;
        int diffY = GetY() - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_NOSIZE || SWP_NOZORDER);
    }
}

void CBaseControl::SetWidth(const int width)
{
    _size.cx = width;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, GetX(), GetY(), GetX() + _size.cx, GetY() + GetHeight());
        AdjustWindowRect(&rect, _style, FALSE);
        int diffX = GetX() - rect.left;
        int diffY = GetY() - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void CBaseControl::SetHeight(const int height)
{
    _size.cy = height;

    if (_hWnd != nullptr)
    {
        RECT rect;

        SetRect(&rect, GetX(), GetY(), GetX() + GetWidth(), GetY() + _size.cy);
        AdjustWindowRect(&rect, _style, FALSE);
        int diffX = GetX() - rect.left;
        int diffY = GetY() - rect.top;
        SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

        SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE | SWP_NOZORDER);
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

void CBaseControl::SetMouseLButtonUpEvent(const std::wstring mouseLButtonUpEvent)
{
    _mouseLButtonUpEvent = mouseLButtonUpEvent;
}

void CBaseControl::SetMouseLButtonDownEvent(std::wstring mouseLButtonDownEvent)
{
    _mouseLButtonDownEvent = mouseLButtonDownEvent;
}

void CBaseControl::SetMouseMoveEvent(std::wstring mouseMoveEvent)
{
    _mouseMoveEvent = mouseMoveEvent;
}

void CBaseControl::SetMouseHoverEvent(std::wstring mouseHoverEvent)
{
    _mouseHoverEvent = mouseHoverEvent;
}

void CBaseControl::SetMouseLeaveEvent(std::wstring mouseLeaveEvent)
{
    _mouseLeaveEvent = mouseLeaveEvent;
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