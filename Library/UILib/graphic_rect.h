#pragma once

#include <windows.h>

#include "LuaLib\lua_tinker_helper.h"

namespace three_kingdoms {
class CGraphicRect {
 public:
  static void RegisterFunctions(lua_State *L);

  CGraphicRect();
  virtual ~CGraphicRect();

  COLORREF GetColor();
  int GetWidth();
  int GetHeight();
  double GetOpacity();

  void SetColor(COLORREF color);
  void SetWidth(int width);
  void SetHeight(int height);
  void SetOpacity(double opacity);

 private:
  COLORREF _color = 0;
  SIZE _size{0, 0};
  double _opacity = 1.0;
};
}  // namespace three_kingdoms
