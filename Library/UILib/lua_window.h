#pragma once

#include <string>
#include <vector>

namespace three_kingdoms {
class LuaWindowControl {
 public:
  void Create();

 private:
  int *_dialogResult = nullptr;

  std::wstring title_name_ = L"";

  int _activeEvent = LUA_NOREF;
  int _closeEvent = LUA_NOREF;
  int _sizeEvent = LUA_NOREF;
  int _keyDownEvent = LUA_NOREF;
  int _keyUpEvent = LUA_NOREF;

  COLORREF _backgroundColor = GetSysColor(COLOR_3DFACE);
  HBRUSH _backBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

  CMenu *_menu = nullptr;
  CToolbarControl *_toolbar = nullptr;
};
} // namespace three_kingdoms

