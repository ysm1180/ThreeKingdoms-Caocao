#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <string>

#include "LuaLib\lua_tinker_helper.h"

namespace three_kingdoms {
class CBaseControl {
 public:
  static void RegisterFunctions(lua_State* L);

  CBaseControl();
  virtual ~CBaseControl();

  virtual bool IsHover() const;
  virtual bool IsPressed() const;
  virtual bool IsEnabled() const;
  virtual bool IsVisible() const;
  virtual int GetWidth() const;
  virtual int GetHeight() const;
  virtual int GetX() const;
  virtual int GetY() const;
  virtual LONG GetStyle() const;
  virtual int GetCreateEvent() const;
  virtual int GetDestroyEvent() const;
  virtual int GetMouseLButtonUpEvent() const;
  virtual int GetMouseLButtonDownEvent() const;
  virtual int GetMouseMoveEvent() const;
  virtual int GetMouseEnterEvent() const;
  virtual int GetMouseLeaveEvent() const;
  virtual HWND GetHWnd() const;
  virtual std::wstring GetUserData();
  virtual std::wstring GetType();

  virtual void SetStyle(LONG style);
  virtual void SetEnabled(bool isEnabled);
  virtual void SetY(int y);
  virtual void SetX(int x);
  virtual void SetWidth(int width);
  virtual void SetHeight(int height);
  virtual void SetCreateEvent();
  virtual void SetDestroyEvent();
  virtual void SetMouseLButtonUpEvent();
  virtual void SetMouseLButtonDownEvent();
  virtual void SetMouseMoveEvent();
  virtual void SetMouseEnterEvent();
  virtual void SetMouseLeaveEvent();
  virtual void SetUserData(std::wstring data);
  virtual void SetParentControl(CBaseControl* parent);

  virtual void Refresh();
  virtual void RefreshRegion(int left, int top, int right, int bottom);

  virtual void Show();
  virtual void Hide();

  virtual bool Create() = 0;
  virtual void Destroy() {
    if (_hWnd != nullptr) {
      DestroyWindow(_hWnd);
      _hWnd = nullptr;
    }
  }

  static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam) {
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
  }

 protected:
  bool _isHover = false;
  bool _isPressed = false;

  bool _isVisible = false;
  bool _isEnabled = true;
  HWND _hWnd = nullptr;
  CBaseControl* _parentControl = nullptr;
  SIZE _size = SIZE{0, 0};
  POINT _position = POINT{0, 0};
  LONG _style = 0;

  int _createEvent = LUA_NOREF;
  int _destroyEvent = LUA_NOREF;
  int _mouseLButtonUpEvent = LUA_NOREF;
  int _mouseLButtonDownEvent = LUA_NOREF;
  int _mouseMoveEvent = LUA_NOREF;
  int _mouseEnterEvent = LUA_NOREF;
  int _mouseLeaveEvent = LUA_NOREF;

  std::wstring _userData = L"";

  std::wstring _type = L"";
};
}  // namespace three_kingdoms
