#include "BaseControl.h"
#include "ControlManager.h"
#include "LuaLib\LuaTinker.h"
#include <CommCtrl.h>

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
    LUA_METHOD(GetCreateEvent);
    LUA_METHOD(GetDestroyEvent);
    LUA_METHOD(GetMouseLButtonUpEvent);
    LUA_METHOD(GetMouseLButtonDownEvent);
    LUA_METHOD(GetMouseMoveEvent);
    LUA_METHOD(GetMouseEnterEvent);
    LUA_METHOD(GetMouseLeaveEvent);
    LUA_METHOD(GetStyle);
    LUA_METHOD(GetUserData);

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
    LUA_METHOD(SetMouseEnterEvent);
    LUA_METHOD(SetMouseLeaveEvent);
    LUA_METHOD(SetStyle);
    LUA_METHOD(SetUserData);

    LUA_METHOD(Refresh);
    LUA_METHOD(RefreshRegion);

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
    return _position.x;
}

int CBaseControl::GetY() const
{
    if (_hWnd)
    {
        RECT rect;
        GetWindowRect(_hWnd, &rect);
        return rect.top;
    }
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

std::wstring CBaseControl::GetMouseEnterEvent() const
{
    return _mouseEnterEvent;
}

std::wstring CBaseControl::GetMouseLeaveEvent() const
{
    return _mouseLeaveEvent;
}

inline HWND CBaseControl::GetHWnd() const
{
    return _hWnd;
}

std::wstring CBaseControl::GetUserData()
{
    return _userData;
}

void CBaseControl::SetStyle(const LONG style)
{
    if (_style != style)
    {
        _style = style;
    }
}

void CBaseControl::SetVisible(const bool isVisible)
{
    if (_isVisible != isVisible)
    {
        _isVisible = isVisible;
    }
}

void CBaseControl::SetEnabled(const bool isEnabled)
{
    if (_isEnabled != isEnabled)
    {
        _isEnabled = isEnabled;

        if (_hWnd)
        {
            EnableWindow(_hWnd, _isEnabled);
        }
    }
}

void CBaseControl::SetY(const int y)
{
    if (_position.y != y)
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

            SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE || SWP_NOZORDER);
        }
    }
}

void CBaseControl::SetX(const int x)
{
    if (_position.x != x)
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

            SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE || SWP_NOZORDER);
        }
    }
}

void CBaseControl::SetWidth(const int width)
{
    if (_size.cx != width)
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

            SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
        }
    }
}

void CBaseControl::SetHeight(const int height)
{
    if (_size.cy != height)
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

            SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
        }
    }
}

void CBaseControl::SetCreateEvent(const std::wstring createEvent)
{
    if (_createEvent.compare(createEvent) != 0)
    {
        _createEvent = createEvent;
    }
}

void CBaseControl::SetDestroyEvent(const std::wstring destroyEvent)
{
    if (_destroyEvent.compare(destroyEvent) != 0)
    {
        _destroyEvent = destroyEvent;
    }
}

void CBaseControl::SetMouseLButtonUpEvent(const std::wstring mouseLButtonUpEvent)
{
    if (_mouseLButtonUpEvent.compare(mouseLButtonUpEvent) != 0)
    {
        _mouseLButtonUpEvent = mouseLButtonUpEvent;
    }
}

void CBaseControl::SetMouseLButtonDownEvent(std::wstring mouseLButtonDownEvent)
{
    if (_mouseLButtonDownEvent.compare(mouseLButtonDownEvent) != 0)
    {
        _mouseLButtonDownEvent = mouseLButtonDownEvent;
    }
}

void CBaseControl::SetMouseMoveEvent(std::wstring mouseMoveEvent)
{
    if (_mouseMoveEvent.compare(mouseMoveEvent) != 0)
    {
        _mouseMoveEvent = mouseMoveEvent;
    }
}

void CBaseControl::SetMouseEnterEvent(std::wstring mouseEnterEvent)
{
    if (_mouseEnterEvent.compare(mouseEnterEvent) != 0)
    {
        _mouseEnterEvent = mouseEnterEvent;
    }
}

void CBaseControl::SetMouseLeaveEvent(std::wstring mouseLeaveEvent)
{
    if (_mouseLeaveEvent.compare(mouseLeaveEvent) != 0)
    {
        _mouseLeaveEvent = mouseLeaveEvent;
    }
}

void CBaseControl::SetUserData(std::wstring data)
{
    if (_userData.compare(data) != 0)
    {
        _userData = data;
    }
}

void CBaseControl::SetParentControl(CBaseControl* parent)
{
    _parentControl = parent;
}

void CBaseControl::Refresh()
{
    if (_hWnd)
    {
        RECT rect;

        GetClientRect(_hWnd, &rect);
        InvalidateRect(_hWnd, &rect, TRUE);
        UpdateWindow(_hWnd);
    }
}

void CBaseControl::RefreshRegion(int left, int top, int right, int bottom)
{
    if (_hWnd)
    {
        RECT rect;

        SetRect(&rect, left, top, right, bottom);
        InvalidateRect(_hWnd, &rect, TRUE);
        UpdateWindow(_hWnd);
    }
}


void CBaseControl::Show()
{
    _isVisible = true;
    if (_hWnd)
    {
        ShowWindow(_hWnd, TRUE);
    }
}

void CBaseControl::Hide()
{
    _isVisible = false;
    if (_hWnd)
    {
        ShowWindow(_hWnd, FALSE);
    }
}
}