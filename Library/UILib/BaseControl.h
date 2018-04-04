#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <windowsx.h>

#include <string>

namespace jojogame {
class  CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CBaseControl();
    virtual ~CBaseControl();

    virtual bool IsEnabled() const;
    virtual bool IsVisible() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual LONG GetStyle() const;
    virtual std::wstring GetCreateEvent() const;
    virtual std::wstring GetDestroyEvent() const;
    virtual std::wstring GetMouseLButtonUpEvent() const;
    virtual std::wstring GetMouseLButtonDownEvent() const;
    virtual std::wstring GetMouseMoveEvent() const;
    virtual std::wstring GetMouseEnterEvent() const;
    virtual std::wstring GetMouseLeaveEvent() const;
    virtual HWND GetHWnd() const;
    virtual std::wstring GetUserData();
    virtual std::wstring GetType();

    virtual void SetStyle(LONG style);
    virtual void SetEnabled(bool isEnabled);
    virtual void SetY(int y);
    virtual void SetX(int x);
    virtual void SetWidth(int width);
    virtual void SetHeight(int height);
    virtual void SetCreateEvent(std::wstring createEvent);
    virtual void SetDestroyEvent(std::wstring destroyEvent);
    virtual void SetMouseLButtonUpEvent(std::wstring mouseLButtonUpEvent);
    virtual void SetMouseLButtonDownEvent(std::wstring mouseLButtonDownEvent);
    virtual void SetMouseMoveEvent(std::wstring mouseMoveEvent);
    virtual void SetMouseEnterEvent(std::wstring mouseEnterEvent);
    virtual void SetMouseLeaveEvent(std::wstring mouseLeaveEvent);
    virtual void SetUserData(std::wstring data);
    virtual void SetParentControl(CBaseControl *parent);

    virtual void Refresh();
    virtual void RefreshRegion(int left, int top, int right, int bottom);

    virtual void Show();
    virtual void Hide();

    virtual bool Create() = 0;
    virtual void Destroy() = 0;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }

protected:
    bool _isHover = false;

    bool _isVisible = false;
    bool _isEnabled = true;
    HWND _hWnd = nullptr;
    CBaseControl *_parentControl = nullptr;
    SIZE _size = SIZE{ 0, 0 };
    POINT _position = POINT{ 0, 0 };
    LONG _style = 0;

    std::wstring _createEvent = L"";
    std::wstring _destroyEvent = L"";
    std::wstring _mouseLButtonUpEvent = L"";
    std::wstring _mouseLButtonDownEvent = L"";
    std::wstring _mouseMoveEvent = L"";
    std::wstring _mouseEnterEvent = L"";
    std::wstring _mouseLeaveEvent = L"";

    std::wstring _userData = L"";

    std::wstring _type = L"";
};
}
