#pragma once

#include <string>


#include "LuaLib\LuaTinker.h"

namespace three_kingdoms {
class LuaBaseControl {
 public:
  static void RegisterFunctions(lua_State* L);

  LuaBaseControl();
  virtual ~LuaBaseControl();

  virtual bool IsHover() const;
  virtual bool IsPressed() const;
  virtual bool IsEnabled() const;
  virtual bool IsVisible() const;
  virtual int GetWidth() const;
  virtual int GetHeight() const;
  virtual int GetX() const;
  virtual int GetY() const;
  virtual int GetCreateEvent() const;
  virtual int GetDestroyEvent() const;
  virtual int GetMouseLButtonUpEvent() const;
  virtual int GetMouseLButtonDownEvent() const;
  virtual int GetMouseMoveEvent() const;
  virtual int GetMouseEnterEvent() const;
  virtual int GetMouseLeaveEvent() const;
  virtual std::wstring GetUserData();
  virtual std::wstring GetType();

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
    
  }

 protected:
  bool _isHover = false;
  bool _isPressed = false;

  bool _isVisible = false;
  bool _isEnabled = true;
  SIZE _size = SIZE{0, 0};
  std::pair _position = POINT{0, 0};
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
