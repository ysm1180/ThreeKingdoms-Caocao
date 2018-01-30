#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <windowsx.h>

#include <string>

namespace jojogame {
class CBaseControl
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
    virtual std::wstring GetClickEvent() const;
    virtual HWND GetHWnd() const;

    virtual void SetStyle(LONG style);
    virtual void SetVisible(bool isVisible);
    virtual void SetEnabled(bool isEnabled);
    virtual void SetY(int x);
    virtual void SetX(int y);
    virtual void SetWidth(int width);
    virtual void SetHeight(int height);
    virtual void SetCreateEvent(std::wstring createEvent);
    virtual void SetDestroyEvent(std::wstring destroyEvent);
    virtual void SetClickEvent(std::wstring clickEvent);

    virtual void Show();
    virtual void Hide();

    virtual bool Create() = 0;
    virtual void Destroy() = 0;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
protected:
    bool _isVisible = false;
    bool _isEnabled = true;
    HWND _hWnd = nullptr;
    HWND _parentHWnd = nullptr;
    SIZE _size = SIZE{ 0, 0 };
    POINT _position = POINT{ 0, 0 };
    LONG _style = 0;

    std::wstring _createEvent = L"";
    std::wstring _destroyEvent = L"";
    std::wstring _clickEvent = L"";
};
}
