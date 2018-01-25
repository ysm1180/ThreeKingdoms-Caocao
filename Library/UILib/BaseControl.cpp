#include "stdafx.h"

#include "BaseControl.h"
#include "ControlManager.h"

namespace jojogame {
void CBaseControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CBaseControl, "BaseControl");

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

    LUA_METHOD(MoveToCenter);
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
}

void CBaseControl::SetX(const int x)
{
    _position.x = x;
}

void CBaseControl::SetWidth(const int width)
{
    _size.cx = width;
}

void CBaseControl::SetHeight(const int height)
{
    _size.cy = height;
}

void CBaseControl::SetCreateEvent(const std::wstring createEventName)
{
    _createEvent = createEventName;
}

void CBaseControl::SetDestroyEvent(const std::wstring destroyEventName)
{
    _destroyEvent = destroyEventName;
}

void CBaseControl::SetClickEvent(const std::wstring clickEventName)
{
    _clickEvent = clickEventName;
}

void CBaseControl::Show()
{
    ShowWindow(_hWnd, TRUE);
}

void CBaseControl::Hide()
{
    ShowWindow(_hWnd, FALSE);
}

void CBaseControl::MoveToCenter()
{
    auto parentHWnd = _parentHWnd;
    if (parentHWnd == nullptr)
    {
        parentHWnd = GetDesktopWindow();
    }

    RECT parentRect;
    GetWindowRect(parentHWnd, &parentRect);

    const int parentWidth = parentRect.right - parentRect.left;
    const int parentHeight = parentRect.bottom - parentRect.top;
    _position.x = parentWidth / 2 - _size.cx / 2;
    _position.y = parentHeight / 2 - _size.cy / 2;

    if (_hWnd != nullptr)
    {
        MoveWindow(_hWnd, _position.x, _position.y, _size.cx, _size.cy, TRUE);
    }
}
}